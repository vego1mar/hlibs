#include <catch2/catch_test_macros.hpp>
#include <iostream>

#include "../../sources/io/helper_objects.hpp"
#include "../../sources/io/free_functions.hpp"


TEST_CASE("StreamToFile", "[libs][io][file][StreamToFile]")
{
    using hlibs::io::StreamToFile;
    using hlibs::io::GetFileSize;

    SECTION("redirect to clog → no exception", "[basic_check]") {
        const std::string file("../../outputs/stream-to-file-1-clog.txt");

        constexpr auto redirect = [](const std::string& path) {
            StreamToFile wrapper(std::clog, path);
        };

        REQUIRE_NOTHROW(redirect(file));
    }

    SECTION("redirect to stdout → file size as expected", "[functional_requirements]") {
        const std::string file("../../outputs/stream-to-file-2-stdout.txt");

        constexpr auto redirect = [](const std::string& path) {
            StreamToFile stdoutWrapper(std::cout, path);
            std::cout << "StreamToFile(stdout)" << '\n';             // 21
            std::cout << path << '\n';
            std::cout << "~StreamToFile(stdout)";                    // 21
            std::cout << std::endl;                                  //  1
        };

        redirect(file);

        const auto fileSize = GetFileSize(file);
        const auto expectedFileSize = 43 + file.size() + 1;
        REQUIRE(fileSize == expectedFileSize);
    }

    SECTION("redirect to many targets → the same sizes at no-op", "[memory]") {
        const std::string file1("../../outputs/stream-to-file-3-stderr.txt");
        const std::string file2("../../outputs/stream-to-file-3-clog.txt");
        const std::string file3("../../outputs/stream-to-file-3-stdout.txt");

        constexpr auto redirect = [](std::ostream& stream, const std::string& path) {
            StreamToFile wrapper(stream, path);
            return sizeof(wrapper);
        };

        const auto size1 = redirect(std::cerr, file1);
        const auto size2 = redirect(std::clog, file2);
        const auto size3 = redirect(std::cout, file3);
        REQUIRE(size1 == size2);
        REQUIRE(size1 == size3);
    }

}
