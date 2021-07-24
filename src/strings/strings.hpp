#ifndef STRINGS_HPP
#define STRINGS_HPP

#include <string>
#include <string_view>
#include <algorithm>


namespace strings {

    static std::string ToUpperCase(const std::string_view source) {
        std::string str{source};
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);
        return str;
    }

    static bool Contains(const std::string_view source, const char what) {
        return source.find(what) != std::string::npos;
    }

    static bool Contains(const std::string_view source, const std::string_view what) {
        return source.find(what) != std::string::npos;
    }

    // TODO: ::string -> Split(), Contains(); check LM project

}

#endif //STRINGS_HPP
