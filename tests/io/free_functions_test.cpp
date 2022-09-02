#include <catch2/catch_test_macros.hpp>

#include <filesystem>

#include "../../sources/io/free_functions.hpp"


TEST_CASE("GetFileSize()", "[libs][io][GetFileSize]")
{
    using hlibs::io::GetFileSize;

    SECTION("get file size, path is lvalue → expected file size", "[type_traits][functional_requirements]") {
        const std::filesystem::path path("../../inputs/get-file-size-1.txt");
        constexpr std::size_t expected = 5805UL;

        constexpr auto getFileSize = [](const std::filesystem::path& p) {
            CHECK(std::is_lvalue_reference_v<decltype(p)>);
            auto size = GetFileSize(p);
            return size;
        };

        auto size = getFileSize(path);
        REQUIRE(size == expected);
    }

    SECTION("get unicode file size → expected file size", "[type_traits][functional_requirements]") {
        const std::filesystem::path path("../../inputs/get-file-size-2-unicode.txt");

        auto size = GetFileSize(path);
        auto cppSize = std::filesystem::file_size(std::filesystem::path(path));

        REQUIRE(size == 9920UL);
        REQUIRE(size == cppSize);
    }

}
