#include "logging/console_logger.hpp"


int main() {
    logging::ConsoleLogger logger{};
    logger.debug("Hello World!");
    return EXIT_SUCCESS;
}
