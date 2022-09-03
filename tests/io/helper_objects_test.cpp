#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <iostream>
#include <filesystem>
#include <cassert>
#include <array>

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

    SECTION("is_standard_layout → false", "[type_traits]") {
        REQUIRE(!std::is_standard_layout_v<FileLoader>);
    }

    SECTION("is_default_constructible → false", "[type_traits]") {
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

    SECTION("is_standard_layout → false", "[type_traits]") {
        REQUIRE(!std::is_standard_layout_v<FileReader>);
    }

    SECTION("is_default_constructible → false", "[type_traits]") {
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
        const std::filesystem::path path("../../inputs/file-reader-1-read.txt");
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
        const std::filesystem::path path("../../inputs/file-reader-2-read.txt");
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
    using hlibs::io::FileLoader;

    SECTION("is_standard_layout → false", "[type_traits]") {
        REQUIRE(!std::is_standard_layout_v<FileWriter>);
    }

    SECTION("is_default_constructible → false", "[type_traits]") {
        REQUIRE(!std::is_default_constructible_v<FileWriter>);
    }

    SECTION("create & no-op → no exception", "[basic_check]") {
        const std::filesystem::path path("../../outputs/file-writer-1-write.txt");
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

    SECTION("write & put unformatted → expected bytes written", "[functional_requirements]") {
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

    SECTION("write formatted numbers → expected bytes written", "[functional_requirements]") {
        const std::filesystem::path path("../../outputs/file-writer-4-formatted.txt");
        const std::size_t expected = 128UL;

        constexpr auto writeFormatted = [](const std::filesystem::path& p) {
            constexpr auto newline = [](FileWriter& w) {
                w.newline(FileWriter::NewLine::Windows);
            };

            FileWriter writer{p};
            writer.boolean(false, 30);
            newline(writer);
            writer.integer(144, std::ios::dec, 30);
            newline(writer);
            writer.floating(739528.046, 6, std::ios::fixed, 30);
            newline(writer);
            writer.floating(7395.28046, 6, std::ios::fixed, 30);
            newline(writer);
            return writer.bytes();
        };

        auto written = writeFormatted(path);
        REQUIRE(written == expected);
    }

    SECTION("write integers → expected number of bytes written", "[functional_requirements]") {
        const std::filesystem::path path("../../outputs/file-writer-5-integers.txt");
        const std::size_t expected = 40UL;

        constexpr auto writeIntegers = [](const std::filesystem::path& p) {
            FileWriter writer{p};
            writer.integer(true, std::ios_base::boolalpha);
            writer.newline();
            writer.integer('\t', std::ios_base::unitbuf);
            writer.newline();
            writer.integer(static_cast<short>(15), std::ios_base::hex | std::ios::showbase);
            writer.newline();
            writer.integer(static_cast<int16_t>(128), std::ios_base::dec);
            writer.newline();
            writer.integer(111UL, std::ios::showpos | std::ios::oct);
            writer.newline();
            writer.integer(static_cast<long long>(132407896), std::ios::basefield);
            writer.newline();
            writer.integer(23, std::ios::dec, 10);
            writer.newline();
            return writer.bytes();
        };

        auto written = writeIntegers(path);
        REQUIRE(written == expected);
    }

    SECTION("write floating point numbers → expected bytes written", "[functional_requirements]") {
        const std::filesystem::path path("../../outputs/file-writer-6-floating-points.txt");
        const std::size_t expected = 73UL;

        constexpr auto writeFloatingPoints = [](const std::filesystem::path& p) {
            FileWriter writer{p};
            writer.floating(121.0077, 4, std::ios::floatfield);
            writer.newline();
            writer.floating(1501.0277541, 5, std::ios::scientific);
            writer.newline();
            writer.floating(9876.543201, 4, std::ios::fixed);
            writer.newline();
            writer.floating(9876.543201);
            writer.newline();
            writer.floating(9876.543201, 6, std::ios::fixed, 15);
            writer.newline();
            return writer.bytes();
        };

        auto written = writeFloatingPoints(path);
        REQUIRE(written == expected);
    }

    SECTION("write booleans → expected bytes written", "[functional_requirements]") {
        const std::filesystem::path path("../../outputs/file-writer-7-booleans.txt");
        const std::size_t expected = 20UL;

        constexpr auto writeBooleans = [](const std::filesystem::path& p) {
            FileWriter writer{p};
            writer.boolean(false);
            writer.newline();
            writer.boolean(true);
            writer.newline();
            writer.boolean(false, 9);
            return writer.bytes();
        };

        auto written = writeBooleans(path);
        REQUIRE(written == expected);
    }

    SECTION("write, set & get position, then put → expected bytes written", "[functional_requirements]") {
        const std::filesystem::path path("../../outputs/file-writer-8-position.txt");
        const std::string expected = "*1234*67*9*";

        constexpr auto useCursorAndWrite = [](const std::filesystem::path& p) {
            FileWriter writer(p);
            writer.write("0123456789");
            writer.position(5, std::ios::beg);
            writer.put('*');
            writer.position(2, std::ios::cur);
            writer.put('*');
            writer.position(0, std::ios::end);
            writer.put('*');
            writer.position(0, std::ios::beg);
            writer.put('*');
            return writer.toString();
        };

        constexpr auto getFileContent = [](const std::filesystem::path& p) {
            FileLoader loader{p};
            loader.read();
            return std::string(loader.data().data());
        };

        auto repr = useCursorAndWrite(path);
        auto str = getFileContent(path);
        REQUIRE_THAT(str, Catch::Matchers::StartsWith(expected));
        REQUIRE_THAT(repr, Catch::Matchers::ContainsSubstring(path));
        REQUIRE_THAT(repr, Catch::Matchers::ContainsSubstring("\"14\""));
    }

    SECTION("write, move cursor, then get position → cursor repositioned", "[basic_check]") {
        const std::filesystem::path path("../../outputs/file-writer-9-position.txt");
        const std::fstream::pos_type expected = 4;

        constexpr auto reposition = [](const std::filesystem::path& p) {
            FileWriter writer(p);
            writer.write("paganini");
            writer.position(4, std::ios::beg);
            return writer.position();
        };

        auto pos = reposition(path);
        REQUIRE(pos == expected);
    }

    SECTION("write width-lines(integer, floating-point) → expected bytes written & str representation", "[use_case]") {
        const std::filesystem::path path("../../outputs/file-writer-10-width-numbers.txt");
        const std::size_t expected = 440UL;

        constexpr auto populateFile = [](const std::filesystem::path& p) {
            const std::size_t size = 10;
            const std::array<int, size> integers = {1, 22, 333, 4444, 55555, 666666, 76543, 8765, 987, 0};
            const std::array<float, size> floats = {1, +22, .333f, 44.44f, 555.55f, 66666.6f, 7.6543f, 87.65f, 0.987f, -0};

            const std::streamsize width = 20;
            FileWriter writer(p);

            for (auto i = 0; i != size; ++i) {
                writer.integer(integers[i], std::ios::dec, width);
                writer.write(" | ");
                writer.floating(floats[i], 5, std::ios::fixed, width);
                writer.newline();
            }

            return writer.bytes();
        };

        auto bytes = populateFile(path);
        REQUIRE(bytes == expected);
    }

}
