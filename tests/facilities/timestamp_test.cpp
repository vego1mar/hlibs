#include <catch2/catch_test_macros.hpp>
#include <cctype>

#include "../../sources/facilities/timestamp.hpp"
#include "../../sources/facilities/strings.hpp"


TEST_CASE("DateTime", "[libs][facilities][timestamp][DateTime]")
{
    using hlibs::facilities::timestamp::DateTime;
    using hlibs::facilities::strings::Contains;

    SECTION("is_standard_layout → true", "[type_traits]") {
        REQUIRE(std::is_standard_layout_v<DateTime>);
    }

    SECTION("is_default_constructible → true", "[type_traits]") {
        REQUIRE(std::is_default_constructible_v<DateTime>);
    }

    SECTION("construct DateTime → date=YYYY-MM-DD, time=HH:MM:SS", "[functional_requirements]") {
        constexpr auto isCorrectStr = [](std::string_view sv, char sep) {
            bool containsWhiteSpace = Contains(sv, ' ');
            bool containsSeparator = Contains(sv, sep);
            return !containsWhiteSpace && containsSeparator;
        };

        DateTime dt{};
        bool isCorrectDate = isCorrectStr(dt.date, '-');
        bool isCorrectTime = isCorrectStr(dt.time, ':');
        bool isCorrect = isCorrectDate && isCorrectTime;
        REQUIRE(isCorrect);
    }

    SECTION("construct localized DateTime → expected str size", "[basic_check]") {
        constexpr auto constructLocalized = []() {
            DateTime dt(std::locale("pl_PL.utf8"));
            bool isDateLength = dt.date.size() == 10;
            bool isTimeLength = dt.time.size() == 8;
            return isDateLength && isTimeLength;
        };

        auto isProperLength = constructLocalized();
        REQUIRE(isProperLength);
    }

}
