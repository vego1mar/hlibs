#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <sstream>
#include <mutex>


namespace log {

    class Logger {
    private:
        std::ostrinstream buffer;
        mutable std::mutex mutex;

    public:
        Logger() = delete;

        Logger(const Logger& rhs) = delete;

        Logger(Logger&& rhs) noexcept = delete;

        Logger& operator=(const Logger& rhs) = delete;

        Logger& operator=(Logger&& rhs) noexcept = delete;

        ~Logger() = default;
    };

}


#endif //LOGGER_HPP
