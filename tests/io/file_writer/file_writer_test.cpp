#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <iostream>

#include "../../../sources/io/file_writer/file_writer.hpp"
#include "../../../sources/io/free_functions.hpp"


TEST_CASE("FileWriter", "[libs][io][file_writer]")
{
    using hlibs::io::file_writer::FileWriter;
    using hlibs::io::GetFileSize;

    struct Paths {
        static std::string GetNext()
        {
            static uint8_t no = 0;
            ++no;
            std::string path = "../../outputs/test_FileWriter__" + std::to_string(no) + ".txt";
            return path;
        }
    };

    SECTION("write nothing -> OK", "[functional_requirements]") {
        const std::string path = Paths::GetNext();
        FileWriter writer{path};
        REQUIRE(writer.isOpened());

        const auto toStr = writer.toString();
        REQUIRE_THAT(toStr, Catch::Matchers::Equals("{test_FileWriter__1.txt, 0 B}"));
    }

    SECTION("write dump -> OK", "[functional_requirements]") {
        constexpr auto writeDumpData = []() -> std::string {
            const std::string path = Paths::GetNext();
            FileWriter writer(path);

            writer.write('a');
            writer.write(static_cast<signed char>('b'));
            writer.write(static_cast<unsigned char>('\n'));
            writer.write("dump\n");
            writer.write(path);

            return writer.toString();
        };

        const auto reportedState = writeDumpData();
        REQUIRE_THAT(reportedState, Catch::Matchers::Equals("{test_FileWriter__2.txt, 44 B}"));
    }

    SECTION("get/set file cursor -> OK", "[functional_requirements]") {
        const std::string path = Paths::GetNext();

        constexpr auto writeDataWithCursorManipulations = [](const std::string& path) {
            FileWriter writer(path);
            CHECK(writer.isOpened());

            writer.write("sample text");
            CHECK(writer.getCursorPosition() == 11L);
            writer.setCursorPosition(6);
            writer.write('_');
            CHECK(writer.getCursorPosition() == 7L);
            writer.setCursorPosition(0, std::ios::beg);
            writer.write('S');
            CHECK(writer.getCursorPosition() == 1L);
            writer.setCursorPosition(0, std::ios::end);
            writer.write('!');
            CHECK(writer.getCursorPosition() == 12L);
        };

        REQUIRE_NOTHROW(writeDataWithCursorManipulations(path));
        const auto fileSize = GetFileSize(path);
        const bool isInValidSize = fileSize == 12L || fileSize == 14L;
        REQUIRE(isInValidSize);
    }

    SECTION("write using inserter -> OK", "[functional_requirements]") {
        const std::string path = Paths::GetNext();

        constexpr auto writeUsingInserter = [](const std::string& path) {
            FileWriter writer(path);
            writer << "dump";
            writer << '\t';
            writer << static_cast<signed char>('a');
            writer << static_cast<char>('b');
            writer << static_cast<unsigned char>('c');
        };

        writeUsingInserter(path);
        const auto fileSize = GetFileSize(path);
        REQUIRE(fileSize == 8L);
    }

    SECTION("move semantic -> OK", "[functional_requirements]") {
        const std::string path = Paths::GetNext();

        constexpr auto moveConstruct = [](const std::string& path) -> std::string {
            FileWriter writer1(path);
            writer1.write("Sample text");
            CHECK(writer1.isOpened());
            CHECK(GetFileSize(path) == 0L);

            FileWriter writer2(std::move(writer1));
            REQUIRE(writer2.isOpened());
            REQUIRE(GetFileSize(path) == 0L);
            writer2 << " dump.";

            FileWriter writer3 = std::move(writer2);
            REQUIRE(writer3.isOpened());
            REQUIRE(GetFileSize(path) == 0L);

            return writer3.toString();
        };

        const auto repr = moveConstruct(path);
        REQUIRE_THAT(repr, Catch::Matchers::Equals("{test_FileWriter__5.txt, 17 B}"));
        REQUIRE(GetFileSize(path) == 17L);
    }

}
