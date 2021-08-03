#ifndef LIBS_CONSOLE_LOGGER_HPP
#define LIBS_CONSOLE_LOGGER_HPP

#include <iostream>

#include "stdout_logger.hpp"
#include "../standard/ansi.hpp"


namespace libs::logging {

    /// Uses ANSI escape characters to print colourful output.
    class ConsoleLogger : public StdOutLogger {
      public:
        ConsoleLogger() : StdOutLogger(true)
        {
            elicitSettings().messages_before_flush = 1;
            elicitSettings().use_stdout = true;
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
        std::vector<std::pair<SeverityLevel::Level, std::string>> con_messages{};
        libs::standard::ansi::ANSISequencer con_ansi_sequencer{};
        std::size_t con_last_message_pos = 0;


        void classifyMessageByLevel(const std::string& message)
        {
            using libs::facilities::string::Contains;
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
            using libs::facilities::string::ToUpperCase;
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
            using libs::facilities::string::Contains;
            using libs::types::RGBColor;
            using libs::standard::ansi::ANSISequencer;
            using CN = libs::types::ColorName;
            using CP = libs::standard::ansi::SGRSequencer::ColorType;
            using Format = libs::standard::ansi::ANSISequencer::DisplayFormat;

            bool isDebugMsg = Contains(message, GetLevelStr(SeverityLevel::Level::Debug));
            bool mayLogException = Contains(message, "std::exception");
            bool isExceptionMsg = isDebugMsg && mayLogException;

            if (isExceptionMsg) {
                const auto tealBG = con_ansi_sequencer.setColor(RGBColor(CN::Teal), CP::Background);
                const auto whiteFG = con_ansi_sequencer.setColor(RGBColor(255, 255, 255), CP::Foreground);
                const auto reset = ANSISequencer::ResetColors();
                std::cout << tealBG << whiteFG << message << reset;
                return;
            }

            const auto invert = ANSISequencer::SetDisplay(Format::Invert);
            const auto revert = ANSISequencer::SetDisplay(Format::NotInverted);
            std::cout << invert << message << revert;
        }

        void printFatalMessage(const std::string& message) const
        {
            using libs::types::RGBColor;
            using libs::standard::ansi::ANSISequencer;
            using CP = libs::standard::ansi::SGRSequencer::ColorType;
            using Format = libs::standard::ansi::ANSISequencer::DisplayFormat;

            const auto resetColors = ANSISequencer::ResetColors();
            const auto bold = ANSISequencer::SetDisplay(Format::Bold);
            const auto notBold = ANSISequencer::SetDisplay(Format::NormalIntensity);
            const auto whiteFG = con_ansi_sequencer.setColor(RGBColor(255, 255, 255), CP::Foreground);
            const auto redBG = con_ansi_sequencer.setColor(RGBColor(255, 0, 0), CP::Background);
            std::cout << redBG << whiteFG << bold << message << notBold << resetColors;
        }

        void printWarningMessage(const std::string& message) const
        {
            using libs::types::RGBColor;
            using libs::standard::ansi::ANSISequencer;
            using CP = libs::standard::ansi::SGRSequencer::ColorType;

            const auto resetColors = ANSISequencer::ResetColors();
            const auto redFG = con_ansi_sequencer.setColor(RGBColor(255, 0, 0), CP::Foreground);
            std::cout << redFG << message << resetColors;
        }

        void printInfoMessage(const std::string& message) const
        {
            using libs::standard::ansi::ANSISequencer;

            const auto resetColors = ANSISequencer::ResetColors();
            std::cout << message << resetColors;
        }
    };

}

#endif //LIBS_CONSOLE_LOGGER_HPP
