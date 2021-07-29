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

    SECTION("[-2,0,2] -> Range(-2,0).isIn()", "[functional_requirements]") {
        Range<long> range(-2L, 0L);
        const auto length = range.getLength();

        bool isNegative2InRange = range.isIn(-2L);
        bool is0InRange = range.isIn(0);
        bool is2InRange = range.isIn(2L);

        CHECK(length == 2);
        REQUIRE(isNegative2InRange);
        REQUIRE(is0InRange);
        REQUIRE(!is2InRange);
    }

}
