#ifndef LIBS_STRING_HPP
#define LIBS_STRING_HPP

#include <string>
#include <string_view>
#include <ranges>
#include <algorithm>
#include <regex>


namespace hlibs::facilities::strings {

    [[maybe_unused]] static std::string ToUpperCase(std::string_view source)
    {
        std::string str{source};
        std::ranges::transform(str, str.begin(), ::toupper);
        return str;
    }

    [[maybe_unused]] static bool Contains(std::string_view source, char what) noexcept
    {
        return source.find(what) != std::string::npos;
    }

    [[maybe_unused]] static bool Contains(std::string_view source, std::string_view what) noexcept
    {
        return source.find(what) != std::string::npos;
    }

    /// Checks if str consists only of digits.
    [[maybe_unused]] static bool IsNumber(std::string_view str)
    {
        bool hasOnlyDigits = true;

        auto isNumber = [&hasOnlyDigits](const auto& ch) {
            hasOnlyDigits = hasOnlyDigits && std::isdigit(ch);
        };

        std::ranges::for_each(str, isNumber);
        return hasOnlyDigits;
    }

    /// Checks if str corresponds to a number notation (e.g. +1.0, -1).
    [[maybe_unused]] static bool IsNumeric(const std::string& str)
    {
        std::regex number("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?");
        return std::regex_match(str, number);
    }

}

#endif //LIBS_STRING_HPP
