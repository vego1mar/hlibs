#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <sstream>
#include <fstream>
#include <cinttypes>
#include <iosfwd>
#include <unordered_map>
#include <experimental/source_location>


namespace logging {

    struct SeverityLevel {
    public:
        enum class Level : uint8_t {
            Fatal = 1,
            Warning = 2,
            Info = 4,
            Debug = 8,
        };

        static std::string ToString(const Level& level) {
            const std::unordered_map<Level, std::string> mapping = {
                    {Level::Fatal,   "Fatal"},
                    {Level::Warning, "Warning"},
                    {Level::Info,    "Info"},
                    {Level::Debug,   "Debug"},
            };

            return mapping.at(level);
        }

        static const Level& Begin() noexcept {
            return *all_items.begin();
        }

        static const Level& Next(int position) {
            return *(all_items.begin() + position);
        }

        static const Level& End() noexcept {
            return *all_items.end();
        }

    private:
        static constexpr std::array<Level, 4> all_items = {
                Level::Fatal, Level::Warning, Level::Info, Level::Debug
        };
    };


    struct LoggerSettings {
        unsigned short messages_before_flush = 11;
        bool is_enabled = true;
    };


    using SourceLocation = std::experimental::source_location;


    class Logger {
    private:
        LoggerSettings settings{};

    public:
        Logger(const Logger& rhs) = delete;

        Logger(Logger&& rhs) noexcept = delete;

        Logger& operator=(const Logger& rhs) = delete;

        Logger& operator=(Logger&& rhs) noexcept = delete;

        void fatal(const std::string& msg, SourceLocation source = SourceLocation::current()) {
            log(SeverityLevel::Level::Fatal, msg, source);
        }

        void warning(const std::string& msg, SourceLocation source = SourceLocation::current()) {
            log(SeverityLevel::Level::Warning, msg, source);
        }

        void info(const std::string& msg, SourceLocation source = SourceLocation::current()) {
            log(SeverityLevel::Level::Info, msg, source);
        }

        void debug(const std::string& msg, SourceLocation source = SourceLocation::current()) {
            log(SeverityLevel::Level::Debug, msg, source);
        }

        void set(LoggerSettings&& newSettings) noexcept {
            settings = newSettings;
        }

    protected:
        Logger() = default;

        virtual ~Logger() noexcept = default;

        virtual void log(SeverityLevel::Level level, const std::string& msg, std::experimental::source_location source) = 0;

        inline LoggerSettings& elicitSettings() noexcept {
            return settings;
        }
    };

}

#endif //LOGGER_HPP
