#include "../../externs/catch.hpp"
#include "../../src/logging/logger.hpp"
#include "../../src/logging/string_logger.hpp"


TEST_CASE("StringLogger", "[logging]")
{
    using libs::logging::LoggerSettings;
    using libs::logging::StringLogger;


    SECTION("info() -> flush=OK", "[functional_requirements]") {
        std::string target{};
        LoggerSettings settings{};
        settings.messages_before_flush = 2;
        settings.is_enabled = true;

        StringLogger logger{target};
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
        LoggerSettings settings{};
        settings.messages_before_flush = 1;
        settings.is_enabled = false;

        StringLogger logger{target};
        logger.set(std::move(settings));
        logger.info("message that should not be buffered");

        REQUIRE_THAT(target, Catch::Equals(""));
    }

    SECTION("exception() -> flush=OK", "[functional_requirements]") {
        std::string target{};
        LoggerSettings settings{};
        settings.messages_before_flush = 1;

        StringLogger logger{target};
        logger.set(std::move(settings));
        const std::string exceptionMsg("Deliberately thrown runtime_exception!");
        const std::string msgToLog("std::runtime_error");

        try {
            throw std::runtime_error(exceptionMsg);
        }
        catch (std::runtime_error& e) {
            logger.exception(msgToLog, e);
        }

        std::string expectedMsg(" [DEBUG] string_logger_test.cpp(");
        const std::string expectedEnding =
                ") @\"____C_A_T_C_H____T_E_S_T____0\": std::exception | " + msgToLog + " | " + exceptionMsg + '\n';
        REQUIRE_THAT(target, Catch::Contains(expectedMsg));
        REQUIRE_THAT(target, Catch::EndsWith(expectedEnding));
    }

    SECTION("~StringLogger() -> buffer flushed", "[functional_requirements]") {
        std::string target{};

        constexpr auto logOneMessage = [](std::string& target) {
            LoggerSettings settings{};
            settings.messages_before_flush = 2;

            StringLogger logger{target};
            logger.set(std::move(settings));
            logger.debug("dtor_check");

            REQUIRE_THAT(target, Catch::Equals(""));
        };

        logOneMessage(target);
        const std::string expectedEnding("dtor_check\n~StringLogger(1)\n");
        REQUIRE_THAT(target, Catch::EndsWith(expectedEnding));
    }

}
