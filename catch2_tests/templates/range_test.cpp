#include "../catch.hpp"
#include "../../src/templates/range.hpp"


TEST_CASE("Range", "[templates]") {

    using templates::Range;


    SECTION("Range(5,-2) -> Range(-2,5)", "[functional_requirements]") {
        const Range<int> range{ 5, -2 };
        const auto length = range.getLength();
        const auto str = range.toString();

        bool isLeftLower = range.left < range.right;
        bool isLeftConst = std::is_const_v<decltype(range.left)>;
        bool isRightConst = std::is_const_v<decltype(range.right)>;
        bool isTheSameType = std::is_same_v<decltype(range.left), decltype(range.right)>;
        bool isLengthUnsigned = std::is_unsigned_v<decltype(range.getLength())>;

        REQUIRE(isLeftLower);
        CHECK(isLeftConst);
        CHECK(isRightConst);
        CHECK(isTheSameType);
        REQUIRE(length == 7);
        CHECK(isLengthUnsigned);
        REQUIRE_THAT(str, Catch::Equals("(-2,5)"));
    }

}
