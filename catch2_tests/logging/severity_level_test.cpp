#include "../catch.hpp"
#include "../../src/logging/logger.hpp"


TEST_CASE("SeverityLevel", "[logging]") {

    SECTION("Level::Info -> str(Info)", "[functional_requirements]") {
        const std::string expectedStr = "Info";
        const auto level = logging::SeverityLevel::Level::Info;
        std::ostringstream out;

        out << level;

        CHECK_THAT(out.str(), Catch::Equals(expectedStr));
    }

    SECTION("enum(Level) -> set_str(_1, _2, ...)", "[functional_requirements]") {
        using SeverityLevel = logging::SeverityLevel;
        using Level = logging::SeverityLevel::Level;

        const std::unordered_map<Level, std::string> expectedStr{
                {Level::Debug,   "Debug"},
                {Level::Info,    "Info"},
                {Level::Warning, "Warning"},
                {Level::Error,   "Error"},
                {Level::Fatal,   "Fatal"},
        };

        auto it = SeverityLevel::begin();
        int i = 0;

        while (it != SeverityLevel::end()) {
            const auto& value = it;
            bool isInExpected = expectedStr.contains(value);
            CHECK(isInExpected);

            const auto previousValue = value;
            ++i;
            it = SeverityLevel::next(i);
            const auto& nextValue = it;
            bool isValueDistinct = previousValue != nextValue;
            REQUIRE(isValueDistinct);
        }

        CHECK(i == expectedStr.size());
    }

}
