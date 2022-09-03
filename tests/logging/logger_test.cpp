#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "../../sources/logging/logger.hpp"


TEST_CASE("InMemoryLogger", "[libs][logging][InMemoryLogger]")
{
    using hlibs::logging::InMemoryLogger;

    SECTION("is_standard_layout → true", "[type_traits]") {
        REQUIRE(std::is_standard_layout_v<InMemoryLogger>);
    }

    SECTION("is_default_constructible → true", "[type_traits]") {
        REQUIRE(std::is_default_constructible_v<InMemoryLogger>);
    }

    SECTION("log 1 str → pushed back on list", "[basic_check]") {
        InMemoryLogger logger{};

        logger.info("one");

        REQUIRE(logger.messages.size() == 1);
        REQUIRE_THAT(logger.toString(), Catch::Matchers::Equals("{\"1\"}"));
    }

    SECTION("log many messages → pushed back on list", "[basic_check]") {
        InMemoryLogger logger{};

        logger.info("one");
        logger.warning("two");
        logger.fatal("three");
        logger.debug("four");
        logger.exception("six", std::domain_error("five"));

        REQUIRE(logger.messages.size() == 5);
        REQUIRE_THAT(logger.toString(), Catch::Matchers::Equals("{\"5\"}"));
    }

    SECTION("log many messages → expected str representation", "[basic_check]") {
        constexpr auto log = []() {
            constexpr std::size_t nMessages = 100;
            InMemoryLogger logger;

            for (auto i = 0; i != nMessages; ++i) {
                logger.info("str-" + std::to_string(i));
            }

            return logger.toString();
        };

        auto repr = log();
        REQUIRE_THAT(repr, Catch::Matchers::Equals("{\"100\"}"));
    }

}

TEST_CASE("StdoutLogger", "[libs][logging][StdoutLogger]")
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

    SECTION("log 1 str to stdout → no exception", "[basic_check]") {
        StdoutLogger logger;
        REQUIRE_NOTHROW(logger.info("stdout"));
    }

    SECTION("log 1 str to stderr → no exception", "[basic_check]") {
        StdoutLogger logger;
        REQUIRE_NOTHROW(logger.fatal("stderr"));
    }

    SECTION("log 1 str to clog → no exception", "[basic_check]") {
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
                logger.fatal("str-" + std::to_string(i));
            }

            return logger.toString();
        };

        auto repr = logToFile();
        REQUIRE_THAT(repr, Catch::Matchers::Equals("{{\"111\"}}"));
    }

}

TEST_CASE("TerminalLogger", "[libs][logging][TerminalLogger]")
{
    using hlibs::logging::TerminalLogger;
    using hlibs::io::StreamToFile;

    SECTION("is_standard_layout → true", "[type_traits]") {
        REQUIRE(std::is_standard_layout_v<TerminalLogger>);
    }

    SECTION("is_default_constructible → true", "[type_traits]") {
        REQUIRE(std::is_default_constructible_v<TerminalLogger>);
    }

    SECTION("log warning str → no exception", "[basic_check]") {
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
            for (auto i = 0; i != nLogs; ++i) logger.exception("str-" + std::to_string(i), e);

            return logger.toString();
        };

        auto repr = logToFile();
        REQUIRE_THAT(repr, Catch::Matchers::Equals("{{\"67\"}}"));
    }

}

TEST_CASE("FileLogger", "[libs][logging][FileLogger]")
{

    using hlibs::logging::FileLogger;

    struct FileName {
        static std::string FromToString(std::string_view str)
        {
            // {{"1"},{"../../outputs/file_logger_20220903_173759.log","0x7fffffffcd50","86"}}
            auto first = str.find_first_of(',') + 3;
            auto last = str.rfind(".log") + 4;
            auto distance = last - first;
            auto result = str.substr(first, distance);
            return std::string(result);
        }
    };

    SECTION("is_standard_layout → false", "[type_traits]") {
        REQUIRE(!std::is_standard_layout_v<FileLogger>);
    }

    SECTION("is_default_constructible → false", "[type_traits]") {
        REQUIRE(!std::is_default_constructible_v<FileLogger>);
    }

    SECTION("construct with a file name → exception", "[basic_check][exception]") {
        const std::filesystem::path path("../../outputs/file-logger-1-empty.txt");

        constexpr auto construct = [](const std::filesystem::path& p) {
            FileLogger logger{p};
        };

        REQUIRE_THROWS_AS(construct(path), std::invalid_argument);
    }

    SECTION("construct with a folder name → no exception", "[basic_check][exception]") {
        const std::filesystem::path path("../../outputs/");

        constexpr auto construct = [](const std::filesystem::path& p) {
            FileLogger logger{p};
        };

        REQUIRE_NOTHROW(construct(path));
    }

    SECTION("log 1 info message → expected file size", "[functional_requirements]") {
        const std::filesystem::path path("../../outputs/");
        const std::size_t expected = 86UL;

        constexpr auto log = [](const std::filesystem::path& p) {
            FileLogger logger{p};
            logger.info("first-and-last-message");
            return logger.toString();
        };

        auto repr = log(path);

        REQUIRE(std::filesystem::file_size(FileName::FromToString(repr)) == expected);
        REQUIRE_THAT(repr, Catch::Matchers::ContainsSubstring("{\"1\"}"));
        REQUIRE_THAT(repr, Catch::Matchers::ContainsSubstring(path.string()));
    }

    SECTION("log many different messages → expected file size", "[functional_requirements]") {
        const std::filesystem::path path("../../outputs/");
        const std::size_t expected = 493UL;

        constexpr auto log = [](const std::filesystem::path& p) {
            FileLogger logger{p};
            logger.info("1\t...");
            logger.warning("2\t...");
            logger.fatal("3\t...");
            logger.debug("4\t...");
            logger.exception("5\t...", std::runtime_error("...6..."));
            return logger.toString();
        };

        auto repr = log(path);

        REQUIRE_THAT(repr, Catch::Matchers::ContainsSubstring(path.string()));
        REQUIRE_THAT(repr, Catch::Matchers::ContainsSubstring("{\"5\"}"));
//        REQUIRE_THAT(repr, Catch::Matchers::ContainsSubstring("{\"493\"}"));
        REQUIRE(std::filesystem::file_size(FileName::FromToString(repr)) == expected);
    }

}
