#ifndef LIBS_LOGGER_HPP
#define LIBS_LOGGER_HPP

#include <string>
#include <sstream>
#include <fstream>
#include <cinttypes>
#include <iosfwd>
#include <unordered_map>
#include <experimental/source_location>
#include <string_view>
#include <vector>
#include <array>
#include <iostream>

#include "../types/types.hpp"
#include "../facilities/strings.hpp"
#include "../facilities/timestamp.hpp"


namespace hlibs::logging {

    struct SeverityLevel {
      public:
        enum class Level : uint8_t {
            Fatal = 1,
            Warning = 2,
            Info = 4,
            Debug = 8,
        };

        static std::string ToString(const Level& level)
        {
            const std::unordered_map<Level, std::string> mapping = {
                    {Level::Fatal,   "Fatal"},
                    {Level::Warning, "Warning"},
                    {Level::Info,    "Info"},
                    {Level::Debug,   "Debug"},
            };

            return mapping.at(level);
        }

        static const Level& Begin() noexcept
        {
            return *all_items.begin();
        }

        static const Level& Next(int position)
        {
            return *(all_items.begin() + position);
        }

        static const Level& End() noexcept
        {
            return *all_items.end();
        }

      private:
        static constexpr std::array<Level, 4> all_items = {
                Level::Fatal, Level::Warning, Level::Info, Level::Debug
        };
    };


    struct LoggerSettings {
        /// Number of messages that will go to buffer before placing them in the str_target output.
        unsigned short messages_before_flush = 11;

        /// On false, prevents processing of logged messages.
        bool is_enabled = true;

        /// Skips printing the messages, but do not prevent from processing them (StdOutLogger/ConsoleLogger).
        bool use_stdout = true;
    };


    class Logger : private hlibs::types::ObjectCounter<Logger> {
      public:
        using SourceLocation = std::experimental::source_location;


        Logger(const Logger& rhs) = delete;

        Logger(Logger&& rhs) noexcept = delete;

        Logger& operator=(const Logger& rhs) = delete;

        Logger& operator=(Logger&& rhs) noexcept = delete;

        void fatal(const std::string& msg, SourceLocation source = SourceLocation::current())
        {
            log(SeverityLevel::Level::Fatal, msg, source);
        }

        void warning(const std::string& msg, SourceLocation source = SourceLocation::current())
        {
            log(SeverityLevel::Level::Warning, msg, source);
        }

        void info(const std::string& msg, SourceLocation source = SourceLocation::current())
        {
            log(SeverityLevel::Level::Info, msg, source);
        }

        void debug(const std::string& msg, SourceLocation source = SourceLocation::current())
        {
            log(SeverityLevel::Level::Debug, msg, source);
        }

        void set(const LoggerSettings& newSettings) noexcept
        {
            settings = newSettings;
        }

        [[nodiscard]] inline const unsigned int& getID() const noexcept
        {
            return id;
        }

      protected:
        Logger()
        {
            id = hlibs::types::ObjectCounter<Logger>::created;
        }

        ~Logger() noexcept override = default;

        virtual void log(SeverityLevel::Level level, const std::string& msg, SourceLocation source) = 0;

        inline LoggerSettings& elicitSettings() noexcept
        {
            return settings;
        }

      private:
        LoggerSettings settings{};
        unsigned int id = 0;
    };


    // TODO: new approach

    struct Level final {
      public:
        enum class Severity : uint8_t {
            Exception = 0,
            Fatal = 1,
            Warning = 2,
            Info = 4,
            Debug = 8,
        };

        std::string toString(Severity level) const noexcept
        {
            return items.at(level);
        }

      private:
        std::unordered_map<Severity, std::string> items = {
                {Severity::Exception, "Exception"},
                {Severity::Fatal,     "Fatal"},
                {Severity::Warning,   "Warning"},
                {Severity::Info,      "Info"},
                {Severity::Debug,     "Debug"}
        };
    };


    struct Message final {
        using Source = std::experimental::source_location;

        std::string message;

        /// ${Header}$Message$NewLine
        Message(Level::Severity level, std::string_view msg, const Source& location)
        {
            auto head = Header(level, location);
            message.append(head);
            message.append(msg);
            message.append("\n");
        }

        /// ${Header}std::exception->$Type | $What | $Message$NewLine
        Message(std::string_view msg, const std::exception& e, const Source& location)
        {
            auto head = Header(Level::Severity::Exception, location);
            auto typeID = std::string(typeid(e).name());
            auto tail = "std::exception->" + typeID + " | what: " + e.what() + " | ";
            message.append(head);
            message.append(tail);
            message.append(msg);
            message.append("\n");
        }

      private:
        static std::string Header(Level::Severity lv, const Source& source)
        {
            static Level severity;
            std::string timestamp = hlibs::facilities::timestamp::GetDateAndTime();
            std::string level = hlibs::facilities::strings::ToUpperCase(severity.toString(lv));
            std::string path = source.file_name();
            std::string file = path.substr(path.find_last_of("/\\") + 1);
            std::string line = std::to_string(source.line());
            std::string function = source.function_name();

            // $Date $Time [$Level] $SourceFile($Line) @"$FunctionPrototype": ${Message}
            std::string header{};
            header.append(timestamp + " [" + level + "] ");
            header.append(file + '(' + line + ") @\"" + function + "\": ");
            return header;
        }
    };


