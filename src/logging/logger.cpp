#include "logger.hpp"


namespace logging {

    Logger::Logger(const std::string& path) : buffer(std::ios::ate), file(path, std::ios::out) {
        if (!file.is_open()) {
            throw std::ios::failure("!file.is_open()");
        }
    }

    void Logger::log(SeverityLevel::Level level, const std::string& msg) {
        std::lock_guard lock(log_mutex);
        // TODO: build proper logging message
        buffer << "%TODO:%" << level << msg << '\n';
    }

}