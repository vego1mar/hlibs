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
            using libs::types::RGBColor;
            using libs::types::ColorName;
            using libs::standard::ansi::ANSISequencer;
            using CP = libs::standard::ansi::SGRSequencer::ColorType;

            const auto resetColors = ANSISequencer::ResetColors();
            const auto crimson = con_ansi_sequencer.setColor(RGBColor(ColorName::Crimson), CP::Foreground);
            const auto purple = con_ansi_sequencer.setColor(RGBColor(128, 0, 128), CP::Foreground);

            for (const auto&[level, message] : con_messages) {
                switch (level) {
                    case SeverityLevel::Level::Debug:
                        printDebugMessage(message);
                        break;
                    case SeverityLevel::Level::Fatal:
                        std::cout << crimson << message << resetColors;
                        break;
                    case SeverityLevel::Level::Warning:
                        std::cout << purple << message << resetColors;
                        break;
                    case SeverityLevel::Level::Info:
                        std::cout << message << resetColors;
                        break;
                }
            }
        }

        void printDebugMessage(const std::string& message) const
        {
            using libs::facilities::string::Contains;
            using libs::types::RGBColor;
            using libs::types::ColorName;
            using libs::standard::ansi::ANSISequencer;
            using CP = libs::standard::ansi::SGRSequencer::ColorType;
            using Format = libs::standard::ansi::ANSISequencer::DisplayFormat;

            bool isDebugMsg = Contains(message, GetLevelStr(SeverityLevel::Level::Debug));
            bool mayLogException = Contains(message, "std::exception");
            bool isExceptionMsg = isDebugMsg && mayLogException;

            if (isExceptionMsg) {
                const auto greenFG = con_ansi_sequencer.setColor(RGBColor(0, 200, 0), CP::Foreground);
                std::cout << greenFG << message << ANSISequencer::ResetColors();
                return;
            }

            const auto invert = ANSISequencer::SetDisplay(Format::Invert);
            const auto revert = ANSISequencer::SetDisplay(Format::NotInverted);
            std::cout << invert << message << revert;
        }
    };

}

#endif //LIBS_CONSOLE_LOGGER_HPP
