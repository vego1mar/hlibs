#include <iostream>

#include "../catch.hpp"
#include "../../src/file/stream_to_file.hpp"
#include "../../src/file/file_info.hpp"


TEST_CASE("StreamToFile", "[file]") {

    SECTION("StreamToFile(path) -> read(path) OK", "[functional_requirements]") {
        const std::string stdoutFile("../../outputs/stdout.txt");
        const std::string messageToDump("StreamToFile(stdout)");

        constexpr auto redirectAndRelease = [](const std::string& path, const std::string& msg){
            file::StreamToFile stdoutWrapper(std::cout, path);
            std::cout << msg << std::endl;
        };

        redirectAndRelease(stdoutFile, messageToDump);
        std::cout << "~StreamToFile(stdout)" << std::endl;
        const auto fileSize = file::info::GetFileSize(stdoutFile);
        const auto expectedFileSize = messageToDump.size() + 1;
        REQUIRE(fileSize == expectedFileSize);
    }

}
