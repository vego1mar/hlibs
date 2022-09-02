#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <string>

#include "../../sources/facilities/strings.hpp"


TEST_CASE("ToUpperCase", "[libs][facilities][strings]")
{
    using hlibs::facilities::strings::ToUpperCase;

    SECTION("ToUpperCase('lvalue_str') -> 'LVALUE_STR'", "[functional_requirements]") {
        const std::string input("camelCase");
        const std::string expectedStr("CAMELCASE");
        const auto result = ToUpperCase(input);
        REQUIRE_THAT(result, Catch::Matchers::Equals(expectedStr));
    }

    SECTION("ToUpperCase(const char*) -> str(STR)", "[functional_requirements]") {
        const char* input = "camelCase with spaces and numbers 0123";
        const std::string expectedStr("CAMELCASE WITH SPACES AND NUMBERS 0123");
        const auto result = ToUpperCase(input);
        REQUIRE_THAT(result, Catch::Matchers::Equals(expectedStr));
    }
}

TEST_CASE("Contains", "[libs][facilities][strings]")
{
    using hlibs::facilities::strings::Contains;

    SECTION("Contains(char) -> true", "[functional_requirements]") {
        std::string source("xXVvOfFvjW9zHyNAVzQL\n");
        bool isNewLined = Contains(source, '\n');
        REQUIRE(isNewLined);
    }

    SECTION("Contains(str) -> true", "[functional_requirements]") {
        std::string source("5D7OVjSrm0JLVEgTwae2");
        std::string strToFind("m0JLVEg");
        bool isFound = Contains(source, strToFind);
        REQUIRE(isFound);
    }

    SECTION("Contains(str) -> false", "[functional_requirements]") {
        std::string source("MMWfSBhRBOiMau3yAfMO");
        std::string strToFind("RBOj");
        bool isFound = Contains(source, strToFind);
        REQUIRE(!isFound);
    }
}
