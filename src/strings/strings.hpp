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

}

#endif //STRINGS_HPP
