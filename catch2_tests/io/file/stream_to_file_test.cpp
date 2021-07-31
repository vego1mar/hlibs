#include <iostream>

#include "../../../externs/catch.hpp"
#include "../../../src/io/file/stream_to_file.hpp"
#include "../../../src/io/file/file_info.hpp"


TEST_CASE("StreamToFile", "[libs][io][file]")
{
    using libs::io::file::StreamToFile;
    using libs::io::file::GetFileSize;


    SECTION("StreamToFile(path) -> read(path) OK", "[functional_requirements]") {
        const std::string stdoutFile("../../outputs/test_StreamToFile__stdout.txt");
        const std::string messageToDump("StreamToFile(stdout)");

        constexpr auto redirectAndRelease = [](const std::string& path, const std::string& msg) {
            StreamToFile stdoutWrapper(std::cout, path);
            std::cout << msg << std::endl;
        };

        redirectAndRelease(stdoutFile, messageToDump);
        std::cout << "~StreamToFile(stdout)" << std::endl;
        const auto fileSize = GetFileSize(stdoutFile);
        const auto expectedFileSize = messageToDump.size() + 1;
        REQUIRE(fileSize == expectedFileSize);
    }

}
