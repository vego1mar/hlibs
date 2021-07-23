#ifndef STRING_LOGGER_HPP
#define STRING_LOGGER_HPP

#include <sstream>
#include <iostream>

#include "logger.hpp"


namespace logging {

    using SourceLocation = std::experimental::source_location;


    class StringLogger : public Logger {
    private:
        std::string& target;
        std::ostringstream str_buffer{};
        unsigned short buffered_messages = 0;

    public:
        explicit StringLogger(std::string& target) : target(target) {
        }

        void fatal(const std::string& msg) override {
            //log(SeverityLevel::Level::Fatal, msg);
        }

        void warning(const std::string& msg) override {
            //log(SeverityLevel::Level::Warning, msg);
        }

        void info(const std::string& msg, SourceLocation source = SourceLocation::current()) override {
            log(SeverityLevel::Level::Info, msg, source);
        }

        void debug(const std::string& msg) override {
            //log(SeverityLevel::Level::Debug, msg);
        }

        void exception(const std::string& msg, const std::exception& e) override {
            // TODO: handle exception logging
        }

    private:
        void log(SeverityLevel::Level level, const std::string& msg, SourceLocation source) override {
            if (!elicitSettings().is_enabled) {
                return;
            }

            // TODO: build message string and put it in a buffer
            str_buffer << '@' << msg << '\n';
            // 2021-07-22 19:46 DEBUG string_logger.cpp(15:55) @"struct LoggerMsgHelper()": Hello log!
            // $Date $Time $Level $SourceFile($Line:$Column) @"$FunctionPrototype": $Message
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
