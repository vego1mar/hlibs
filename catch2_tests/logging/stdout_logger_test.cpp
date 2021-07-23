#include "../catch.hpp"
#include "../../src/logging/stdout_logger.hpp"


TEST_CASE("StdOutLogger", "[logging]") {

    SECTION("? -> ?", "[functional_requirements]") {
        logging::StdOutLogger logger{true, 4 };
        logger.debug("debug1");
        logger.info("info2");
        logger.fatal("fatal3");
        logger.warning("warning4");

        // TODO: try to redirect stdout and stderr into files
        // TODO: file::StreamToFile
    }

}
