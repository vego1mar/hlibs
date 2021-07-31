#ifndef STRINGS_HPP
#define STRINGS_HPP

#include <string>
#include <string_view>
#include <ranges>
#include <algorithm>

#include "../templates/range.hpp"


namespace strings {

    static std::string ToUpperCase(const std::string_view source)
    {
        std::string str{source};
        std::ranges::transform(str.begin(), str.end(), str.begin(), ::toupper);
        return str;
    }

    static bool Contains(const std::string_view source, const char what)
    {
        return source.find(what) != std::string::npos;
    }

    static bool Contains(const std::string_view source, const std::string_view what)
    {
        return source.find(what) != std::string::npos;
    }

    template<typename T>
    static bool CheckRange(const std::string_view source, const templates::Range<T>& range)
    {
        const auto isInRange = [&range](const char& ch) {
            return range.isIn(ch);
        };

        return std::ranges::all_of(source.begin(), source.end(), isInRange);
    }

    // TODO: ::string -> Split(), Contains(); check LM project
    // RangeCheck(str, RangeCheckType::Letters | Digits | Alphanumerics | Special | ControlChars | ASCII)
    // OrderCheck(str, OrderCheckType::Lexicographical | ASCIbetical | Alphabetical_Unicode | Subsequent)

}

#endif //STRINGS_HPP
