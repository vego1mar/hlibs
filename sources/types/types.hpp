#ifndef LIBS_TYPES_HPP
#define LIBS_TYPES_HPP

#include <concepts>
#include <type_traits>
#include <string>
#include <cinttypes>
#include <unordered_map>
#include <array>


namespace hlibs::types {

    template<typename T>
    concept Character = std::is_same_v<T, char> || std::is_same_v<T, signed char> || std::is_same_v<T, unsigned char>;

    template<typename T>
    concept Numeric = std::is_integral_v<T> || std::is_floating_point_v<T>;


    template<typename T>
    struct ObjectCounter {
      public:
        static unsigned int created;
        static unsigned int alive;

        ObjectCounter()
        {
            increment();
        }

        ObjectCounter(const ObjectCounter&)
        {
            increment();
        }

        ObjectCounter& operator=(const ObjectCounter&) = delete;

        ObjectCounter(ObjectCounter&&) noexcept = delete;

        ObjectCounter& operator=(ObjectCounter&&) noexcept = delete;

      protected:
        virtual ~ObjectCounter() noexcept
        {
            --alive;
        }

        inline void increment() const
        {
            ++created;
            ++alive;
        }

    };

    template<typename T> unsigned int ObjectCounter<T>::alive(0);
    template<typename T> unsigned int ObjectCounter<T>::created(0);


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

    enum class ColorName : uint8_t {
        Black,
        Teal,
        Crimson,
        Lavender,
        Isabelline,
    };


    struct RGBColor {
      public:
        const uint8_t red = 0;
        const uint8_t green = 0;
        const uint8_t blue = 0;

        RGBColor(uint8_t r, uint8_t g, uint8_t b) : red(r), green(g), blue(b)
        {
        }

        explicit RGBColor(ColorName color = ColorName::Black) : red(Get(color)), green(Get(color, 1)), blue(Get(color, 2))
        {
        }

      private:
        static uint8_t Get(ColorName color, int shift = 0)
        {
            static const std::unordered_map<ColorName, std::array<uint8_t, 3>> colorCodes = {
                    {ColorName::Black,      {0,   0,   0}},
                    {ColorName::Teal,       {0,   128, 128}},
                    {ColorName::Crimson,    {220, 20,  60}},
                    {ColorName::Lavender,   {230, 230, 250}},
                    {ColorName::Isabelline, {244, 240, 236}},
            };

            const auto& codes = colorCodes.at(color);
            return *(codes.begin() + shift);
        }
    };

}

#endif //LIBS_TYPES_HPP
