#include <memory>

#include "../catch.hpp"
#include "../../src/templates/object_counter.hpp"


TEST_CASE("ObjectCounter", "[templates]") {

    struct Implementer1 : templates::ObjectCounter<Implementer1> {
    };

    struct Implementer2 : templates::ObjectCounter<Implementer2> {
        char field;

        explicit Implementer2(char v) : field(v) {
        }
    };

    using A = Implementer1;
    using B = Implementer2;
    using CounterA = templates::ObjectCounter<Implementer1>;
    using CounterB = templates::ObjectCounter<Implementer2>;


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

        A a1;
        B b1('a');
        REQUIRE(CounterA::alive == CounterA::created);
        REQUIRE(CounterB::alive == CounterB::created);
        REQUIRE(CounterA::alive == 1U);
        REQUIRE(CounterB::alive == 1U);

        createAndDestroy();
        B b5('e');
        REQUIRE(CounterA::alive == 1U);
        REQUIRE(CounterB::alive == 2U);
        REQUIRE(CounterA::created == 6U);
        REQUIRE(CounterB::created == 5U);
    }

    SECTION("ctor(dynamic) -> OK", "[functional_requirements]") {
        std::unique_ptr<A> a1(new A());
        auto b1 = std::make_unique<B>('1');
        REQUIRE(CounterA::alive == 1U);
        REQUIRE(CounterB::alive == 1U);
        REQUIRE(CounterA::created != CounterA::alive);
        REQUIRE(CounterB::created != CounterB::alive);

    }

}
