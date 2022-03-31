#ifndef LIBS_CONSOLE_LOGGER_HPP
#define LIBS_CONSOLE_LOGGER_HPP

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <cinttypes>

#include "stdout_logger.hpp"
#include "../standard/ansi.hpp"


namespace hlibs::logging {

    /// Uses ANSI escape characters to print colourful output.
    class ConsoleLogger : public StdOutLogger {
      public:
        ConsoleLogger() : StdOutLogger(true)
        {
            elicitSettings().messages_before_flush = 1;
            elicitSettings().use_stdout = true;
            buildCache();
        }

        ConsoleLogger(const ConsoleLogger&) = delete;

        ConsoleLogger(ConsoleLogger&&) noexcept = delete;

        ConsoleLogger& operator=(const ConsoleLogger&) = delete;

        ConsoleLogger& operator=(ConsoleLogger&&) noexcept = delete;

        ~ConsoleLogger() noexcept override
        {
            onDestructor();
            printToConsole();

            if (!elicitSettings().use_stdout) {
                return;
            }

            std::cout << "~ConsoleLogger(" << elicitFlushedMessages() << ")\n";
            elicitSettings().use_stdout = false;
        }

      protected:
        void onFlush() override
        {
            if (!elicitSettings().use_stdout) {
                return;
            }

            printToConsole();
            con_last_message_pos = 0;
            con_messages.clear();
        }

        void onBufferedMessage() override
        {
            const auto str = elicitStringBuffer().str();
            auto[nextMsgPosition, lastMessage] = GetNextMessage(str, con_last_message_pos);
            con_last_message_pos = nextMsgPosition;

            auto[target, message] = GetClassifiedMessage(lastMessage);
            classifyMessageByLevel(message);
        }

      private:
        enum class Cache : uint8_t {
            ResetColors,
            RedFG,
            WhiteFG,
            RedBG,
            Bold,
            NotBold,
            TealBG,
            Invert,
            Revert,
            LevelDebug,
        };

        void classifyMessageByLevel(const std::string& message)
        {
            using hlibs::facilities::strings::Contains;
            using SLevel = SeverityLevel::Level;

            const std::array<SLevel, 4> levels = {
                    SLevel::Fatal, SLevel::Warning, SLevel::Info, SLevel::Debug
            };

            for (const auto& level : levels) {
                if (bool isLevelMsg = Contains(message, GetLevelStr(level)); isLevelMsg) {
                    con_messages.emplace_back(std::make_pair<>(level, message));
                    break;
                }
            }
        }

        static std::string GetLevelStr(SeverityLevel::Level level)
        {
            using hlibs::facilities::strings::ToUpperCase;
            return '[' + ToUpperCase(SeverityLevel::ToString(level)) + ']';
        };

        void printToConsole() const
        {
            for (const auto&[level, message] : con_messages) {
                switch (level) {
                    case SeverityLevel::Level::Debug:
                        printDebugMessage(message);
                        break;
                    case SeverityLevel::Level::Fatal:
                        printFatalMessage(message);
                        break;
                    case SeverityLevel::Level::Warning:
                        printWarningMessage(message);
                        break;
                    case SeverityLevel::Level::Info:
                        printInfoMessage(message);
                        break;
                }
            }
        }

        void printDebugMessage(const std::string& message) const
        {
            using hlibs::facilities::strings::Contains;

            bool isDebugMsg = Contains(message, getCache(Cache::LevelDebug));
            bool mayLogException = Contains(message, "std::exception");

            if (bool isExceptionMsg = isDebugMsg && mayLogException; isExceptionMsg) {
                std::cout << getCache(Cache::TealBG) << getCache(Cache::WhiteFG) << message << getCache(Cache::ResetColors);
                return;
            }

            std::cout << getCache(Cache::Invert) << message << getCache(Cache::Revert);
        }

        void printFatalMessage(const std::string& message) const
        {
            std::cout << getCache(Cache::RedBG) << getCache(Cache::WhiteFG) << getCache(Cache::Bold)
                      << message << getCache(Cache::NotBold) << getCache(Cache::ResetColors);
        }

        void printWarningMessage(const std::string& message) const
        {
            std::cout << getCache(Cache::RedFG) << message << getCache(Cache::ResetColors);
        }

        void printInfoMessage(const std::string& message) const
        {
            std::cout << message << getCache(Cache::ResetColors);
        }

        void buildCache()
        {
            using RGB = hlibs::types::RGBColor;
            using CN = hlibs::types::ColorName;
            using Seq = hlibs::standard::ansi::ANSISequencer;
            using CP = hlibs::standard::ansi::SGRSequencer::ColorType;
            using Fmt = hlibs::standard::ansi::ANSISequencer::DisplayFormat;

            Seq seq{};

            con_cache = {
                    {Cache::ResetColors, Seq::ResetColors()},
                    {Cache::RedFG,       seq.setColor(RGB(255, 0, 0), CP::Foreground)},
                    {Cache::WhiteFG,     seq.setColor(RGB(255, 255, 255), CP::Foreground)},
                    {Cache::RedBG,       seq.setColor(RGB(255, 0, 0), CP::Background)},
                    {Cache::Bold,        Seq::SetDisplay(Fmt::Bold)},
                    {Cache::NotBold,     Seq::SetDisplay(Fmt::NormalIntensity)},
                    {Cache::TealBG,      seq.setColor(RGB(CN::Teal), CP::Background)},
                    {Cache::Invert,      Seq::SetDisplay(Fmt::Invert)},
                    {Cache::Revert,      Seq::SetDisplay(Fmt::NotInverted)},
                    {Cache::LevelDebug,  GetLevelStr(SeverityLevel::Level::Debug)},
            };
        }

        const std::string& getCache(Cache what) const
        {
            return con_cache.at(what);
        }

        std::vector<std::pair<SeverityLevel::Level, std::string>> con_messages{};
        std::unordered_map<Cache, std::string> con_cache;
        std::size_t con_last_message_pos = 0;
    };

}

#endif //LIBS_CONSOLE_LOGGER_HPP
