#include "../catch.hpp"
#include "../../src/file/file_info.hpp"


TEST_CASE("GetFileSize()", "[file][GetFileSize]") {

    SECTION("f(literal) -> 1213 B", "[functional_requirements]") {
        constexpr long LICENSE_MD_SIZE = 1213;
        const auto size = file::info::GetFileSize("../../UNLICENSE.md");
        CHECK(size == LICENSE_MD_SIZE);
    }

    SECTION("f(std::string) -> 65 B", "[functional_requirements]") {
        constexpr long README_MD_SIZE = 44;
        const std::string README_MD_PATH("../../README.md");
        const auto size = file::info::GetFileSize(README_MD_PATH);
        CHECK(size == README_MD_SIZE);
    }

}
