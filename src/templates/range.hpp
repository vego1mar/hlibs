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

        Range(T a, T b) : left(a < b ? a : b), right(a < b ? b : a)
        {
        }

        template<typename R>
        explicit Range(const Range<R>& rhs) : left(static_cast<T>(rhs.left)), right(static_cast<T>(rhs.right))
        {
        }

        [[nodiscard]] inline unsigned getLength() const noexcept
        {
            return right - left;
        }

        [[nodiscard]] inline std::string toString() const noexcept
        {
            return '(' + std::to_string(left) + ',' + std::to_string(right) + ')';
        }

        inline bool isIn(const T& value) const noexcept
        {
            return value >= left && value <= right;
        }

    };

    // TODO: non-contiguous Range

}

#endif //RANGE_HPP
