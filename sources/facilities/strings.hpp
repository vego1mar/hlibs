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

}

#endif //LIBS_STRING_HPP
