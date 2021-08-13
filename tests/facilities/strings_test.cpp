#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <string>

#include "../../sources/facilities/strings.hpp"


TEST_CASE("ToUpperCase", "[libs][facilities][string]")
{
    using hlibs::facilities::string::ToUpperCase;

    SECTION("ToUpperCase(str::string) -> str(STR)", "[functional_requirements]") {
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

TEST_CASE("Contains", "[libs][facilities][string]")
{
    using hlibs::facilities::string::Contains;

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


TEST_CASE("CheckRange", "[libs][facilities][string]")
{
    using hlibs::facilities::string::CheckRange;
    using hlibs::types::Range;

    SECTION("CheckRange(97,122) -> OK", "[functional_requirements]") {
        Range<char> range1('a', 'z');
        Range<int> range2(range1);
        const std::string source1("abcdefghijklmnopqrstuvwxyz");
        const std::string source2("abcdefghijKlmnopqrstuvwxyz");
        CHECK(typeid(range1.left) == typeid(const char));
        CHECK(typeid(range2.left) == typeid(const int));

        const bool isStrInRange1 = CheckRange<char>(source1, range1);
        const bool isStrInRange2 = CheckRange<int>(source2, range2);
        REQUIRE(isStrInRange1);
        REQUIRE(!isStrInRange2);
    }

    SECTION("CheckRange('\\033',0x7E) -> true", "[functional_requirements]") {
        Range<char> range('\033', 0x7E);
        const std::string source("\033[38;2;0;255;1m");
        const bool isStrInRange = CheckRange<char>(source, range);
        REQUIRE(isStrInRange);
    }

}

