#include "../../externs/catch.hpp"
#include "../../src/logging/logger.hpp"


TEST_CASE("SeverityLevel", "[logging]")
{

    SECTION("Level::Info -> str(Info)", "[functional_requirements]") {
        const std::string expectedStr = "Info";
        const auto level = logging::SeverityLevel::Level::Info;
        const auto levelStr = logging::SeverityLevel::ToString(level);
        CHECK_THAT(levelStr, Catch::Equals(expectedStr));
    }

    SECTION("enum(Level) -> set_str(_1, _2, ...)", "[functional_requirements]") {
        using Level = logging::SeverityLevel::Level;

        const std::unordered_map<Level, std::string> expectedStr{
                {Level::Debug,   "Debug"},
                {Level::Info,    "Info"},
                {Level::Warning, "Warning"},
                {Level::Fatal,   "Fatal"},
        };

        auto it = logging::SeverityLevel::Begin();
        int i = 0;

        while (it != logging::SeverityLevel::End()) {
            const auto& value = it;
            bool isInExpected = expectedStr.contains(value);
            CHECK(isInExpected);

            const auto previousValue = value;
            ++i;
            it = logging::SeverityLevel::Next(i);
            const auto& nextValue = it;
            bool isValueDistinct = previousValue != nextValue;
            REQUIRE(isValueDistinct);
        }

        CHECK(i == expectedStr.size());

        for (const auto& item : expectedStr) {
            const auto& level = item.first;
            const auto& levelStr = item.second;
            const auto str = logging::SeverityLevel::ToString(level);
            REQUIRE_THAT(str, Catch::Equals(levelStr));
        }
    }

}
