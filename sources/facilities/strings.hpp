#ifndef LIBS_STRING_HPP
#define LIBS_STRING_HPP

#include <string>
#include <string_view>
#include <ranges>
#include <algorithm>


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

    // TODO: IsNumber

}

#endif //LIBS_STRING_HPP
