#include "../catch.hpp"
#include "../../src/ansi/ansi.hpp"


TEST_CASE("ansi", "[ansi]") {

    SECTION("CSI set/get -> OK", "[functional_requirements]") {
        ansi::CSI csi{};
        const auto octalCSI = csi();
        REQUIRE_THAT(octalCSI, Catch::Equals("\\033["));

        csi.set(ansi::EscapeCharacterType::Hexadecimal);
        const auto hexCSI = csi();
        REQUIRE_THAT(hexCSI, Catch::Equals("\\x1B["));

        csi.set(ansi::EscapeCharacterType::Octal);
        const auto result = csi();
        REQUIRE_THAT(result, Catch::Equals(octalCSI));
    }

    SECTION("CSIEnclose -> convertible to char", "[functional_requirements]") {
        const auto enclose = ansi::CSIEnclose::SGR;
        const char endChar = static_cast<const char>(enclose);
        REQUIRE(endChar == 'm');
    }

    SECTION("Enclose -> convertible to char", "[functional_requirements]") {
        const auto enclose = ansi::Enclose{ ansi::CSIEnclose::SD };
        const char endChar = enclose();
        REQUIRE(endChar == 'T');
    }

}
