#ifndef TYPES_HPP
#define TYPES_HPP

#include <cinttypes>
#include <unordered_map>
#include <array>


namespace libs::types {

    enum class ColorName : uint8_t {
        Black,
        Teal,
    };


    struct RGBColor {
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
                    {ColorName::Black, {0, 0,   0}},
                    {ColorName::Teal,  {0, 128, 128}},
            };

            const auto& codes = colorCodes.at(color);
            return *(codes.begin() + shift);
        }

        // TODO: provide named colors
        // TODO: convert from hex to rgb
        // TODO: RGBColor from/to hex, CMY, CMYK, 256 color set
    };

}

#endif //TYPES_HPP
