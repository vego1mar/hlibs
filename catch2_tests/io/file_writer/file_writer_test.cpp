#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <iostream>

#include "../../../src/io/file_writer/file_writer.hpp"


TEST_CASE("FileWriter", "[libs][io][file_writer]")
{
    using libs::io::file_writer::FileWriter;

    SECTION("write dump -> OK", "[functional_requirements]") {
        const std::string path("../../outputs/test_FileWriter__1.txt");

        FileWriter writer{path};
        REQUIRE(writer.isOpened());
        // writer <<
        const auto toStr = writer.toString();

        REQUIRE_THAT(toStr, Catch::Matchers::Equals("{test_FileWriter__1.txt, 0}"));
    }

}
