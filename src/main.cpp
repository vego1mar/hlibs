#include "logging/console_logger.hpp"
#include "ansi/ansi.hpp"


int main() {
    using logging::ConsoleLogger;

    ConsoleLogger logger{};
    logger.debug("Hello World!");

    return EXIT_SUCCESS;
}
