#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <iostream>
#include <filesystem>
#include <cassert>

#include "../../sources/io/helper_objects.hpp"
#include "../../sources/io/free_functions.hpp"


TEST_CASE("StreamToFile", "[libs][io][StreamToFile]")
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

TEST_CASE("FileLoader", "[libs][io][FileLoader]")
{
    using hlibs::io::FileLoader;

    SECTION("is_standard_layout → true", "[type_traits]") {
        REQUIRE(!std::is_standard_layout_v<FileLoader>);
    }

    SECTION("is_default_constructible → true", "[type_traits]") {
        REQUIRE(!std::is_default_constructible_v<FileLoader>);
    }

    SECTION("non-existing file → exception thrown", "[exceptions]") {
        std::string what;
        std::error_code errorCode;

        try {
            FileLoader loader{"./this-file-should-not-exist"};
        }
        catch (const std::ios::failure& e) {
            what = e.what();
            errorCode = e.code();
        }

        REQUIRE_THAT(what, Catch::Matchers::StartsWith("!isOpened()"));
        REQUIRE_THAT(errorCode.message(), Catch::Matchers::Equals("iostream error"));
    }

    SECTION("read file A → data stored", "[basic_check]") {
        const std::string path("../../inputs/file-loader-1-read.txt");
        const std::size_t expected = 635UL;

        constexpr auto loadFile = [](std::string_view sv) {
            FileLoader loader(sv);
            loader.read();
            return loader.data().size();
        };

        auto size = loadFile(path);
        REQUIRE(size == expected);
    }

    SECTION("read file B → data stored, str_id valid", "[functional_requirements]") {
        const std::string path("../../inputs/file-loader-2-read.txt");
        const std::size_t expected = 633UL;

        constexpr auto loadFile = [](const std::filesystem::path& fp, std::size_t size) {
            FileLoader loader{fp.string()};
            loader.read();
            assert(loader.data().size() == size);
            return loader.toString();
        };

        auto strId = loadFile(std::filesystem::path(path), expected);
        REQUIRE_THAT(strId, Catch::Matchers::ContainsSubstring(std::to_string(expected)));
        REQUIRE_THAT(strId, Catch::Matchers::ContainsSubstring(path));
    }

}
