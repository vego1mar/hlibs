#ifndef LIBS_STRING_HPP
#define LIBS_STRING_HPP

#include <string>
#include <string_view>
#include <ranges>
#include <algorithm>

#include "../types/types.hpp"


namespace hlibs::facilities::strings {

    static std::string ToUpperCase(const std::string_view source)
    {
        std::string str{source};
        std::ranges::transform(str, str.begin(), ::toupper);
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
    static bool CheckRange(const std::string_view source, const types::Range <T>& range)
    {
        const auto isInRange = [&range](const char& ch) {
            return range.isIn(ch);
        };

        return std::ranges::all_of(source.begin(), source.end(), isInRange);
    }

}

#endif //LIBS_STRING_HPP
