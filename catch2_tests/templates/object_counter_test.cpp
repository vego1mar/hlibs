#include <memory>

#include "../../externs/catch.hpp"
#include "../../src/templates/object_counter.hpp"


TEST_CASE("ObjectCounter", "[templates]")
{

    struct Implementer1 : templates::ObjectCounter<Implementer1> {
    };

    struct Implementer2 : templates::ObjectCounter<Implementer2> {
        char field;

        explicit Implementer2(char v) : field(v)
        {
        }
    };


    using A = Implementer1;
    using B = Implementer2;
    using CounterA = templates::ObjectCounter<Implementer1>;
    using CounterB = templates::ObjectCounter<Implementer2>;


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
