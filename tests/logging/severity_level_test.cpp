#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "../../sources/logging/logger.hpp"


TEST_CASE("SeverityLevel", "[libs][logging]")
{
    using libs::logging::SeverityLevel;


    SECTION("Level::Info -> str(Info)", "[functional_requirements]") {
        const std::string expectedStr = "Info";
        const auto level = SeverityLevel::Level::Info;
        const auto levelStr = SeverityLevel::ToString(level);
        CHECK_THAT(levelStr, Catch::Matchers::Equals(expectedStr));
    }

    SECTION("enum(Level) -> iteration & toString OK", "[functional_requirements]") {
        using Level = SeverityLevel::Level;

        const std::unordered_map<Level, std::string> expectedStr{
                {Level::Debug,   "Debug"},
                {Level::Info,    "Info"},
                {Level::Warning, "Warning"},
                {Level::Fatal,   "Fatal"},
        };

        auto it = SeverityLevel::Begin();
        int i = 0;

        while (it != SeverityLevel::End()) {
            const auto& value = it;
            bool isInExpected = expectedStr.contains(value);
            CHECK(isInExpected);

            const auto previousValue = value;
            ++i;
            it = SeverityLevel::Next(i);
            const auto& nextValue = it;
            bool isValueDistinct = previousValue != nextValue;
            REQUIRE(isValueDistinct);
        }

        CHECK(i == expectedStr.size());

        for (const auto& item : expectedStr) {
            const auto& level = item.first;
            const auto& levelStr = item.second;
            const auto str = SeverityLevel::ToString(level);
            REQUIRE_THAT(str, Catch::Matchers::Equals(levelStr));
        }
    }

}
