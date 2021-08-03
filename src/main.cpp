#include <iostream>
#include <exception>

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
            throw std::runtime_error("Deliberately thrown!");
        }
        catch (std::exception& e) {
            logger.exception("Hello exception!", e);
        }
    }

}


int main()
{
    hello_world::PrintColorfulHello();
    return EXIT_SUCCESS;
}

// TODO: logging plan
// * query terminal for ANSI/TrueColor support  # https://gist.github.com/XVilka/8346728

// TODO: types plan
// * non-contiguous Range
// * RGBColor - provide named colors
// * RGBColor - from/to hex, CMY, CMYK, 256 color set
// * RGColor - complementary colors # https://www.sessions.edu/color-calculator/
// * RGBColor - contrast colors # https://coolors.co/contrast-checker/008080-acc8e5

// TODO: string plan
// * Split(), Contains(); check LM project for more ideas
// * CheckRange: type=[Letters, Digits, Alphanumerics, Special, ControlChars, ASCII]
// * CheckOrder: type=[Lexicographical, ASCII-betical, Alphabetical_Unicode, Subsequent]

// TODO: doc/README
// * expected usage for functions from all namespaces
// * ANSISequencer whole possibilities in action, beside ConsoleLogger
