#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <string>

#include "../../sources/facilities/strings.hpp"


TEST_CASE("ToUpperCase", "[libs][facilities][strings][ToUpperCase]")
{
    using hlibs::facilities::strings::ToUpperCase;

    SECTION("not all chars lowercase → all uppercase", "[functional_requirements]") {
        const std::string input("camelCase");
        const std::string expected("CAMELCASE");

        auto result = ToUpperCase(input);

        REQUIRE_THAT(result, Catch::Matchers::Equals(expected));
    }

    SECTION("text with numbers & control characters → all uppercase", "[functional_requirements]") {
        const std::string input = "camel_Case\r\fwith_spaces and\tnumbers\t0123.";
        const std::string expected("CAMEL_CASE\r\fWITH_SPACES AND\tNUMBERS\t0123.");

        const auto result = ToUpperCase(input);

        REQUIRE_THAT(result, Catch::Matchers::Equals(expected));
    }

    SECTION("all lowercase, T=const char* → all uppercase", "[basic_check]") {
        const char* input = "abracadabra";
        const std::string expected("ABRACADABRA");

        const auto result = ToUpperCase(input);

        REQUIRE_THAT(result, Catch::Matchers::Equals(expected));
    }

}

TEST_CASE("Contains", "[libs][facilities][strings][Contains]")
{
    using hlibs::facilities::strings::Contains;

    SECTION("unique char in the middle → found", "[functional_requirements]") {
        const std::string source("xXVvOfFvjW9zHyNAVzQL\n");
        bool isFound = Contains(source, 'W');
        REQUIRE(isFound);
    }

    SECTION("unique char at the end → found", "[functional_requirements]") {
        const std::string source("xXVvOfFvjW9zHyNAVzQL\n");
        bool isFound = Contains(source, '\n');
        REQUIRE(isFound);
    }

    SECTION("unique char at the beginning → found", "[functional_requirements]") {
        const std::string source("aBxXVvOfFvjW9zHyNAVzQL");
        bool isFound = Contains(source, 'a');
        REQUIRE(isFound);
    }

    SECTION("the same lowercase and uppercase char → case sensitive search", "[functional_requirements]") {
        const std::string source("baVvOf_visW9zXHyNAVzQL");

        bool isFoundLower = Contains(source, 'a');
        bool isFoundUpper = Contains(source, 'A');

        bool isFound = isFoundLower && isFoundUpper;
        REQUIRE(isFound);
    }

    SECTION("substring in the middle → found", "[functional_requirements]") {
        const std::string source("5D7OVjSrm0JLVEg_T.w.a.e_2");
        const std::string expected("m0JLVEg");

        bool isFound = Contains(source, expected);

        REQUIRE(isFound);
    }

    SECTION("substring at the end → found", "[functional_requirements]") {
        const std::string source("5D7OVjSrm0JLVEg_T.w.ae_2");
        const std::string expected("ae_2");

        bool isFound = Contains(source, expected);

        REQUIRE(isFound);
    }

    SECTION("substring at the beginning → found", "[functional_requirements]") {
        const std::string source("5D7OVjSrm0JLVEg");
        const std::string expected("5D7OV");

        bool isFound = Contains(source, expected);

        REQUIRE(isFound);
    }

    SECTION("substring not in string → not found", "[functional_requirements]") {
        const std::string source("MMWfSBhRBOiMau3yAfMO");
        const std::string expected("RBOj");

        bool isFound = Contains(source, expected);

        REQUIRE(!isFound);
    }

}
