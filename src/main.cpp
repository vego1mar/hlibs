#include "logging/stdout_logger.hpp"


int main() {
    logging::StdOutLogger logger(true);
    logger.debug("Hello World!");
    return EXIT_SUCCESS;
}
