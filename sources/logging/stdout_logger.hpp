#ifndef LIBS_STDOUT_LOGGER_HPP
#define LIBS_STDOUT_LOGGER_HPP

#include <iostream>
#include <vector>
#include <cinttypes>
#include <utility>
#include <string_view>

#include "string_logger.hpp"
#include "../facilities/strings.hpp"


namespace libs::logging {

    class StdOutLogger : public StringLogger {
      public:
        explicit StdOutLogger(bool isEnabled)
        {
            elicitSettings().is_enabled = isEnabled;
            elicitSettings().messages_before_flush = 4;
            elicitSettings().use_stdout = true;
        }

        StdOutLogger() : StdOutLogger(true)
        {
        }

        StdOutLogger(const StdOutLogger& rhs) = delete;

        StdOutLogger(StdOutLogger&& rhs) = delete;

        StdOutLogger& operator=(const StdOutLogger& rhs) = delete;

        StdOutLogger& operator=(StdOutLogger&& rhs) noexcept = delete;

        ~StdOutLogger() noexcept override
        {
            onDestructor();
            print();
            printDestructorMessage();
        }

      protected:
        enum class MessageTarget : bool {
            StdOut,
            StdErr
        };

        using ClassifiedMessage = std::pair<MessageTarget, std::string>;
        using PositionedMessage = std::pair<std::size_t, std::string>;

        void onFlush() override
        {
            print();
            last_message_position = 0;
            messages.clear();
        }

        void onBufferedMessage() override
        {
            const auto str = elicitStringBuffer().str();
            auto[nextMessagePosition, lastMessage] = GetNextMessage(str, last_message_position);
            last_message_position = nextMessagePosition;
            messages.emplace_back(GetClassifiedMessage(lastMessage));
        }

        static PositionedMessage GetNextMessage(std::string_view bufferStr, const std::size_t& lastMsgPos)
        {
            const auto lastMessage = std::string(bufferStr.substr(lastMsgPos));
            const auto nextMsgPos = lastMsgPos + lastMessage.size();
            return std::make_pair<>(nextMsgPos, lastMessage);
        }

        static ClassifiedMessage GetClassifiedMessage(const std::string& lastMessage)
        {
            using libs::facilities::string::ToUpperCase;
            using libs::facilities::string::Contains;

            const auto fatalLevel = ToUpperCase(SeverityLevel::ToString(SeverityLevel::Level::Fatal));
            bool isExceptionMsg = Contains(lastMessage, "std::exception |");
            bool isFatalMsg = Contains(lastMessage, "[" + fatalLevel + "]");
            bool isErrorMsg = isExceptionMsg || isFatalMsg;
            const auto key = isErrorMsg ? MessageTarget::StdErr : MessageTarget::StdOut;
            return std::make_pair<>(key, lastMessage);
        }

      private:
        std::vector<std::pair<MessageTarget, std::string>> messages{};
        std::size_t last_message_position = 0;


        void print()
        {
            if (!elicitSettings().use_stdout) {
                return;
            }

            printMessages();
        }

        void printMessages() const
        {
            for (const auto&[msgTarget, message] : messages) {
                if (msgTarget == MessageTarget::StdOut) {
                    std::cout << message;
                    continue;
                }

                std::clog << message;
            }
        }

        void printDestructorMessage()
        {
            if (!elicitSettings().use_stdout) {
                return;
            }

            std::cout << "~StdOutLogger(" << elicitFlushedMessages() << ")\n";
        }

    };

}

#endif //LIBS_STDOUT_LOGGER_HPP
