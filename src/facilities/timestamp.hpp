#ifndef LIBS_TIMESTAMP_HPP
#define LIBS_TIMESTAMP_HPP

#include <string>
#include <ctime>
#include <array>


namespace libs::facilities::timestamp {

    static std::string GetDateAndTime()
    {
        time_t now = std::time(nullptr);
        struct tm timeStruct{};
        std::array<char, 40> buffer{};
        timeStruct = *localtime_r(&now, &timeStruct);
        strftime(buffer.begin(), buffer.size(), "%Y-%m-%d %X", &timeStruct);
        std::string result(std::begin(buffer));
        return result;
    }

    static std::string GetDate()
    {
        const auto dateTime = GetDateAndTime();
        const auto delimiterPos = dateTime.find(' ');
        auto date = dateTime.substr(0, delimiterPos);
        return date;
    }

    static std::string GetTime()
    {
        const auto dateTime = GetDateAndTime();
        auto time = dateTime.substr(dateTime.find(' ') + 1);
        return time;
    }

}

#endif //LIBS_TIMESTAMP_HPP
