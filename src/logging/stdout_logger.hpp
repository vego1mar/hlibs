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
    private:
        enum class MessageTarget : uint8_t {
            StdOut,
            StdErr
        };

        std::vector<std::pair<MessageTarget, std::string>> messages{};
        std::size_t last_message_position = 0;
        std::string str_target{};

    public:
        StdOutLogger() = delete;

        explicit StdOutLogger(bool isEnabled, unsigned short messagesBeforeFlush = 4) : StringLogger(str_target) {
            elicitSettings().is_enabled = isEnabled;
            elicitSettings().messages_before_flush = messagesBeforeFlush;
            // TODO: possibly change parameter to more meaningful: hash/id?
        }

        StdOutLogger(const StdOutLogger& rhs) = delete;

        StdOutLogger(StdOutLogger&& rhs) = delete;

        StdOutLogger& operator=(const StdOutLogger& rhs) = delete;

        StdOutLogger& operator=(StdOutLogger&& rhs) noexcept = delete;

        ~StdOutLogger() noexcept override {
            flushStringBufferInDerivedDestructor();
            print();
            str_target.clear();
            std::cout << "~StdOutLogger(" << elicitFlushedMessages() << ")\n";
        }

    protected:
        void onFlush() override {
            print();
            last_message_position = 0;
            messages.clear();
        }

        void print() {
            for (const auto& item : messages) {
                const auto& target = item.first;
                const auto& message = item.second;

                if (target == MessageTarget::StdOut) {
                    std::cout << message;
                    continue;
                }

                std::clog << message;
            }
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

    };

}

#endif //STDOUT_LOGGER_HPP
