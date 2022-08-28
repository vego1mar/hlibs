#include <catch2/catch_test_macros.hpp>

#include "../../sources/logging/logger.hpp"


TEST_CASE("StringLogger", "[libs][logging][logger][InMemoryLogger]")
{
    using hlibs::logging::Level;
    using hlibs::logging::Message;
    using hlibs::logging::InMemoryLogger;


    SECTION("is_standard_layout → true", "[type_traits]") {
        REQUIRE(std::is_standard_layout_v<InMemoryLogger>);
    }

    SECTION("is_default_constructible → true", "[type_traits]") {
        REQUIRE(std::is_default_constructible_v<InMemoryLogger>);
    }

    SECTION("log 1 message → pushed back on list", "[basic_check]") {
        InMemoryLogger zl{};
        zl.info("one");
        REQUIRE(zl.messages.size() == 1);
    }

    SECTION("log many messages → pushed back on list", "[basic_check]") {
        InMemoryLogger zl{};

        zl.info("one");
        zl.warning("two");
        zl.fatal("three");
        zl.debug("four");

        auto e = std::domain_error("five");
        zl.exception("six", e);

        REQUIRE(zl.messages.size() == 5);
    }

}
