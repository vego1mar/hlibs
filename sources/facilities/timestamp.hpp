#ifndef LIBS_TIMESTAMP_HPP
#define LIBS_TIMESTAMP_HPP

#include <string>
#include <ctime>
#include <array>
#include <chrono>
#include <iomanip>
#include <locale>


namespace hlibs::facilities::timestamp {

    struct DateTime {
        /// url:https://en.cppreference.com/w/cpp/locale/locale
        explicit DateTime(const std::locale& locale = std::locale::classic())
        {
            auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::tm current = *std::localtime(&now);

            std::ostringstream ss;
            ss.imbue(std::locale(locale));
            ss << std::put_time(&current, "%Y-%m-%d %H:%M:%S");
            auto str = ss.str();

            auto delimiter = str.find_first_of(' ');
            date = str.substr(0, delimiter);
            time = str.substr(delimiter + 1);
        }

        /// "$Date $Time"
        [[nodiscard]] std::string timestamp() const noexcept
        {
            return date + ' ' + time;
        }

        std::string date;
        std::string time;
    };

}

#endif //LIBS_TIMESTAMP_HPP
