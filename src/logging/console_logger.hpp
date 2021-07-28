#ifndef CONSOLE_LOGGER_HPP
#define CONSOLE_LOGGER_HPP

#include <iostream>

#include "stdout_logger.hpp"


namespace logging {

    /// Uses ANSI escape characters to print colourful output.
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
                    std::cout << "\033[38:2::0:128:128m" << message << "\033[0m";
                    continue;
                }

                std::clog << message;
            }

            // Defaults FG & BG
            std::cout << "\033[39m\033[49m";
        }

        // TODO: set colors methods, use ::ansi facilities
        // TODO: query terminal for ANSI/TrueColor support
        // https://gist.github.com/XVilka/8346728

    };

}

#endif //CONSOLE_LOGGER_HPP
