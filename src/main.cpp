#include <iostream>

#include "logging/console_logger.hpp"


namespace hello_world {

    static void PrintColorfulHello()
    {
        using libs::logging::ConsoleLogger;
        using libs::logging::LoggerSettings;

        LoggerSettings settings{};
        settings.messages_before_flush = 10;

        ConsoleLogger logger{};
        logger.set(settings);
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


