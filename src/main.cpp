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

// TODO: logging plan
// * specialize ConsoleLogger from StdOutLogger, override 'event methods' to build own messages
// * make another plan that will realize TODOs
// * remove setting_skip, disable base StdOutLogger, build own messages list
// * use ::ansi to print colorful logs ({fatal, exception, info, debug} → {red, teal, black, bg clementine/reverse video})
// * query terminal for ANSI/TrueColor support  # https://gist.github.com/XVilka/8346728

// TODO: types plan
// * non-contiguous Range
// * RGBColor - provide named colors
// * RGBColor - from/to hex, CMY, CMYK, 256 color set

// TODO: string plan
// * Split(), Contains(); check LM project for more ideas
// * CheckRange: type=[Letters, Digits, Alphanumerics, Special, ControlChars, ASCII]
// * CheckOrder: type=[Lexicographical, ASCII-betical, Alphabetical_Unicode, Subsequent]

// TODO: doc/README
// * expected usage for functions from all namespaces
// * ANSISequencer whole possibilities in action, beside ConsoleLogger
