#include <catch2/catch_test_macros.hpp>
#include <cctype>

#include "../../sources/facilities/timestamp.hpp"


TEST_CASE("GetDateTime()", "[libs][facilities][timestamp]")
{
    using hlibs::facilities::timestamp::GetDate;
    using hlibs::facilities::timestamp::GetTime;


    SECTION("GetDateTime() -> date=YYYY-MM-DD, time=HH:MM:SS", "[functional_requirements]") {
        const auto& date = GetDate();
        const auto& time = GetTime();

        for (int i = 0; i < date.size(); i++) {
            if (i == 4 || i == 7) {
                REQUIRE(date[i] == '-');
                continue;
            }

            bool is_digit = std::isdigit(date[i]);
            REQUIRE(is_digit);
        }

        for (std::size_t i = 0; i < time.size(); i++) {
            if (i == 2 || i == 5) {
                REQUIRE(time[i] == ':');
                continue;
            }

            bool is_digit = std::isdigit(time[i]);
            REQUIRE(is_digit);
        }
    }

}
