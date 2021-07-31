#include <iostream>

#include "logging/console_logger.hpp"
#include "standard/ansi.hpp"


namespace hello_world {

    static void PrintColorful()
    {
        using libs::logging::ConsoleLogger;
        using Console = libs::standard::ansi::ANSISequencer;
        using Format = libs::standard::ansi::ANSISequencer::DisplayFormat;

        ConsoleLogger logger{};
        logger.debug("Hello World!");

        std::cout << Console::SetDisplay(Format::Invert);
        std::cout << "Colorful Hello World!" << '\n';
        std::cout << Console::SetDisplay(Format::NotInverted);
    }

}


int main()
{
    hello_world::PrintColorful();
    return EXIT_SUCCESS;
}
