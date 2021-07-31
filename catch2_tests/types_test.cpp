#include <cinttypes>

#include "../externs/catch.hpp"
#include "../src/types.hpp"


TEST_CASE("RGBColor", "[libs][types]")
{
    using libs::types::RGBColor;
    using libs::types::ColorName;


    SECTION("ctor(name) -> values OK", "[functional_requirement]") {
        RGBColor teal(ColorName::Teal);

        REQUIRE(teal.red == 0);
        REQUIRE(teal.green == 128);
        REQUIRE(teal.blue == 128);
    }

    SECTION("ctor(r,g,b) -> types OK", "[functional_requirement]") {
        RGBColor rgb(1, 23, 255);

        bool isRedConst = typeid(rgb.red) == typeid(const uint8_t);
        bool isGreenConst = typeid(rgb.green) == typeid(const uint8_t);
        bool isBlueConst = typeid(rgb.blue) == typeid(const uint8_t);
        bool areFieldsConst = isRedConst && isGreenConst && isBlueConst;
        REQUIRE(areFieldsConst);
    }

}
