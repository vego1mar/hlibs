#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <sstream>
#include <fstream>
#include <mutex>
#include <cinttypes>
#include <iosfwd>
#include <unordered_map>


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


    class Logger {
    public:
        static constexpr uint8_t MESSAGES_BEFORE_FLUSH = 11;

    private:
        uint8_t buffered_messages = 0;
        std::ostringstream buffer;
        std::ofstream file;

    public:
        Logger() = delete;

        Logger(const Logger& rhs) = delete;

        Logger(Logger&& rhs) noexcept = delete;

        Logger& operator=(const Logger& rhs) = delete;

        Logger& operator=(Logger&& rhs) noexcept = delete;

        virtual ~Logger() noexcept = default;

        virtual void info(const std::string& msg) = 0;

        virtual void exception(const std::string& msg, const std::exception& e) = 0;

    private:
        virtual void log(SeverityLevel::Level level, const std::string& msg) = 0;
    };

}


#endif //LOGGER_HPP
