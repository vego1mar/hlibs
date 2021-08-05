#include <catch2/catch_test_macros.hpp>

#include "../../src/logging/stdout_logger.hpp"
#include "../../src/io/file/stream_to_file.hpp"
#include "../../src/io/file/file_info.hpp"


TEST_CASE("StdOutLogger", "[libs][logging]")
{
    using libs::logging::Logger;
    using libs::logging::StringLogger;
    using libs::logging::StdOutLogger;
    using libs::logging::LoggerSettings;
    using libs::types::ObjectCounter;
    using libs::io::file::StreamToFile;
    using libs::io::file::GetFileSize;

    struct Before {
        using LoggerCounter = ObjectCounter<Logger>;

        static void CleanStaticStorage()
        {
            LoggerCounter::created = 0;
            LoggerCounter::alive = 0;
        }
    };

    SECTION("~dtor() automatic/dynamic -> OK", "[functional_requirements]") {
        LoggerSettings settings{};
        settings.use_stdout = false;
        StdOutLogger logger1;
        logger1.set(settings);

        constexpr auto disposeAutomatic = [](const LoggerSettings& settings) {
            StdOutLogger logger2(false);
            logger2.set(settings);
        };

        constexpr auto disposeDynamic = [](const LoggerSettings& settings) {
            auto logger3 = std::make_unique<StdOutLogger>();
            logger3->set(settings);
            logger3->debug("info");
        };

        REQUIRE_NOTHROW(logger1);
        REQUIRE_NOTHROW(disposeAutomatic(settings));
        REQUIRE_NOTHROW(disposeDynamic(settings));
    }

    SECTION("log 3+1 messages + dtor -> const log size", "[functional_requirements]") {
        const std::string stdOutFile("../../outputs/test_StdOutLogger__stdout.txt");
        const std::string clogFile("../../outputs/test_StdOutLogger__clog.txt");

        const auto logIntoFiles = [](const std::string& stdOutPath, const std::string& clogPath) {
            StreamToFile stdOut(std::cout, stdOutPath);
            StreamToFile stdLog(std::clog, clogPath);
            StdOutLogger logger{};
            logger.debug("debug1");
            logger.info("info2");
            logger.fatal("fatal3");
            logger.warning("warning4");
        };

        REQUIRE_NOTHROW(logIntoFiles(stdOutFile, clogFile));

        const auto expectedStdOutFileSize = 250UL;
        const auto expectedClogFileSize = 77UL;
        const auto stdOutFileSize = GetFileSize(stdOutFile);
        const auto clogFileSize = GetFileSize(clogFile);
        REQUIRE(stdOutFileSize == expectedStdOutFileSize);
        REQUIRE(clogFileSize == expectedClogFileSize);
    }

    SECTION("GetID() -> distinct per object", "[functional_requirements]") {
        Before::CleanStaticStorage();
        LoggerSettings settings{};
        settings.use_stdout = false;

        StdOutLogger logger1(false);
        StringLogger logger2{};
        logger1.set(settings);

        REQUIRE(logger1.getID() == 1UL);
        REQUIRE(logger2.getID() == 2UL);
    }

    SECTION("flush at ~dtor() -> OK", "[functional_requirements]") {
        const std::string stdOutPath("../../outputs/test_dtorStdOutLogger__stdout.txt");

        constexpr auto logMessage = [](const std::string& stdOutFile){
            LoggerSettings settings{};
            settings.use_stdout = true;
            settings.is_enabled = true;
            settings.messages_before_flush = 100;

            StreamToFile stdOut(std::cout, stdOutFile);
            StdOutLogger logger;
            logger.set(settings);
            logger.debug("The only message that should go to buffer.");
        };

        REQUIRE_NOTHROW(logMessage(stdOutPath));
        const auto logSize = GetFileSize(stdOutPath);
        REQUIRE(logSize == 130L);
    }

}
