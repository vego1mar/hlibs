#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <iostream>
#include <filesystem>
#include <cassert>

#include "../../sources/io/helper_objects.hpp"


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
            FileLoader loader{fp};
            loader.read();
            assert(loader.data().size() == size);
            return loader.toString();
        };

        auto strId = loadFile(std::filesystem::path(path), expected);
        REQUIRE_THAT(strId, Catch::Matchers::ContainsSubstring(std::to_string(expected)));
        REQUIRE_THAT(strId, Catch::Matchers::ContainsSubstring(path));
    }

}

TEST_CASE("FileReader", "[libs][io][FileReader]")
{
    using hlibs::io::FileReader;
    using hlibs::io::GetFileSize;

    SECTION("is_standard_layout → true", "[type_traits]") {
        REQUIRE(!std::is_standard_layout_v<FileReader>);
    }

    SECTION("is_default_constructible → true", "[type_traits]") {
        REQUIRE(!std::is_default_constructible_v<FileReader>);
    }

    SECTION("construct & destruct → no exception", "[basic_check]") {
        const std::string path("../../inputs/file-reader-1-read.txt");

        constexpr auto openAndClose = [](const std::filesystem::path& p) {
            FileReader reader(p);
        };

        REQUIRE_NOTHROW(openAndClose(path));
    }

    SECTION("read next lines until eof → expected file size", "[functional_requirements][basic_check]") {
        const std::string path("../../inputs/file-reader-1-read.txt");
        const std::size_t expected = 802UL;
        std::vector<std::string> buffer{};

        auto readTillEnd = [&buffer](const std::filesystem::path& p) {
            FileReader reader(p);

            while (reader.hasNextLine()) {
                buffer.emplace_back(reader.getNextLine());
            }

            return reader.toString();
        };

        auto repr = readTillEnd(path);
        auto size = hlibs::io::GetFileSize(path);
        REQUIRE(size == expected);
        REQUIRE_THAT(repr, Catch::Matchers::ContainsSubstring(path));
        REQUIRE_THAT(repr, Catch::Matchers::EndsWith("\"14\"}"));
    }

    SECTION("read whole file → all lines read", "[basic_check]") {
        const std::string path("../../inputs/file-reader-2-read.txt");
        const std::size_t expected = 392UL;

        auto readFile = [](const std::filesystem::path& p) {
            FileReader reader(p);

            while (reader.hasNextLine()) {
                reader.getNextLine();
            }

            return reader.lines();
        };

        auto linesRead = readFile(path);
        auto size = hlibs::io::GetFileSize(path);
        REQUIRE(size == expected);
        REQUIRE(linesRead == 21UL);
    }

}

TEST_CASE("FileWriter", "[libs][io][FileWriter]")
{
    using hlibs::io::FileWriter;
    using hlibs::io::GetFileSize;

    SECTION("is_standard_layout → true", "[type_traits]") {
        REQUIRE(!std::is_standard_layout_v<FileWriter>);
    }

    SECTION("is_default_constructible → true", "[type_traits]") {
        REQUIRE(!std::is_default_constructible_v<FileWriter>);
    }

    SECTION("create & no-op → no exception", "[basic_check]") {
        const std::string path("../../outputs/file-writer-1-write.txt");
        const std::size_t expected = 0UL;

        constexpr auto create = [](const std::filesystem::path& p) {
            FileWriter writer{p};
        };

        REQUIRE_NOTHROW(create(path));
        auto size = GetFileSize(path);
        REQUIRE(size == expected);
    }

    SECTION("append to file & read again → content appended", "[basic_check]") {
        const std::filesystem::path path("../../outputs/file-writer-2-write.txt");
        const std::size_t size = GetFileSize(path);

        constexpr auto append = [](const std::filesystem::path& p, char ch1, char ch2) {
            FileWriter writer{p, std::ios::app};
            writer.put(ch1);
            writer.put(ch2);
            return writer.bytes();
        };

        auto bytes = append(path, 'a', 'b');
        bytes += append(path, 'y', 'z');

        auto after = GetFileSize(path);
        std::size_t expected = size + bytes;
        REQUIRE(after == expected);
        REQUIRE(bytes == 4);
    }

    SECTION("wrong open mode → exception", "[exception]") {
        constexpr auto tryCreate = []() {
            const std::filesystem::path path("../../outputs/file-writer-2-write.txt");
            FileWriter writer(path, std::ios::in);
            writer.sync();
        };

        REQUIRE_THROWS(tryCreate());
    }

    SECTION("write & put unformatted → expected number of bytes written", "[functional_requirements]") {
        const std::filesystem::path path("../../outputs/file-writer-3-unformatted.txt");
        const std::size_t expected = 40UL;

        constexpr auto writeUnformatted = [](const std::filesystem::path& p) {
            FileWriter writer{p};
            writer.write("UGV3bm/Fm8SHIGpha8SFIGRhamUgd2lhcmE=");
            writer.put(' ');
            writer.put(static_cast<signed char>('s'));
            writer.put(static_cast<unsigned char>('u'));
            writer.put('\n');
            return writer.bytes();
        };

        auto written = writeUnformatted(path);
        REQUIRE(written == expected);
    }

    SECTION("write & put formatted → expected number of bytes written", "[functional_requirements]") {
        CHECK(false);
    }

    SECTION("write floating point numbers → expected number of bytes written", "[functional_requirements]") {
        CHECK(false);
    }

    SECTION("write booleans → expected number of bytes written", "[functional_requirements]") {
        CHECK(false);
    }

    SECTION("write, set & get position, then put → expected number of bytes written", "[functional_requirements]") {
        CHECK(false);
    }

    SECTION("write different data → expected string representation", "[basic_check]") {
        CHECK(false);
    }

}
