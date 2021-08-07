#ifndef LIBS_STRING_LOGGER_HPP
#define LIBS_STRING_LOGGER_HPP

#include <sstream>
#include <iostream>
#include <memory>

#include "logger.hpp"
#include "../facilities/timestamp.hpp"
#include "../facilities/strings.hpp"


namespace libs::logging {

    class StringLogger : public Logger {
      public:
        StringLogger() : str_target(nullptr)
        {
        }

        explicit StringLogger(std::shared_ptr<std::string>& externalTarget) : str_target(externalTarget)
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

            str_messages_flushed += str_messages_buffered;
            str_buffer << "~StringLogger(" << str_messages_flushed << ")\n";

            if (str_target) {
                str_target->append(str_buffer.str());
            }
        }

        /// Retrieves the unbuffered messages.
        friend std::ostream& operator<<(std::ostream& lhs, const StringLogger& rhs)
        {
            lhs << rhs.str_buffer.str();
            return lhs;
        }

        void exception(const std::string& msg, const std::exception& e, SourceLocation source = SourceLocation::current())
        {
            if (!elicitSettings().is_enabled) {
                return;
            }

            // ${Header}std::exception | $Message | $What$NewLine
            const auto header = GetMessageHeader(SeverityLevel::Level::Debug, source);
            const auto typeID = std::string(typeid(e).name());
            const std::string message = "std::exception->" + typeID + " | what: " + e.what() + " | " + msg;
            str_buffer << header << message << '\n';
            ++str_messages_buffered;
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
            ++str_messages_buffered;
            onBufferedMessage();
            checkFlush();
        }

        /// An event for flushing logged messages in derived classes before clearing the buffer.
        virtual void onFlush()
        {
            // It will be invoked before flush() that will clear str_buffer.
        }

        /// An event indicating that another message has been logged into str_buffer.
        virtual void onBufferedMessage()
        {
            // Used after message addition to str_buffer with log()/exception() but before checkFlush().
        }

        /// An event to use only in derived destructors to force flush from str_buffer to str_target.
        void onDestructor()
        {
            flush();
        }

        inline const std::ostringstream& elicitStringBuffer() const noexcept
        {
            return str_buffer;
        }

        inline const unsigned long& elicitFlushedMessages() const noexcept
        {
            return str_messages_flushed;
        }

      private:
        std::shared_ptr<std::string> str_target{};
        std::ostringstream str_buffer{};
        unsigned short str_messages_buffered = 0;
        unsigned long str_messages_flushed = 0;


        static std::string GetMessageHeader(const SeverityLevel::Level& level, const SourceLocation& source)
        {
            const auto timestamp = libs::facilities::timestamp::GetDateAndTime();
            const auto levelStr = libs::facilities::string::ToUpperCase(SeverityLevel::ToString(level));
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
            if (str_target) {
                str_target->append(str_buffer.str());
            }

            str_buffer.str("");
            str_buffer.clear();
            str_messages_flushed += str_messages_buffered;
            str_messages_buffered = 0;
        }

        void checkFlush()
        {
            if (str_messages_buffered >= elicitSettings().messages_before_flush) {
                onFlush();
                flush();
            }
        }

    };

}

#endif //LIBS_STRING_LOGGER_HPP
