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

}
