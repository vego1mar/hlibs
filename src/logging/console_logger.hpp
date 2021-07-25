#ifndef CONSOLE_LOGGER_HPP
#define CONSOLE_LOGGER_HPP

#include <iostream>

#include "stdout_logger.hpp"


namespace logging {

    // https://stackoverflow.com/questions/4842424/list-of-ansi-color-escape-sequences
    /// Suitable for Linux (it uses ANSI escape characters to print colourful output).
    class ConsoleLogger : public StdOutLogger {
    public:
        ConsoleLogger() : StdOutLogger(true, 1) {
            elicitSettings().skip_print_to_stdout = true;
        }

        ConsoleLogger(const ConsoleLogger&) = delete;

        ConsoleLogger(ConsoleLogger&&) noexcept = delete;

        ConsoleLogger& operator=(const ConsoleLogger&) = delete;

        ConsoleLogger& operator=(ConsoleLogger&&) noexcept = delete;

        ~ConsoleLogger() noexcept override {
            if (elicitSettings().messages_before_flush != 1) {
                onFlushInDerivedDestructor();
                printToConsole();
            }

            std::cout << "~ConsoleLogger(" << elicitFlushedMessages() << ")\n";
        }

    protected:
        void onFlush() override {
            printToConsole();
        }

        void printToConsole() const {
            for (const auto&[msgTarget, message] : elicitStdOutMessages()) {
                if (msgTarget == MessageTarget::StdOut) {
                    // std::cout << "\x1B[38;2;0;128;128m" << message; # teal, \x1B for C#
                    //std::cout << "\033[38;2;0;128;128;1m" << message << "\033[0m"; // true colors, 2-bit
                    //std::cout << "\033[38;5;57m" << message << "\033[0m"; // 8-bit colors
                    std::cout << "\033[38:2::136:23:152m" << message << "\033[0m"; // magenta powershell

                    // echo -e "$ESC[48;5;209m"                 # \033 or \x1B
                    // CSI (Control Sequence Introducer)        # $ESC[
                    // SGR (Select Graphics Rendition) params   # 48;5;209m

                    continue;
                    // \033[38;2;<r>;<g>;<b>m     # FG
                    // \033[48;2;<r>;<g>;<b>m     # BG
                }

                std::clog << message;
            }

            // Defaults FG & BG
            std::cout << "\033[39m\033[49m";
        }

        // TODO: set colors methods
        // * user sets ANSI escapes (dictionary or AnsiEscapes class)
        // * before console_print we build the proper codes - AnsiEscapes.compile()

        // TODO: query terminal for ANSI/TrueColor support
        // https://gist.github.com/XVilka/8346728

    };

}

#endif //CONSOLE_LOGGER_HPP
