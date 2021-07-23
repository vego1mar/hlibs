#include "../catch.hpp"
#include "../../src/logging/logger.hpp"
#include "../../src/logging/string_logger.hpp"


TEST_CASE("StringLogger", "[logging]") {

    SECTION("info() -> flush=OK", "[functional_requirements]") {
        std::string target{};
        logging::LoggerSettings settings{};
        settings.messages_before_flush = 2;
        settings.is_enabled = true;

        logging::StringLogger logger{target};
        logger.set(std::move(settings));
        logger.info("message1");
        logger.info("2message");

        std::string expectedMsg("[INFO] string_logger_test.cpp(");
        std::string expectedMiddle(" @\"____C_A_T_C_H____T_E_S_T____0\": message1\n");
        std::string expectedEnding(" @\"____C_A_T_C_H____T_E_S_T____0\": 2message\n");
        REQUIRE_THAT(target, Catch::Contains(expectedMsg));
        REQUIRE_THAT(target, Catch::Contains(expectedMiddle));
        REQUIRE_THAT(target, Catch::EndsWith(expectedEnding));
    }

    SECTION("info(), StringLogger=disabled -> flush=empty", "[functional_requirements]") {
        std::string target{};
        logging::LoggerSettings settings{};
        settings.messages_before_flush = 1;
        settings.is_enabled = false;

        logging::StringLogger logger{target};
        logger.set(std::move(settings));
        logger.info("message that should not be buffered");

        REQUIRE_THAT(target, Catch::Equals(""));
    }

}
