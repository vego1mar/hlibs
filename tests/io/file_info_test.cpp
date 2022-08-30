#include <catch2/catch_test_macros.hpp>

#include "../../sources/io/file_info.hpp"


TEST_CASE("GetFileSize()", "[libs][io][file]")
{
    using hlibs::io::GetFileSize;

    SECTION("f(literal) -> 1213 B", "[functional_requirements]") {
        constexpr long LICENSE_MD_SIZE = 1213;
        const auto size = GetFileSize("../../UNLICENSE.md");
        CHECK(size == LICENSE_MD_SIZE);
    }

    SECTION("f(std::strings) -> 65 B", "[functional_requirements]") {
        constexpr long README_MD_SIZE = 44;
        const std::string README_MD_PATH("../../README.md");
        const auto size = GetFileSize(README_MD_PATH);
        CHECK(size == README_MD_SIZE);
    }

}
