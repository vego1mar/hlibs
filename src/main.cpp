#include <iostream>

#include "logging/console_logger.hpp"
#include "ansi/ansi.hpp"


namespace hello_world {

    static void PrintColorful()
    {
        using logging::ConsoleLogger;
        using Con = ansi::ANSISequencer;
        using Format = ansi::ANSISequencer::DisplayFormat;

        ConsoleLogger logger{};
        logger.debug("Hello World!");

        std::cout << Con::SetDisplay(Format::Invert);
        std::cout << "Colorful Hello World!" << '\n';
        std::cout << Con::SetDisplay(Format::NotInverted);
        std::cout << std::endl;
    }

}

int main()
{
    hello_world::PrintColorful();
    return EXIT_SUCCESS;
}
