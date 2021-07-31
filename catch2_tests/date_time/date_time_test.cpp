#include <cctype>

#include "../../externs/catch.hpp"
#include "../../src/date_time/date_time.hpp"


TEST_CASE("date_time", "[date_time]")
{

    SECTION("GetDateTime() -> date=YYYY-MM-DD, time=HH:MM:SS", "[functional_requirements]") {
        const auto& date = date_time::GetDate();
        const auto& time = date_time::GetTime();

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
