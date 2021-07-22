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
            Error = 2,
            Warning = 4,
            Info = 16,
            Debug = 32,
        };

        friend std::ostream& operator<<(std::ostream& lhs, const Level& rhs) noexcept {
            const std::unordered_map<Level, std::string> mapping = {
                    {Level::Fatal,   "Fatal"},
                    {Level::Error,   "Error"},
                    {Level::Warning, "Warning"},
                    {Level::Info,    "Info"},
                    {Level::Debug,   "Debug"},
            };

            lhs << mapping.at(rhs);
            return lhs;
        }

        static const Level& begin() {
            return *all_items.begin();
        }

        static const Level& next(int position) {
            return *(all_items.begin() + position);
        }

        static const Level& end() {
            return *all_items.end();
        }

    private:
        static constexpr std::array<Level, 5> all_items = {
                Level::Fatal,
                Level::Error,
                Level::Warning,
                Level::Info,
                Level::Debug
        };
    };


    class Logger {
    private:
        uint8_t buffered_messages = 0;
        std::ostringstream buffer;
        std::ofstream file;
        mutable std::mutex log_mutex;

    public:
        Logger() = delete;

        explicit Logger(const std::string& path);

        Logger(const Logger& rhs) = delete;

        Logger(Logger&& rhs) noexcept = delete;

        Logger& operator=(const Logger& rhs) = delete;

        Logger& operator=(Logger&& rhs) noexcept = delete;

        ~Logger() = default;

    private:
        void log(SeverityLevel::Level level, const std::string& msg);
    };

}


#endif //LOGGER_HPP
