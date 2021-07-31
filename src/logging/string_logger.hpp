#ifndef STRING_LOGGER_HPP
#define STRING_LOGGER_HPP

#include <sstream>
#include <iostream>

#include "logger.hpp"
#include "../date_time/date_time.hpp"
#include "../strings/strings.hpp"


namespace logging {

    class StringLogger : public Logger {
      public:
        StringLogger() = delete;

        explicit StringLogger(std::string& strTarget) : target(strTarget)
        {
        }

        StringLogger(const StringLogger& rhs) = delete;

        StringLogger(StringLogger&& rhs) noexcept = delete;

        StringLogger& operator=(const StringLogger& rhs) = delete;

        StringLogger& operator=(StringLogger&& rhs) noexcept = delete;

        ~StringLogger() noexcept override
        {
            if (!elicitSettings().is_enabled) {
                return;
            }

            flushed_messages += buffered_messages;
            str_buffer << "~StringLogger(" << flushed_messages << ")\n";
            target.append(str_buffer.str());
        }

        void exception(const std::string& msg, const std::exception& e, SourceLocation source = SourceLocation::current())
        {
            if (!elicitSettings().is_enabled) {
                return;
            }

            // ${Header}std::exception | $Message | $What$NewLine
            const auto header = GetMessageHeader(SeverityLevel::Level::Debug, source);
            const std::string message = "std::exception | " + msg + " | " + e.what();
            str_buffer << header << message << '\n';
            ++buffered_messages;
            onBufferedMessage();
            checkFlush();
        }

      protected:
        void log(SeverityLevel::Level level, const std::string& msg, SourceLocation source) override
        {
            if (!elicitSettings().is_enabled) {
                return;
            }

            // ${Header}$Message$NewLine
            const auto header = GetMessageHeader(level, source);
            str_buffer << header << msg << '\n';
            ++buffered_messages;
            onBufferedMessage();
            checkFlush();
        }

        /// An event for flushing logged messages in derived classes before clearing the buffer.
        virtual void onFlush()
        {
            // It will be invoked before flush() that will clear str_buffer.
        }

        /// An event indicating that another message has been logged into buffer.
        virtual void onBufferedMessage()
        {
            // Used after message addition to str_buffer with log() or exception() but before checkFlush().
        }

        /// An event to use only in derived destructors to force flush from buffer to str_target.
        void onFlushInDerivedDestructor()
        {
            flush();
        }

        inline const std::ostringstream& elicitStringBuffer() const noexcept
        {
            return str_buffer;
        }

        inline const unsigned long& elicitFlushedMessages() const noexcept
        {
            return flushed_messages;
        }

      private:
        std::string& target;
        std::ostringstream str_buffer{};
        unsigned short buffered_messages = 0;
        unsigned long flushed_messages = 0;


        static std::string GetMessageHeader(const SeverityLevel::Level& level, const SourceLocation& source)
        {
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

        void flush()
        {
            target.append(str_buffer.str());
            str_buffer.str("");
            str_buffer.clear();
            flushed_messages += buffered_messages;
            buffered_messages = 0;
        }

        void checkFlush()
        {
            if (buffered_messages >= elicitSettings().messages_before_flush) {
                onFlush();
                flush();
            }
        }

    };

}

#endif //STRING_LOGGER_HPP
