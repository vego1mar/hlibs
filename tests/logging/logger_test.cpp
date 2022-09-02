#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <iostream>

#include "../../sources/logging/logger.hpp"
#include "../../sources/io/helper_objects.hpp"


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

    SECTION("log many messages → expected str representation", "[basic_check]") {
        constexpr auto log = []() {
            constexpr std::size_t nMessages = 100;
            InMemoryLogger logger;

            for (auto i = 0; i != nMessages; ++i) {
                logger.info("message-" + std::to_string(i));
            }

            return logger.toString();
        };

        auto repr = log();
        REQUIRE_THAT(repr, Catch::Matchers::Equals("{\"100\"}"));
    }

}

TEST_CASE("StdoutLogger", "[libs][logging][logger][StdoutLogger]")
{
    using hlibs::logging::StdoutLogger;
    using hlibs::io::StreamToFile;
    using hlibs::io::GetFileSize;

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
        const std::size_t expected = 650;

        constexpr auto log = [](const std::string& file) {
            const std::size_t nLogs = 10;
            StreamToFile stdout(std::cout, file);
            StdoutLogger logger;
            for (auto i = 0; i != nLogs; ++i) logger.info(std::to_string(i));
            return logger.toString();
        };

        auto repr = log(path);

        const auto size = GetFileSize(path);
        REQUIRE(size == expected);
        REQUIRE_THAT(repr, Catch::Matchers::ContainsSubstring("{\"10\"}"));
    }

    SECTION("log many messages → expected str representation", "[basic_check]") {
        constexpr auto logToFile = []() {
            const std::string path("../../outputs/stdout-logger-2-to-string.txt");
            constexpr std::size_t nMessages = 111;
            StreamToFile stderr(std::cerr, path);
            StdoutLogger logger;

            for (auto i = 0; i != nMessages; ++i) {
                logger.fatal("message-" + std::to_string(i));
            }

            return logger.toString();
        };

        auto repr = logToFile();
        REQUIRE_THAT(repr, Catch::Matchers::Equals("{{\"111\"}}"));
    }

}

TEST_CASE("TerminalLogger", "[libs][logging][logger][TerminalLogger]")
{
    using hlibs::logging::TerminalLogger;
    using hlibs::io::StreamToFile;

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

    SECTION("log many messages → expected str representation", "[basic_check]") {
        constexpr auto logToFile = []() {
            const std::string path("../../outputs/stdout-logger-3-to-string.txt");
            constexpr std::size_t nLogs = 67;

            StreamToFile outLog(std::clog, path);
            TerminalLogger logger;
            std::out_of_range e("out of range exception");
            for (auto i = 0; i != nLogs; ++i) logger.exception("message-" + std::to_string(i), e);

            return logger.toString();
        };

        auto repr = logToFile();
        REQUIRE_THAT(repr, Catch::Matchers::Equals("{{\"67\"}}"));
    }

}
