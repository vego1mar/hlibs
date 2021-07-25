#ifndef STDOUT_LOGGER_HPP
#define STDOUT_LOGGER_HPP

#include <iostream>
#include <vector>
#include <cinttypes>
#include <utility>

#include "string_logger.hpp"
#include "../strings/strings.hpp"


namespace logging {

    class StdOutLogger : public StringLogger {
    protected:
        enum class MessageTarget : bool {
            StdOut,
            StdErr
        };

    private:
        std::vector<std::pair<MessageTarget, std::string>> messages{};
        std::size_t last_message_position = 0;
        std::string str_target{};

    public:
        StdOutLogger() = delete;

        explicit StdOutLogger(bool isEnabled, unsigned short messagesBeforeFlush = 4) : StringLogger(str_target) {
            elicitSettings().is_enabled = isEnabled;
            elicitSettings().messages_before_flush = messagesBeforeFlush;
        }

        StdOutLogger(const StdOutLogger& rhs) = delete;

        StdOutLogger(StdOutLogger&& rhs) = delete;

        StdOutLogger& operator=(const StdOutLogger& rhs) = delete;

        StdOutLogger& operator=(StdOutLogger&& rhs) noexcept = delete;

        ~StdOutLogger() noexcept override {
            onFlushInDerivedDestructor();
            print();
            str_target.clear();
            printDestructorMessage();
        }

    private:
        void print() {
            if (elicitSettings().skip_print_to_stdout) {
                return;
            }

            printMessages();
        }

        void printMessages() const {
            for (const auto&[msgTarget, message] : messages) {
                if (msgTarget == MessageTarget::StdOut) {
                    std::cout << message;
                    continue;
                }

                std::clog << message;
            }
        }

        void printDestructorMessage() {
            if (elicitSettings().skip_print_to_stdout) {
                return;
            }

            std::cout << "~StdOutLogger(" << elicitFlushedMessages() << ")\n";
        }

    protected:
        void onFlush() override {
            print();
            last_message_position = 0;
            messages.clear();
        }

        void onBufferedMessage() override {
            const auto buffer = elicitStringBuffer().str();
            const auto lastMessage = buffer.substr(last_message_position);
            last_message_position += lastMessage.size();

            const auto fatalLevel = strings::ToUpperCase(SeverityLevel::ToString(SeverityLevel::Level::Fatal));
            bool isExceptionMsg = strings::Contains(lastMessage, "std::exception |");
            bool isFatalMsg = strings::Contains(lastMessage, "[" + fatalLevel + "]");
            bool isErrorMsg = isExceptionMsg || isFatalMsg;
            const auto key = isErrorMsg ? MessageTarget::StdErr : MessageTarget::StdOut;
            messages.emplace_back(std::make_pair<>(key, lastMessage));
        }

        inline const auto& elicitStdOutMessages() const noexcept {
            return messages;
        }

    };

}

#endif //STDOUT_LOGGER_HPP
