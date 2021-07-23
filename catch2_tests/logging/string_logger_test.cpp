#include "../catch.hpp"
#include "../../src/logging/logger.hpp"
#include "../../src/logging/string_logger.hpp"


TEST_CASE("StringLogger", "[logging]") {

    SECTION("?", "[functional_requirements]") {
        std::string target{};
        logging::StringLogger logger{target};

        logger.info("dummy");
    }

}
