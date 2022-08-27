#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "../../sources/logging/logger.hpp"


TEST_CASE("StringLogger", "[libs][logging][logger][ZeroLogger]")
{
    using hlibs::logging::Level;
    using hlibs::logging::Message;
    using hlibs::logging::ZeroLogger;


    SECTION("is_standard_layout → true", "[type_traits]") {
        REQUIRE(std::is_standard_layout_v<ZeroLogger>);
    }

    SECTION("is_default_constructible → true", "[type_traits]") {
        REQUIRE(std::is_default_constructible_v<ZeroLogger>);
    }

    SECTION("log 1 message → pushed back on list", "[basic_check]") {
        ZeroLogger zl{};
        zl.info("one");
        REQUIRE_THAT(std::get<1>(zl.metadata()[0]), Catch::Matchers::Equals("1"));
    }

    SECTION("log many messages → pushed back on list", "[basic_check]") {
        ZeroLogger zl{};
        zl.info("one");
        zl.warning("two");
        zl.fatal("three");
        zl.debug("four");

        try {
            throw std::domain_error("five");
        }
        catch (const std::domain_error& e) {
            zl.exception("six", e);
        }

        REQUIRE_THAT(std::get<1>(zl.metadata()[0]), Catch::Matchers::Equals("5"));
    }

    // functional_requirements
    // special_functions

}
