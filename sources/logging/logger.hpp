#ifndef LIBS_LOGGER_HPP
#define LIBS_LOGGER_HPP

#include <string>
#include <sstream>
#include <fstream>
#include <cinttypes>
#include <iosfwd>
#include <unordered_map>
#include <experimental/source_location>
#include <string_view>
#include <vector>
#include <array>

#include "../types/types.hpp"
#include "../facilities/strings.hpp"
#include "../facilities/timestamp.hpp"


namespace hlibs::logging {

    struct SeverityLevel {
      public:
        enum class Level : uint8_t {
            Fatal = 1,
            Warning = 2,
            Info = 4,
            Debug = 8,
        };

        static std::string ToString(const Level& level)
        {
            const std::unordered_map<Level, std::string> mapping = {
                    {Level::Fatal,   "Fatal"},
                    {Level::Warning, "Warning"},
                    {Level::Info,    "Info"},
                    {Level::Debug,   "Debug"},
            };

            return mapping.at(level);
        }

        static const Level& Begin() noexcept
        {
            return *all_items.begin();
        }

        static const Level& Next(int position)
        {
            return *(all_items.begin() + position);
        }

        static const Level& End() noexcept
        {
            return *all_items.end();
        }

      private:
        static constexpr std::array<Level, 4> all_items = {
                Level::Fatal, Level::Warning, Level::Info, Level::Debug
        };
    };


    struct LoggerSettings {
        /// Number of messages that will go to buffer before placing them in the str_target output.
        unsigned short messages_before_flush = 11;

        /// On false, prevents processing of logged messages.
        bool is_enabled = true;

        /// Skips printing the messages, but do not prevent from processing them (StdOutLogger/ConsoleLogger).
        bool use_stdout = true;
    };


    class Logger : private hlibs::types::ObjectCounter<Logger> {
      public:
        using SourceLocation = std::experimental::source_location;


        Logger(const Logger& rhs) = delete;

        Logger(Logger&& rhs) noexcept = delete;

        Logger& operator=(const Logger& rhs) = delete;

        Logger& operator=(Logger&& rhs) noexcept = delete;

        void fatal(const std::string& msg, SourceLocation source = SourceLocation::current())
        {
            log(SeverityLevel::Level::Fatal, msg, source);
        }

        void warning(const std::string& msg, SourceLocation source = SourceLocation::current())
        {
            log(SeverityLevel::Level::Warning, msg, source);
        }

        void info(const std::string& msg, SourceLocation source = SourceLocation::current())
        {
            log(SeverityLevel::Level::Info, msg, source);
        }

        void debug(const std::string& msg, SourceLocation source = SourceLocation::current())
        {
            log(SeverityLevel::Level::Debug, msg, source);
        }

        void set(const LoggerSettings& newSettings) noexcept
        {
            settings = newSettings;
        }

        [[nodiscard]] inline const unsigned int& getID() const noexcept
        {
            return id;
        }

      protected:
        Logger()
        {
            id = hlibs::types::ObjectCounter<Logger>::created;
        }

        ~Logger() noexcept override = default;

        virtual void log(SeverityLevel::Level level, const std::string& msg, SourceLocation source) = 0;

        inline LoggerSettings& elicitSettings() noexcept
        {
            return settings;
        }

      private:
        LoggerSettings settings{};
        unsigned int id = 0;
    };


    // TODO: new approach

    struct Level final {
      public:
        enum class Severity : uint8_t {
            Exception = 0,
            Fatal = 1,
            Warning = 2,
            Info = 4,
            Debug = 8,
        };

        std::string toString(Severity level) const noexcept
        {
            return items.at(level);
        }

      private:
        std::unordered_map<Severity, std::string> items = {
                {Severity::Exception, "Exception"},
                {Severity::Fatal,     "Fatal"},
                {Severity::Warning,   "Warning"},
                {Severity::Info,      "Info"},
                {Severity::Debug,     "Debug"}
        };
    };


    struct Message final {
        using Source = std::experimental::source_location;

        std::string message;

        /// ${Header}$Message$NewLine
        Message(Level::Severity level, std::string_view msg, const Source& location)
        {
            auto head = Header(level, location);
            message.append(head);
            message.append(msg);
            message.append("\n");
        }

        /// ${Header}std::exception->$Type | $What | $Message$NewLine
        Message(std::string_view msg, const std::exception& e, const Source& location)
        {
            auto head = Header(Level::Severity::Exception, location);
            auto typeID = std::string(typeid(e).name());
            auto tail = "std::exception->" + typeID + " | what: " + e.what() + " | ";
            message.append(head);
            message.append(tail);
            message.append(msg);
            message.append("\n");
        }

      private:
        static std::string Header(Level::Severity lv, const Source& source)
        {
            static Level severity;
            std::string timestamp = hlibs::facilities::timestamp::GetDateAndTime();
            std::string level = hlibs::facilities::strings::ToUpperCase(severity.toString(lv));
            std::string path = source.file_name();
            std::string file = path.substr(path.find_last_of("/\\") + 1);
            std::string line = std::to_string(source.line());
            std::string function = source.function_name();

            // $Date $Time [$Level] $SourceFile($Line) @"$FunctionPrototype": ${Message}
            std::string header{};
            header.append(timestamp + " [" + level + "] ");
            header.append(file + '(' + line + ") @\"" + function + "\": ");
            return header;
        }
    };


    class ZeroLogger {
      public:
        using Source = std::experimental::source_location;

        void info(std::string_view msg, Source location = std::experimental::source_location::current())
        {
            messages.emplace_back(Level::Severity::Info, msg, location);
        }

        void warning(std::string_view msg, Source location = std::experimental::source_location::current())
        {
            messages.emplace_back(Level::Severity::Warning, msg, location);
        }

        void fatal(std::string_view msg, Source location = std::experimental::source_location::current())
        {
            messages.emplace_back(Level::Severity::Fatal, msg, location);
        }

        void debug(std::string_view msg, Source location = std::experimental::source_location::current())
        {
            messages.emplace_back(Level::Severity::Debug, msg, location);
        }

        void exception(std::string_view msg, const std::exception& e, Source location = std::experimental::source_location::current())
        {
            messages.emplace_back(msg, e, location);
        }

      private:
        std::vector<Message> messages;
    };

}

#endif //LIBS_LOGGER_HPP
