#include <vector>
#include <string>

#include "../../externs/catch.hpp"
#include "../../src/standard/ansi.hpp"


TEST_CASE("CSISequencer", "[libs][standard][ansi]")
{
    using libs::standard::ansi::Enclose;
    using libs::standard::ansi::CSISequencer;


    SECTION("CSIEndChar -> convertible to char", "[functional_requirements]") {
        const auto enclose = Enclose::SGR;
        const char endChar = static_cast<const char>(enclose);
        REQUIRE(endChar == 'm');
    }

    SECTION("Enclose -> convertible to char", "[functional_requirements]") {
        const auto endChar = Enclose::ToChar(Enclose::SD);
        REQUIRE(endChar == 'T');
    }

    SECTION("getCUU() -> \\033[3A", "[functional_requirements]") {
        CSISequencer sequencer{};
        const auto cursorUp = sequencer.getCUU(3);
        REQUIRE_THAT(cursorUp, Catch::Equals("\033[3A"));
    }

    SECTION("getCUD() -> \\033[5B", "[functional_requirements]") {
        CSISequencer sequencer{};
        const auto cursorDown = sequencer.getCUD(5);
        REQUIRE_THAT(cursorDown, Catch::Equals("\033[5B"));
    }

    SECTION("getCUF() -> \\x1B[2C", "[functional_requirements]") {
        CSISequencer sequencer{};
        const auto cursorForward = sequencer.getCUF(2);
        REQUIRE_THAT(cursorForward, Catch::Equals("\x1B[2C"));
    }

    SECTION("getCUB() -> \\x1B[7D", "[functional_requirements]") {
        CSISequencer sequencer{};
        const auto cursorBack = sequencer.getCUB(7);
        REQUIRE_THAT(cursorBack, Catch::Equals("\x1B[7D"));
    }

    SECTION("getCNL() -> \\033[5E", "[functional_requirements]") {
        CSISequencer sequencer;
        const auto cursorNextLine = sequencer.getCNL(5);
        REQUIRE_THAT(cursorNextLine, Catch::Equals("\033[5E"));
    }

    SECTION("getCPL() -> \\033[55F", "[functional_requirements]") {
        CSISequencer sequencer;
        const auto cursorNextLine = sequencer.getCPL(55);
        REQUIRE_THAT(cursorNextLine, Catch::Equals("\033[55F"));
    }

    SECTION("getCHA() -> \\033[8G", "[functional_requirements]") {
        CSISequencer sequencer;
        const auto cursorNextLine = sequencer.getCHA(8);
        REQUIRE_THAT(cursorNextLine, Catch::Equals("\033[8G"));
    }

    SECTION("getCUP() -> \\033[11;23H", "[functional_requirements]") {
        CSISequencer sequencer;
        const auto cursorPosition = sequencer.getCUP(11, 23);
        REQUIRE_THAT(cursorPosition, Catch::Equals("\033[11;23H"));
    }

    SECTION("getHVP() -> \\033[13;32f", "[functional_requirements]") {
        CSISequencer sequencer;
        const auto cursorPosition = sequencer.getHVP(13, 32);
        REQUIRE_THAT(cursorPosition, Catch::Equals("\033[13;32f"));
    }

    SECTION("getSU() -> \\033[33S", "[functional_requirements]") {
        CSISequencer sequencer;
        const auto scrollUp = sequencer.getSU(33);
        REQUIRE_THAT(scrollUp, Catch::Equals("\033[33S"));
    }

    SECTION("getSD() -> \\033[44T", "[functional_requirements]") {
        CSISequencer sequencer;
        const auto scrollDown = sequencer.getSD(44);
        REQUIRE_THAT(scrollDown, Catch::Equals("\033[44T"));
    }

    SECTION("IsValid() -> OK", "[functional_requirements]") {
        const std::vector<std::string> trueSequences = {
                std::string("\033[s"),
                std::string("\033[0m"),
                std::string("\033[23;24f"),
                std::string("\033[38;2;;;0;0;0;0;1111m"),
        };

        const std::vector<std::string> falseSequences = {
                "\033[!",
                "\033[_A",
                "\033[-1C",
                "\033[2;;F",
                "\033[1;2 222m",
                "\033[38;2; ; ;0;0;0;0;1111m",
                "\033[38;2;;;0;0;0;0;m",
                "\033[u\r\f",
                "\033[\nu",
        };

        for (const auto& sequence : trueSequences) {
            const bool isValid = CSISequencer::IsValid(sequence);
            REQUIRE(isValid);
        }

        for (const auto& sequence : falseSequences) {
            const bool isValid = CSISequencer::IsValid(sequence);
            REQUIRE(!isValid);
        }
    }

}


TEST_CASE("ANSISequencer", "[libs][standard][ansi]")
{
    using libs::standard::ansi::ANSISequencer;

    SECTION("? -> ?", "[functional_requirements]") {
        ANSISequencer sequencer;
        const auto clearScreen = ANSISequencer::ResetTerminal();
        REQUIRE_THAT(clearScreen, Catch::Equals("\033c"));
    }

}
