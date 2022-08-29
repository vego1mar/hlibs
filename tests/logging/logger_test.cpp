#include <catch2/catch_test_macros.hpp>

#include <iostream>

#include "../../sources/logging/logger.hpp"
#include "../../sources/io/file/stream_to_file.hpp"
#include "../../sources/io/file/file_info.hpp"


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
    using hlibs::io::file::StreamToFile;
    using hlibs::io::file::GetFileSize;

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

    SECTION("log many info messages → in stdout", "[functional_requirements]") {
        const std::string path("../../outputs/stdout-logger-1-info.txt");
        const std::size_t expected = 640;

        constexpr auto log = [](const std::string& file) {
            const std::size_t nLogs = 10;
            StreamToFile stdout(std::cout, file);
            StdoutLogger logger;
            for (auto i = 0; i < nLogs; ++i) logger.info(std::to_string(i));
        };

        log(path);

        const auto size = GetFileSize(path);
        REQUIRE(size == expected);
    }

}

TEST_CASE("TerminalLogger", "[libs][logging][logger][TerminalLogger]")
{
    using hlibs::logging::TerminalLogger;

    SECTION("log warning message → no exception", "[basic_check]") {
        TerminalLogger logger;
        REQUIRE_NOTHROW(logger.warning("test warning"));
    }

    SECTION("log many different messages → no exception", "[basic_check]") {
        TerminalLogger logger;
        logger.info("info-msg");
        logger.warning("warning-msg");
        logger.fatal("fatal-msg");
        logger.debug("debug-msg");

        std::runtime_error e("dummy");
        logger.exception("exception-msg", e);
    }

}
