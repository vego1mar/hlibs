#include <iostream>

#include "logging/console_logger.hpp"
#include "ansi/ansi.hpp"


int main()
{
    using logging::ConsoleLogger;
    using ansi::SGRSequencer;
    using SGRDisplayAttribute = ansi::DisplayAttribute::SGRDisplayAttribute;

    ConsoleLogger logger{};
    SGRSequencer sgrSequencer{};

    std::cout << sgrSequencer.getDisplayAttribute(SGRDisplayAttribute::Invert);
    logger.debug("Hello World!");
    std::cout << sgrSequencer.getDisplayAttribute(SGRDisplayAttribute::NotInverted);

    return EXIT_SUCCESS;
}
