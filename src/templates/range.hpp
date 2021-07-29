#ifndef RANGE_HPP
#define RANGE_HPP

#include <concepts>
#include <type_traits>
#include <string>


namespace templates {

    template<typename T> concept Numeric = std::is_integral_v<T> || std::is_floating_point_v<T>;


    template<typename T> requires Numeric<T>
    struct Range {
        const T left{};
        const T right{};

        Range(T a, T b) : left(a < b ? a : b), right(a < b ? b : a) {
        }

        [[nodiscard]] inline unsigned getLength() const noexcept {
            return right - left;
        }

        [[nodiscard]] inline std::string toString() const noexcept {
            return '(' + std::to_string(left) + ',' + std::to_string(right) + ')';
        }

    };

}

#endif //RANGE_HPP