    struct InMemoryLogger {
        using Source = std::experimental::source_location;

        void info(std::string_view msg, Source sl = std::experimental::source_location::current())
        {
            messages.emplace_back(Level::Severity::Info, msg, sl);
            ++current_msg;
        }

        void warning(std::string_view msg, Source sl = std::experimental::source_location::current())
        {
            messages.emplace_back(Level::Severity::Warning, msg, sl);
            ++current_msg;
        }

        void fatal(std::string_view msg, Source sl = std::experimental::source_location::current())
        {
            messages.emplace_back(Level::Severity::Fatal, msg, sl);
            ++current_msg;
        }

        void debug(std::string_view msg, Source sl = std::experimental::source_location::current())
        {
            messages.emplace_back(Level::Severity::Debug, msg, sl);
            ++current_msg;
        }

        void exception(std::string_view msg, const std::exception& e, Source sl = std::experimental::source_location::current())
        {
            messages.emplace_back(msg, e, sl);
            ++current_msg;
        }

        std::vector<Message> messages;
        std::size_t current_msg = 0;
    };


    /// Logs messages to std::cout, std::cerr, or std::clog.
    class StdoutLogger {
      public:
        using Source = std::experimental::source_location;

        void info(std::string_view msg, Source sl = std::experimental::source_location::current())
        {
            sink.info(msg, sl);
            insert();
        }

        void warning(std::string_view msg, Source sl = std::experimental::source_location::current())
        {
            sink.warning(msg, sl);
            insert();
        }

        void fatal(std::string_view msg, Source sl = std::experimental::source_location::current())
        {
            sink.fatal(msg, sl);
            insert(std::cerr);
        }

        void debug(std::string_view msg, Source sl = std::experimental::source_location::current())
        {
            sink.debug(msg, sl);
            insert();
        }

        void exception(std::string_view msg, const std::exception& e, Source sl = std::experimental::source_location::current())
        {
            sink.exception(msg, e, sl);
            insert(std::clog);
        }

      private:
        void insert(std::ostream& stream = std::cout)
        {
            auto current = (sink.current_msg == 0) ? 0 : (sink.current_msg - 1);
            stream << sink.messages.at(current).message;
        }

        InMemoryLogger sink;
    };


    /// Uses ANSI escape characters (SGR sequences of ECMA-48) to print colourful output.
    /// url:https://www.ecma-international.org/publications-and-standards/standards/ecma-48/
    class TerminalLogger {
      public:
        using Source = std::experimental::source_location;

        /// $Message${ColorsReset}
        void info(std::string_view msg, Source sl = std::experimental::source_location::current())
        {
            sink.info(msg, sl);
            currentLog(currentLog() + reset_colors + '\n');
            print();
        }

        /// ${RedFg}$Message${ResetColors}
        void warning(std::string_view msg, Source sl = std::experimental::source_location::current())
        {
            std::string redFg = "\033[38:2::255:0:0m";
            sink.warning(msg, sl);
            currentLog(redFg + currentLog() + reset_colors + '\n');
            print();
        }

        /// ${RedBg}${WhiteFg}$Bold$Message${NormalIntensity}${ResetColors}
        void fatal(std::string_view msg, Source sl = std::experimental::source_location::current())
        {
            std::string redBg = "\033[48:2::255:0:0m";
            std::string whiteFg = "\033[38:2::255:255:255m";
            std::string bold = "\033[1m";
            std::string notBold = "\033[22m";
            sink.fatal(msg, sl);
            currentLog(redBg + whiteFg + bold + currentLog() + notBold + reset_colors + '\n');
            print(std::cerr);
        }

        /// $Invert$Message$Revert
        void debug(std::string_view msg, Source sl = std::experimental::source_location::current())
        {
            std::string invert = "\033[7m";
            std::string revert = "\033[27m";
            sink.debug(msg, sl);
            currentLog(invert + currentLog() + revert + '\n');
            print();
        }

        /// ${TealBg}${WhiteFg}$Message${ResetColors}
        void exception(std::string_view msg, const std::exception& e, Source sl = std::experimental::source_location::current())
        {
            std::string tealBg = "\033[48:2::0:128:128m";
            std::string whiteFg = "\033[38:2::255:255:255m";
            sink.exception(msg, e, sl);
            currentLog(tealBg + whiteFg + currentLog() + reset_colors + '\n');
            print(std::clog);
        }

      private:
        void print(std::ostream& stream = std::cout)
        {
            auto&& current = (sink.current_msg == 0) ? 0 : (sink.current_msg - 1);
            stream << sink.messages.at(current).message;
        }

        [[nodiscard]] std::string currentLog() const noexcept
        {
            auto& current = sink.messages.at(sink.current_msg - 1).message;
            auto noNewline = current.substr(0, current.size() - 1);
            return noNewline;
        }

        void currentLog(std::string&& log) noexcept
        {
            sink.messages.at(sink.current_msg - 1).message = std::move(log);
        }

        InMemoryLogger sink;
        std::string reset_colors = "\033[39;49m";
    };

}

#endif //LIBS_LOGGER_HPP
