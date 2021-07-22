#include "logger.hpp"


namespace logging {


    /*
    Logger::Logger(const std::string& path) : file(path, std::ios::out) {
        if (!file.is_open()) {
            throw std::ios::failure("!file.is_open()");
        }
    }

    void Logger::info(const std::string& msg) {
        log(SeverityLevel::Level::Info, msg);
    }

    void Logger::exception(const std::string& msg, const std::exception& e) {
        // TODO: build string
        buffer << msg << e.what() << '\n';
    }

    void Logger::log(SeverityLevel::Level level, const std::string& msg) {
        // TODO: specialize Logger by source (abstract base class)
        // TODO: build proper logging message
        buffer << "%TODO:%" << level << msg << '\n';
        ++buffered_messages;

        if (buffered_messages > MESSAGES_BEFORE_FLUSH) {
            // TODO: run function() = 0
        }
    }
    */

}
