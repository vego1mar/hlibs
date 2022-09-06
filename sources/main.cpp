#include <iostream>

#include "logging/logger.hpp"


namespace hello_world {

    [[maybe_unused]] static void PrintColorfulHello()
    {
        using hlibs::logging::TerminalLogger;

        TerminalLogger logger{};
        logger.debug("Hello Debug!");
        logger.info("Hello Info!");
        logger.warning("Hello Warning!");
        logger.fatal("Hello Fatal!");

        try {
            throw std::domain_error("Deliberately thrown!");
        }
        catch (const std::domain_error& e) {
            logger.exception("Hello exception!", e);
        }
    }

}


int main()
{
    hello_world::PrintColorfulHello();
    return EXIT_SUCCESS;
}

