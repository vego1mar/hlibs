#ifndef LIBS_TIMESTAMP_HPP
#define LIBS_TIMESTAMP_HPP

#include <string>
#include <ctime>
#include <array>
#include <chrono>
#include <iomanip>
#include <locale>
#include <ranges>


namespace hlibs::facilities::timestamp {

    /// Obtains current date and time.
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

        /// "YYYY-mm-dd HH:MM:SS"
        [[nodiscard]] std::string timestamp() const noexcept
        {
            return date + ' ' + time;
        }

        /// "YYYYmmDD_HHMMSS"
        [[nodiscard]] std::string filestamp() const noexcept
        {
            std::string ymd;
            std::string hm;
            std::ranges::remove_copy(date, std::back_inserter(ymd), '-');
            std::ranges::remove_copy(time, std::back_inserter(hm), ':');
            return ymd + '_' + hm;
        }

        std::string date;    ///< "YYYY-mm-dd"
        std::string time;    ///< "HH:MM:SS"
    };

}

#endif //LIBS_TIMESTAMP_HPP
