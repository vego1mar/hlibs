#define CATCH_CONFIG_MAIN

#include "catch.hpp"

TEST_CASE("root", "[main]") {

    SECTION("dump", "[main]") {
        REQUIRE(true);
    }

}
