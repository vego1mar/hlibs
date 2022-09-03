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

#include "../facilities/strings.hpp"
#include "../facilities/timestamp.hpp"
#include "../io/helper_objects.hpp"


namespace hlibs::logging {

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

        std::string str;

        /// ${Header}$Message$NewLine
        Message(Level::Severity level, std::string_view msg, const Source& location)
        {
            auto head = Header(level, location);
            str.append(head);
            str.append(msg);
            str.append("\n");
        }

        /// ${Header}std::exception->$Type | $What | $Message$NewLine
        Message(std::string_view msg, const std::exception& e, const Source& location)
        {
            auto head = Header(Level::Severity::Exception, location);
            auto typeID = std::string(typeid(e).name());
            auto tail = "std::exception->" + typeID + " | what: " + e.what() + " | ";
            str.append(head);
            str.append(tail);
            str.append(msg);
            str.append("\n");
        }

      private:
        static std::string Header(Level::Severity lv, const Source& source)
        {
            static Level severity;
            hlibs::facilities::timestamp::DateTime marker;

            std::string timestamp = marker.timestamp();
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
        }

        void warning(std::string_view msg, Source sl = std::experimental::source_location::current())
        {
            messages.emplace_back(Level::Severity::Warning, msg, sl);
        }

        void fatal(std::string_view msg, Source sl = std::experimental::source_location::current())
        {
            messages.emplace_back(Level::Severity::Fatal, msg, sl);
        }

        void debug(std::string_view msg, Source sl = std::experimental::source_location::current())
        {
            messages.emplace_back(Level::Severity::Debug, msg, sl);
        }

        void exception(std::string_view msg, const std::exception& e, Source sl = std::experimental::source_location::current())
        {
            messages.emplace_back(msg, e, sl);
        }

        /// {"$NumberOfMessages"}
        [[nodiscard]] std::string toString() const noexcept
        {
            std::ostringstream ss;
            ss << '{';
            ss << std::quoted(std::to_string(messages.size()));
            ss << '}';
            return ss.str();
        }

        [[nodiscard]] std::string last() const
        {
            return messages.at(messages.size() - 1).str;
        }

        void last(std::string&& msg)
        {
            messages.at(messages.size() - 1).str = std::move(msg);
        }

        std::vector<Message> messages;
    };


    /// Logs messages to std::cout, std::cerr, or std::clog.
    class StdoutLogger {
      public:
        using Source = std::experimental::source_location;

        void info(std::string_view msg, Source sl = std::experimental::source_location::current())
        {
            sink.info(msg, sl);
            std::cout << sink.last();
        }

        void warning(std::string_view msg, Source sl = std::experimental::source_location::current())
        {
            sink.warning(msg, sl);
            std::cout << sink.last();
        }

        void fatal(std::string_view msg, Source sl = std::experimental::source_location::current())
        {
            sink.fatal(msg, sl);
            std::cerr << sink.last();
        }

        void debug(std::string_view msg, Source sl = std::experimental::source_location::current())
        {
            sink.debug(msg, sl);
            std::cout << sink.last();
        }

        void exception(std::string_view msg, const std::exception& e, Source sl = std::experimental::source_location::current())
        {
            sink.exception(msg, e, sl);
            std::clog << sink.last();
        }

        /// {{"$NumberOfMessages"}}
        [[nodiscard]] std::string toString() const noexcept
        {
            return '{' + sink.toString() + '}';
        }

      private:
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
            current(current());
            std::cout << sink.last();
        }

        /// ${RedFg}$Message${ResetColors}
        void warning(std::string_view msg, Source sl = std::experimental::source_location::current())
        {
            std::string redFg = "\033[38:2::255:0:0m";
            sink.warning(msg, sl);
            current(redFg + current());
            std::cout << sink.last();
        }

        /// ${RedBg}${WhiteFg}$Bold$Message${NormalIntensity}${ResetColors}
        void fatal(std::string_view msg, Source sl = std::experimental::source_location::current())
        {
            std::string redBg = "\033[48:2::255:0:0m";
            std::string whiteFg = "\033[38:2::255:255:255m";
            std::string bold = "\033[1m";
            std::string notBold = "\033[22m";
            sink.fatal(msg, sl);
            current(redBg + whiteFg + bold + current() + notBold);
            std::cerr << sink.last();
        }

        /// $Invert$Message$Revert
        void debug(std::string_view msg, Source sl = std::experimental::source_location::current())
        {
            std::string invert = "\033[7m";
            std::string revert = "\033[27m";
            sink.debug(msg, sl);
            current(invert + current() + revert, false);
            std::cout << sink.last();
        }

        /// ${TealBg}${WhiteFg}$Message${ResetColors}
        void exception(std::string_view msg, const std::exception& e, Source sl = std::experimental::source_location::current())
        {
            std::string tealBg = "\033[48:2::0:128:128m";
            std::string whiteFg = "\033[38:2::255:255:255m";
            sink.exception(msg, e, sl);
            current(tealBg + whiteFg + current());
            std::clog << sink.last();
        }

        /// {{"$NumberOfMessages"}}
        [[nodiscard]] std::string toString() const noexcept
        {
            return '{' + sink.toString() + '}';
        }

      private:
        // TODO: make this compliant with Windows line break
        [[nodiscard]] std::string current() const noexcept
        {
            auto current = sink.last();
            auto noNewline = current.substr(0, current.size() - 1);
            return noNewline;
        }

        /// "${LogMessage}${ColorResetTerminator}${NewLine}"
        void current(std::string&& log, bool shouldResetColors = true) noexcept
        {
            auto enriched = std::move(log);
            if (shouldResetColors) enriched.append("\033[39;49m");
            enriched.append("\n");
            sink.last(std::move(enriched));
        }

        InMemoryLogger sink;
    };


    // TODO: single-line doc
    class FileLogger {
        using Source = std::experimental::source_location;

      public:
        explicit FileLogger(const std::filesystem::path& folder = std::filesystem::current_path())
                : writer(GetFileName(folder), std::ios::app)
        {
        }

        FileLogger() = delete;
        ~FileLogger() noexcept = default;

        FileLogger(const FileLogger& rhs) = delete;
        FileLogger& operator=(const FileLogger& rhs) = delete;

        // TODO: open class for move
        FileLogger(FileLogger&& rhs) noexcept = delete;
        FileLogger& operator=(FileLogger&& rhs) noexcept = delete;

        void info(std::string_view msg, Source sl = std::experimental::source_location::current())
        {
            sink.info(msg, sl);
            writer.write(sink.last());
        }

        /// {{$InMemoryLogger},{$FileWriter}}
        std::string toString() const noexcept
        {
            std::ostringstream ss;
            ss << '{';
            ss << sink.toString() << ',';
            ss << writer.toString();
            ss << '}';
            return ss.str();
        }

      private:
        /// "file_logger_${YYYYmmDD}_${HHMM}.log"
        static std::string GetFileName(const std::filesystem::path& base)
        {
            using facilities::timestamp::DateTime;
            DateTime dt{};
            auto filename = "file_logger_" + dt.filestamp() + ".log";

            if (base.has_filename()) throw std::invalid_argument("base.has_filename()");
            auto filepath = std::filesystem::path(base);
            filepath.append(filename);
            return filepath.string();
        }

        InMemoryLogger sink{};
        hlibs::io::FileWriter writer;
    };

}

#endif //LIBS_LOGGER_HPP
