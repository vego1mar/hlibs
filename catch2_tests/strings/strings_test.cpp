#include <string>

#include "../catch.hpp"
#include "../../src/strings/strings.hpp"


TEST_CASE("strings", "[strings]") {

    SECTION("ToUpperCase(str::string) -> str(STR)", "[functional_requirements]") {
        const std::string input("camelCase");
        const std::string expectedStr("CAMELCASE");
        const auto result = strings::ToUpperCase(input);
        REQUIRE_THAT(result, Catch::Equals(expectedStr));
    }

    SECTION("ToUpperCase(const char*) -> str(STR)", "[functional_requirements]") {
        const char* input = "camelCase with spaces and numbers 0123";
        const std::string expectedStr("CAMELCASE WITH SPACES AND NUMBERS 0123");
        const auto result = strings::ToUpperCase(input);
        REQUIRE_THAT(result, Catch::Equals(expectedStr));
    }

    SECTION("Contains(char) -> true", "[functional_requirements]") {
        std::string source("xXVvOfFvjW9zHyNAVzQL\n");
        bool isNewLined = strings::Contains(source, '\n');
        REQUIRE(isNewLined);
    }

    SECTION("Contains(str) -> true", "[functional_requirements]") {
        std::string source("5D7OVjSrm0JLVEgTwae2");
        std::string strToFind("m0JLVEg");
        bool isFound = strings::Contains(source, strToFind);
        REQUIRE(isFound);
    }

    SECTION("Contains(str) -> false", "[functional_requirements]") {
        std::string source("MMWfSBhRBOiMau3yAfMO");
        std::string strToFind("RBOj");
        bool isFound = strings::Contains(source, strToFind);
        REQUIRE(!isFound);
    }

}
