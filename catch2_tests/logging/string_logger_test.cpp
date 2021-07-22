#include "../catch.hpp"
#include "../../src/logging/logger.hpp"
#include "../../src/logging/string_logger.hpp"
#include <cctype>


TEST_CASE("LoggerMsgHelper", "[logging]") {

    SECTION("GetDateTime() -> date=YYYY-MM-DD, time=HH:MM:SS", "[functional_requirements]") {
        logging::LoggerMsgHelper helper;
        helper.build();
        const auto& date = helper.date;
        const auto& time = helper.time;

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


TEST_CASE("StringLogger", "[logging]") {

    SECTION("?", "[functional_requirements]") {
        std::string target{};
        logging::StringLogger logger{target};
    }

}
