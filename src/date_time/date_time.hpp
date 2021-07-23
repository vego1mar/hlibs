#ifndef DATE_TIME_HPP
#define DATE_TIME_HPP

#include <string>
#include <ctime>


namespace date_time {

    static std::string GetDateAndTime() {
        time_t now = std::time(nullptr);
        struct tm t_struct{};
        const int BUFFER_SIZE = 80;
        char buf[BUFFER_SIZE];
        t_struct = *localtime(&now);
        strftime(buf, sizeof(buf), "%Y-%m-%d %X", &t_struct);
        std::string result(buf);
        return result;
    }

    static std::string GetDate() {
        const auto dateTime = GetDateAndTime();
        const auto delimiterPos = dateTime.find(' ');
        auto date = dateTime.substr(0, delimiterPos);
        return date;
    }

    static std::string GetTime() {
        const auto dateTime = GetDateAndTime();
        auto time = dateTime.substr(dateTime.find(' ') + 1);
        return time;
    }

}

#endif //DATE_TIME_HPP
