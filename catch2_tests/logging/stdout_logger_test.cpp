#include "../../externs/catch.hpp"
#include "../../src/logging/stdout_logger.hpp"
#include "../../src/io/file/stream_to_file.hpp"
#include "../../src/io/file/file_info.hpp"


TEST_CASE("StdOutLogger", "[libs][logging]")
{
    using libs::logging::Logger;
    using libs::logging::StringLogger;
    using libs::logging::StdOutLogger;
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


    SECTION("log 4 messages -> const log size", "[functional_requirements]") {
        Before::CleanStaticStorage();
        const std::string stdOutFile("../../outputs/test_StdOutLogger__stdout.txt");
        const std::string clogFile("../../outputs/test_StdOutLogger__clog.txt");

        constexpr auto logIntoFiles = [](const std::string& stdOutPath, const std::string& clogPath) {
            StreamToFile stdOut(std::cout, stdOutPath);
            StreamToFile stdLog(std::clog, clogPath);
            StdOutLogger logger{true, 5};
            logger.debug("debug1");
            logger.info("info2");
            logger.fatal("fatal3");
            logger.warning("warning4");
        };

        logIntoFiles(stdOutFile, clogFile);

        const auto expectedStdOutFileSize = 250UL;
        const auto expectedClogFileSize = 77UL;
        const auto stdOutFileSize = GetFileSize(stdOutFile);
        const auto clogFileSize = GetFileSize(clogFile);
        REQUIRE(stdOutFileSize == expectedStdOutFileSize);
        REQUIRE(clogFileSize == expectedClogFileSize);
    }

    SECTION("GetID() -> distinct per object", "[functional_requirements]") {
        Before::CleanStaticStorage();
        std::string output{};
        StdOutLogger logger1(false);
        StringLogger logger2(output);

        REQUIRE(logger1.getID() == 1UL);
        REQUIRE(logger2.getID() == 2UL);
    }

}
