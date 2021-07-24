#include "../catch.hpp"
#include "../../src/logging/stdout_logger.hpp"
#include "../../src/file/stream_to_file.hpp"
#include "../../src/file/file_info.hpp"


TEST_CASE("StdOutLogger", "[logging]") {

    SECTION("log 4 messages -> const log size", "[functional_requirements]") {
        const std::string stdOutFile("../../outputs/test_StdOutLogger__stdout.txt");
        const std::string clogFile("../../outputs/test_StdOutLogger__clog.txt");

        constexpr auto logIntoFiles = [](const std::string& stdOutPath, const std::string& clogPath){
            file::StreamToFile stdOut(std::cout, stdOutPath);
            file::StreamToFile stdLog(std::clog, clogPath);
            logging::StdOutLogger logger{true, 5 };
            logger.debug("debug1");
            logger.info("info2");
            logger.fatal("fatal3");
            logger.warning("warning4");
        };

        logIntoFiles(stdOutFile, clogFile);

        const auto expectedStdOutFileSize = 250UL;
        const auto expectedClogFileSize = 77UL;
        const auto stdOutFileSize = file::info::GetFileSize(stdOutFile);
        const auto clogFileSize = file::info::GetFileSize(clogFile);
        REQUIRE(stdOutFileSize == expectedStdOutFileSize);
        REQUIRE(clogFileSize == expectedClogFileSize);
    }

    SECTION("GetID() -> distinct per object", "[functional_requirements]") {
        std::string output{};
        logging::StdOutLogger logger1(false);
        logging::StringLogger logger2(output);
        //REQUIRE(logger1.GetID() == 1UL);
        //REQUIRE(logger2.GetID() == 2UL);
    }

}
