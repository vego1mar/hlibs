#include <catch2/catch_test_macros.hpp>

#include "../../sources/logging/logger.hpp"


TEST_CASE("InMemoryLogger", "[libs][logging][logger][InMemoryLogger]")
{
    using hlibs::logging::InMemoryLogger;

    SECTION("is_standard_layout → true", "[type_traits]") {
        REQUIRE(std::is_standard_layout_v<InMemoryLogger>);
    }

    SECTION("is_default_constructible → true", "[type_traits]") {
        REQUIRE(std::is_default_constructible_v<InMemoryLogger>);
    }

    SECTION("log 1 message → pushed back on list", "[basic_check]") {
        InMemoryLogger logger{};
        logger.info("one");
        REQUIRE(logger.messages.size() == 1);
        REQUIRE(logger.current_msg == 1);
    }

    SECTION("log many messages → pushed back on list", "[basic_check]") {
        InMemoryLogger logger{};

        logger.info("one");
        logger.warning("two");
        logger.fatal("three");
        logger.debug("four");

        auto e = std::domain_error("five");
        logger.exception("six", e);

        REQUIRE(logger.messages.size() == 5);
        REQUIRE(logger.current_msg == 5);
    }

}

TEST_CASE("StdoutLogger", "[libs][logging][logger][StdoutLogger]")
{
    using hlibs::logging::StdoutLogger;

    SECTION("is_standard_layout → true", "[type_traits]") {
        REQUIRE(std::is_standard_layout_v<StdoutLogger>);
    }

    SECTION("is_default_constructible → true", "[type_traits]") {
        REQUIRE(std::is_default_constructible_v<StdoutLogger>);
    }

    SECTION("log 1 message to stdout → no exception", "[basic_check]") {
        StdoutLogger logger;
        REQUIRE_NOTHROW(logger.info("stdout"));
    }

    SECTION("log 1 message to stderr → no exception", "[basic_check]") {
        StdoutLogger logger;
        REQUIRE_NOTHROW(logger.fatal("stderr"));
    }

    SECTION("log 1 message to clog → no exception", "[basic_check]") {
        StdoutLogger logger;
        const std::domain_error e("domain_error");
        REQUIRE_NOTHROW(logger.exception("clog", e));
    }

    SECTION("log many messages → no exception", "[basic_check]") {
        StdoutLogger logger;
        logger.info("stdout-2");
        logger.fatal("stderr-2");
        logger.debug("stdout-2");
        const std::bad_exception e{};
        logger.exception("clog-2", e);
        logger.warning("stdout-2");
        REQUIRE_NOTHROW(logger);
    }

    // TODO: test StreamToFile
    // TODO: test by using StreamToFile

}
