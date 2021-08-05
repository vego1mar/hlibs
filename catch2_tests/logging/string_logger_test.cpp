#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <string>

#include "../../src/logging/logger.hpp"
#include "../../src/logging/string_logger.hpp"


TEST_CASE("StringLogger", "[libs][logging]")
{
    using libs::logging::LoggerSettings;
    using libs::logging::StringLogger;


    SECTION("info() → flush=OK", "[functional_requirements]") {
        LoggerSettings settings{};
        settings.messages_before_flush = 2;
        settings.is_enabled = true;

        auto target = std::make_shared<std::string>();
        CHECK(target.use_count() == 1L);

        StringLogger logger(target);
        logger.set(settings);
        logger.info("message1");
        logger.info("2message");

        std::string expectedMsg("[INFO] string_logger_test.cpp(");
        std::string expectedMiddle(" @\"____C_A_T_C_H____T_E_S_T____0\": message1\n");
        std::string expectedEnding(" @\"____C_A_T_C_H____T_E_S_T____0\": 2message\n");
        REQUIRE_THAT(*target, Catch::Matchers::Contains(expectedMsg));
        REQUIRE_THAT(*target, Catch::Matchers::Contains(expectedMiddle));
        REQUIRE_THAT(*target, Catch::Matchers::EndsWith(expectedEnding));
    }

    SECTION("info(), StringLogger=disabled → flush=empty", "[functional_requirements]") {
        LoggerSettings settings{};
        settings.messages_before_flush = 1;
        settings.is_enabled = false;

        auto target = std::make_shared<std::string>();
        CHECK(target.use_count() == 1L);

        StringLogger logger{target};
        logger.set(settings);
        logger.info("message that should not be buffered");

        REQUIRE_THAT(*target, Catch::Matchers::Equals(""));
    }

    SECTION("exception() → flush=OK", "[functional_requirements]") {
        LoggerSettings settings{};
        settings.messages_before_flush = 1;

        auto target = std::make_shared<std::string>();
        CHECK(target.use_count() == 1L);

        StringLogger logger{target};
        logger.set(settings);
        const std::string exMsg("Deliberately thrown runtime_exception!");
        const std::string msgToLog("std::runtime_error");

        try {
            throw std::runtime_error(exMsg);
        }
        catch (std::runtime_error& e) {
            logger.exception(msgToLog, e);
        }

        const std::string expectedMsg(" [DEBUG] string_logger_test.cpp(");
        const std::string exType = typeid(std::runtime_error).name();
        const std::string expectedEnding =
                ") @\"____C_A_T_C_H____T_E_S_T____0\": std::exception->" + exType + " | what: " + exMsg + " | " + msgToLog + '\n';
        REQUIRE_THAT(*target, Catch::Matchers::Contains(expectedMsg));
        REQUIRE_THAT(*target, Catch::Matchers::EndsWith(expectedEnding));
    }

    SECTION("~StringLogger() → buffer flushed", "[functional_requirements]") {
        auto target = std::make_shared<std::string>();
        CHECK(target.use_count() == 1L);

        constexpr auto logOneMessage = [](std::shared_ptr<std::string>& exTarget) {
            StringLogger logger{exTarget};
            CHECK(exTarget.use_count() == 2L);
            LoggerSettings settings{};
            settings.messages_before_flush = 2;
            logger.set(settings);
            logger.debug("dtor_check");
            REQUIRE_THAT(*exTarget, Catch::Matchers::Equals(""));
        };

        logOneMessage(target);
        CHECK(target.use_count() == 1L);
        const std::string expectedEnding("dtor_check\n~StringLogger(1)\n");
        REQUIRE_THAT(*target, Catch::Matchers::EndsWith(expectedEnding));
    }

    SECTION("disabled → OK", "functional_requirements") {
        LoggerSettings settings{};
        settings.is_enabled = false;
        StringLogger logger;
        logger.set(settings);
        logger.debug("debug");
        logger.info("info");
        logger.warning("warning");
        logger.fatal("fatal");
        logger.exception("exception", std::runtime_error("dummy"));
        REQUIRE_NOTHROW(logger);
    }

    SECTION("inserter<< → unbuffered messages", "use_case") {
        LoggerSettings settings{};
        settings.messages_before_flush = 100;
        settings.use_stdout = false;
        StringLogger logger{};

        constexpr auto getUnbufferedMessages = [](const StringLogger& logger) -> std::string {
            std::ostringstream oss;
            oss << logger;
            return oss.str();
        };

        const std::string msg = "Unbuffered messages retrieved!";
        logger.debug(msg);
        const auto unbufferedMsg = getUnbufferedMessages(logger);
        REQUIRE_THAT(unbufferedMsg, Catch::Matchers::Contains("[DEBUG] string_logger_test.cpp("));
        REQUIRE_THAT(unbufferedMsg, Catch::Matchers::Contains(") @\"____C_A_T_C_H____T_E_S_T____0\": "));
        REQUIRE_THAT(unbufferedMsg, Catch::Matchers::Contains(msg));
    }

}
