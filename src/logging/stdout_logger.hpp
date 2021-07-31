#ifndef LIBS_LOGGING_STDOUT_LOGGER_HPP
#define LIBS_LOGGING_STDOUT_LOGGER_HPP

#include <iostream>
#include <vector>
#include <cinttypes>
#include <utility>

#include "string_logger.hpp"
#include "../facilities/strings.hpp"


namespace libs::logging {

    class StdOutLogger : public StringLogger {
      public:
        StdOutLogger() = delete;

        explicit StdOutLogger(bool isEnabled, unsigned short messagesBeforeFlush = 4) : StringLogger(str_target)
        {
            elicitSettings().is_enabled = isEnabled;
            elicitSettings().messages_before_flush = messagesBeforeFlush;
        }

        StdOutLogger(const StdOutLogger& rhs) = delete;

        StdOutLogger(StdOutLogger&& rhs) = delete;

        StdOutLogger& operator=(const StdOutLogger& rhs) = delete;

        StdOutLogger& operator=(StdOutLogger&& rhs) noexcept = delete;

        ~StdOutLogger() noexcept override
        {
            onFlushInDerivedDestructor();
            print();
            str_target.clear();
            printDestructorMessage();
        }

      protected:
        enum class MessageTarget : bool {
            StdOut,
            StdErr
        };

        void onFlush() override
        {
            print();
            last_message_position = 0;
            messages.clear();
        }

        void onBufferedMessage() override
        {
            const auto buffer = elicitStringBuffer().str();
            const auto lastMessage = buffer.substr(last_message_position);
            last_message_position += lastMessage.size();

            using libs::facilities::string::ToUpperCase;
            using libs::facilities::string::Contains;
            const auto fatalLevel = ToUpperCase(SeverityLevel::ToString(SeverityLevel::Level::Fatal));
            bool isExceptionMsg = Contains(lastMessage, "std::exception |");
            bool isFatalMsg = Contains(lastMessage, "[" + fatalLevel + "]");
            bool isErrorMsg = isExceptionMsg || isFatalMsg;
            const auto key = isErrorMsg ? MessageTarget::StdErr : MessageTarget::StdOut;
            messages.emplace_back(std::make_pair<>(key, lastMessage));
        }

        inline const auto& elicitStdOutMessages() const noexcept
        {
            return messages;
        }

      private:
        std::vector<std::pair<MessageTarget, std::string>> messages{};
        std::size_t last_message_position = 0;
        std::string str_target{};


        void print()
        {
            if (elicitSettings().skip_print_to_stdout) {
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
            if (elicitSettings().skip_print_to_stdout) {
                return;
            }

            std::cout << "~StdOutLogger(" << elicitFlushedMessages() << ")\n";
        }

    };

}

#endif //LIBS_LOGGING_STDOUT_LOGGER_HPP
