#include <memory>
#include <cinttypes>

#include "../../externs/catch.hpp"
#include "../../src/types/types.hpp"


TEST_CASE("ObjectCounter", "[templates]")
{
    using libs::types::ObjectCounter;

    struct Implementer1 : ObjectCounter<Implementer1> {
    };

    struct Implementer2 : ObjectCounter<Implementer2> {
        char field;

        explicit Implementer2(char v) : field(v)
        {
        }
    };

    using A = Implementer1;
    using B = Implementer2;
    using CounterA = ObjectCounter<Implementer1>;
    using CounterB = ObjectCounter<Implementer2>;

    struct Before {
        static void CleanStaticStorage()
        {
            CounterA::created = 0;
            CounterA::alive = 0;
            CounterB::created = 0;
            CounterB::alive = 0;
        }
    };


    SECTION("ctor(automatic) -> OK", "[functional_requirements]") {
        constexpr auto createAndDestroy = []() {
            A a2;
            B b2('b');
            A a3;
            B b3('c');
            B b4('d');
            A a4;
            A a5;
            A a6;
        };

        Before::CleanStaticStorage();
        A a1;
        B b1('a');
        REQUIRE(CounterA::created == 1U);
        REQUIRE(CounterA::alive == 1U);
        REQUIRE(CounterB::created == 1U);
        REQUIRE(CounterB::alive == 1U);

        createAndDestroy();
        B b5('e');
        REQUIRE(CounterA::alive == 1U);
        REQUIRE(CounterA::created == 6U);
        REQUIRE(CounterB::alive == 2U);
        REQUIRE(CounterB::created == 5U);
    }

    SECTION("ctor(dynamic) -> OK", "[functional_requirements]") {
        constexpr auto tryDynamicStorage = []() {
            std::unique_ptr<A> a1(new A());
            auto b1 = std::make_unique<B>('1');
        };

        Before::CleanStaticStorage();
        tryDynamicStorage();
        REQUIRE(CounterA::alive == 0U);
        REQUIRE(CounterA::created == 1U);
        REQUIRE(CounterB::alive == 0U);
        REQUIRE(CounterB::created == 1U);

        auto b2 = std::make_unique<B>('2');
        auto b3 = std::make_unique<B>('3');
        auto b4 = std::make_unique<B>('4');
        auto b5 = std::make_unique<B>('5');
        auto a2 = std::make_unique<A>();
        auto a3 = std::make_unique<A>();
        auto a4 = std::make_unique<A>();
        b3.reset();
        a3.reset();
        b4.reset();
        b2.reset();
        REQUIRE(CounterA::alive == 2U);
        REQUIRE(CounterA::created == 4U);
        REQUIRE(CounterB::alive == 1U);
        REQUIRE(CounterB::created == 5U);
    }

}


TEST_CASE("Range", "[templates]")
{
    using libs::types::Range;


    SECTION("Range(5,-2) -> Range(-2,5)", "[functional_requirements]") {
        const Range<int> range{5, -2};
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

