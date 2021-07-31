#ifndef LIBS_LOGGING_LOGGER_HPP
#define LIBS_LOGGING_LOGGER_HPP

#include <string>
#include <sstream>
#include <fstream>
#include <cinttypes>
#include <iosfwd>
#include <unordered_map>
#include <experimental/source_location>

#include "../types/types.hpp"


namespace libs::logging {

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
        /// Number of messages that will go to buffer before placing them in the target output.
        unsigned short messages_before_flush = 11;

        /// On false, prevents processing of logged messages.
        bool is_enabled = true;

        /// Used in StdOutLogger to prevent double output in ConsoleLogger.
        bool skip_print_to_stdout = false;
    };


    class Logger : private libs::types::ObjectCounter<Logger> {
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

        void set(LoggerSettings&& newSettings) noexcept
        {
            settings = std::move(newSettings);
        }

        [[nodiscard]] inline const unsigned int& getID() const noexcept
        {
            return id;
        }

      protected:
        Logger()
        {
            id = libs::types::ObjectCounter<Logger>::created;
        }

        ~Logger() noexcept override = default;

        virtual void log(SeverityLevel::Level level, const std::string& msg, std::experimental::source_location source) = 0;

        inline LoggerSettings& elicitSettings() noexcept
        {
            return settings;
        }

      private:
        LoggerSettings settings{};
        unsigned int id = 0;
    };

}

#endif //LIBS_LOGGING_LOGGER_HPP
