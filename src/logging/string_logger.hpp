#ifndef STRING_LOGGER_HPP
#define STRING_LOGGER_HPP

#include "logger.hpp"
#include <sstream>
#include <ctime>


namespace logging {

    struct LoggerMsgHelper {
    public:
        std::string date;
        std::string time;
        // 2021-07-22 19:46 DEBUG string_logger.cpp(15:55) @"struct LoggerMsgHelper()": Hello log!
        // $Date $Time $Level $SourceFile($Line:$Column) @"$FunctionPrototype": $Message

        void build() {
            const auto dateTime = GetDateTime();
            const auto delimiterPos = dateTime.find('_');
            date = dateTime.substr(0, delimiterPos);
            time = dateTime.substr(delimiterPos + 1);
        }

    private:
        static std::string GetDateTime() {
            time_t now = std::time(0);
            struct tm t_struct{};
            const int BUFFER_SIZE = 80;
            char buf[BUFFER_SIZE];
            t_struct = *localtime(&now);
            strftime(buf, sizeof(buf), "%Y-%m-%d_%X", &t_struct);
            std::string result(buf);
            return result;
        }
    };


    class StringLogger : public Logger {
    private:
        std::string& target;
        std::ostringstream str_buffer{};
        unsigned short buffered_messages = 0;

    public:
        explicit StringLogger(std::string& target) : target(target) {
        }

        void fatal(const std::string& msg) override {
            log(SeverityLevel::Level::Fatal, msg);
        }

        void warning(const std::string& msg) override {
            log(SeverityLevel::Level::Warning, msg);
        }

        void info(const std::string& msg,
                  std::experimental::source_location source = std::experimental::source_location::current()) override {
            // TODO: check it out
            log(SeverityLevel::Level::Info, msg);
        }

        void debug(const std::string& msg) override {
            log(SeverityLevel::Level::Debug, msg);
        }

        void exception(const std::string& msg, const std::exception& e) override {
            // TODO: handle exception logging
        }

    private:
        void log(SeverityLevel::Level level, const std::string& msg) override {
            if (!elicitSettings().is_enabled) {
                return;
            }

            // TODO: build message string and put it in a buffer
            ++buffered_messages;

            if (buffered_messages > elicitSettings().messages_before_flush) {
                flush();
            }
        }

        /// Flush string buffer into target.
        virtual void flush() {
            target.append(str_buffer.str());
            str_buffer.str("");
            str_buffer.clear();
            buffered_messages = 0;
        }
    };

}

#endif //STRING_LOGGER_HPP
