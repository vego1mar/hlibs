#ifndef STRING_LOGGER_HPP
#define STRING_LOGGER_HPP

#include <sstream>
#include <iostream>

#include "logger.hpp"
#include "../date_time/date_time.hpp"
#include "../strings/strings.hpp"


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

        void fatal(const std::string& msg, SourceLocation source = SourceLocation::current()) override {
            log(SeverityLevel::Level::Fatal, msg, source);
        }

        void warning(const std::string& msg, SourceLocation source = SourceLocation::current()) override {
            log(SeverityLevel::Level::Warning, msg, source);
        }

        void info(const std::string& msg, SourceLocation source = SourceLocation::current()) override {
            log(SeverityLevel::Level::Info, msg, source);
        }

        void debug(const std::string& msg, SourceLocation source = SourceLocation::current()) override {
            log(SeverityLevel::Level::Debug, msg, source);
        }

        void exception(const std::string& msg, const std::exception& e, SourceLocation source = SourceLocation::current()) override {
            if (!elicitSettings().is_enabled) {
                return;
            }

            // ${Header}std::exception | $Message | $What
            const auto header = GetMessageHeader(SeverityLevel::Level::Debug, source);
            const std::string message = "std::exception | " + msg + " | " + e.what();
            ++buffered_messages;
            checkFlush();
        }

    private:
        static std::string GetMessageHeader(const SeverityLevel::Level& level, const SourceLocation& source) {
            const auto timestamp = date_time::GetDateAndTime();
            const auto levelStr = strings::ToUpperCase(SeverityLevel::ToString(level));
            const std::string path = source.file_name();
            const auto sourceFile = path.substr(path.find_last_of("/\\") + 1);
            const auto line = source.line();
            const std::string prototype = source.function_name();

            // $Date $Time [$Level] $SourceFile($Line) @"$FunctionPrototype": ${Message}
            std::string result{};
            result.append(timestamp + " [" + levelStr + "] ");
            result.append(sourceFile + '(' + std::to_string(line) + ") @\"" + prototype + "\": ");
            return result;
        }

        void log(SeverityLevel::Level level, const std::string& msg, SourceLocation source) override {
            if (!elicitSettings().is_enabled) {
                return;
            }

            // ${Header}$Message$NewLine
            const auto header = GetMessageHeader(level, source);
            str_buffer << header << msg << '\n';
            ++buffered_messages;
            checkFlush();
        }

        virtual void flush() {
            target.append(str_buffer.str());
            str_buffer.str("");
            str_buffer.clear();
            buffered_messages = 0;
        }

        void checkFlush() {
            if (buffered_messages >= elicitSettings().messages_before_flush) {
                flush();
            }
        }
    };

}

#endif //STRING_LOGGER_HPP
