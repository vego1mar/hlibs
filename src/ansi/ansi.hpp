#ifndef ANSI_HPP
#define ANSI_HPP

#include <string>
#include <string_view>
#include <cinttypes>
#include <regex>
#include <array>
#include <tuple>

#include "../templates/range.hpp"
#include "../strings/strings.hpp"
#include "../types.hpp"


namespace ansi {

    /// Control Sequence Introducer.
    class CSI {
    public:
        enum class EscapeType : bool {
            Octal,                       ///< "\033["
            Hexadecimal                  ///< "\\x1B["
        };

    private:
        EscapeType type = EscapeType::Octal;
        std::string csi;

    public:
        explicit CSI(EscapeType charType = EscapeType::Octal) {
            set(charType);
        }

        void set(EscapeType charType) noexcept {
            csi.clear();

            if (charType == EscapeType::Hexadecimal) {
                csi.append("\x1B[", 2);
                return;
            }

            csi.append("\033[", 2);
        }

        inline std::string operator()() const noexcept {
            return csi;
        }

        [[nodiscard]] inline std::string toString() const noexcept {
            return type == EscapeType::Octal ? R"(\033[)" : R"(\x1B[)";
        }

    };


    struct Enclose {
        enum CSIEndChar : char {
            CUU = 'A',                  ///< Cursor Up
            CUD = 'B',                  ///< Cursor Down
            CUF = 'C',                  ///< Cursor Forward
            CUB = 'D',                  ///< Cursor Backward
            CNL = 'E',                  ///< Cursor Next Line
            CPL = 'F',                  ///< Cursor Previous Line
            CHA = 'G',                  ///< Cursor Horizontal Absolute
            CUP = 'H',                  ///< Cursor Position
            EL = 'K',                   ///< Erase in Line
            ED = 'J',                   ///< Erase in Display
            SU = 'S',                   ///< Scroll Up
            SD = 'T',                   ///< Scroll Down
            HVP = 'f',                  ///< Horizontal Vertical Position
            SGR = 'm',                  ///< Select Graphic Rendition
            SCP = 's',                  ///< Save Current Cursor Position (SCOSC)
            RCP = 'u',                  ///< Restore Saved Cursor Position (SCORC)
        };

        CSIEndChar enclose = CSIEndChar::SGR;

        explicit Enclose(CSIEndChar charType = CSIEndChar::SGR) : enclose(charType) {
        }

        inline char operator()() const noexcept {
            return static_cast<char>(enclose);
        }

    };


    /// Produces CSI control sequences "ESC[" except SGR sequences.
    class CSISequencer {
    public:
        struct ParamType {
            enum class EDType : uint8_t {
                ToEndOfScreen = 0,              ///< Clear from cursor to end of screen.
                ToBeginningOfTheScreen = 1,     ///< Clear from cursor to beginning of the screen.
                EntireScreen = 2,               ///< Clear entire screen.
                EntireScreenWithBuffer = 3,     ///< Clear entire screen and delete all lines saved in the scrollback buffer.
            };

            enum class ELType : uint8_t {
                ToEndOfLine = 0,                ///< Clear from cursor to the end of the line.
                ToBeginningOfLine = 1,          ///< Clear from cursor to beginning of the line.
                EntireLine = 2,                 ///< Clear entire line.
            };

            inline static std::string ToString(EDType type) noexcept {
                return std::to_string(static_cast<uint8_t>(type));
            }

            inline static std::string ToString(ELType type) noexcept {
                return std::to_string(static_cast<uint8_t>(type));
            }

        };

    private:
        CSI csi{CSI::EscapeType::Octal};

    public:
        explicit CSISequencer(CSI::EscapeType charType = CSI::EscapeType::Octal) {
            csi.set(charType);
        }

        /// Cursor Up (editor function). Moves the cursor n $cells upward.
        [[nodiscard]] std::string getCUU(std::size_t cells = 1U) const noexcept {
            // "\033[ $n A"
            const char endChar = Enclose{Enclose::CUU}();
            return csi() + std::to_string(cells) + endChar;
        }

        /// Cursor Down (editor function). Moves the cursor n $cells down.
        [[nodiscard]] std::string getCUD(std::size_t cells = 1U) const noexcept {
            // "\033[ $n B"
            const char endChar = Enclose{Enclose::CUD}();
            return csi() + std::to_string(cells) + endChar;;
        }

        /// Cursor Forward (editor function). Moves the cursor n $cells forward.
        [[nodiscard]] std::string getCUF(std::size_t cells = 1U) const noexcept {
            // "\033[ $n C"
            const char endChar = Enclose{Enclose::CUF}();
            return csi() + std::to_string(cells) + endChar;
        }

        /// Cursor Back (editor function). Moves the cursor n $cells backward.
        [[nodiscard]] std::string getCUB(std::size_t cells = 1U) const noexcept {
            // "\033[ $n D"
            const char endChar = Enclose{Enclose::CUB}();
            return csi() + std::to_string(cells) + endChar;
        }

        /// Cursor Next Line (editor function). Moves cursor to beginning of the line $cells lines down.
        [[nodiscard]] std::string getCNL(std::size_t cells = 1U) const noexcept {
            // "\033[ $n E"
            const char endChar = Enclose{Enclose::CNL}();
            return csi() + std::to_string(cells) + endChar;
        }

        /// Cursor Previous Line (editor function). Moves cursor to beginning of the line $cells lines up.
        [[nodiscard]] std::string getCPL(std::size_t cells = 1U) const noexcept {
            // "\033[ $n F"
            const char endChar = Enclose{Enclose::CPL}();
            return csi() + std::to_string(cells) + endChar;
        }

        /// Cursor Horizontal Absolute (editor function). Moves the cursor to column $n.
        [[nodiscard]] std::string getCHA(std::size_t n = 1U) const noexcept {
            // "\033[ $n G"
            const char endChar = Enclose(Enclose::CHA)();
            return csi() + std::to_string(n) + endChar;
        }

        /// Cursor Position (editor function). Moves the cursor to ($row, $column).
        [[nodiscard]] std::string getCUP(std::size_t row = 1U, std::size_t column = 1U) const noexcept {
            // "ESC[ $n ; $m H"
            const char endChar = Enclose(Enclose::CUP)();
            return csi() + std::to_string(row) + ';' + std::to_string(column) + endChar;
        }

        /// Horizontal Vertical Position (format effector function). Moves the cursor to ($row, $column).
        [[nodiscard]] std::string getHVP(std::size_t row = 1U, std::size_t column = 1U) const noexcept {
            // "ESC[ $n ; $m f"
            const char endChar = Enclose(Enclose::HVP)();
            return csi() + std::to_string(row) + ';' + std::to_string(column) + endChar;
        }

        /// Scroll Up (editor function). Scroll whole page up by n $lines.
        [[nodiscard]] std::string getSU(std::size_t lines = 1U) const noexcept {
            // "ESC[ $n S"
            const char endChar = Enclose(Enclose::SU)();
            return csi() + std::to_string(lines) + endChar;
        }

        /// Scroll Down (editor function). Scroll whole page down by n $lines.
        [[nodiscard]] std::string getSD(std::size_t lines = 1U) const noexcept {
            // "ESC[ $n T"
            const char endChar = Enclose(Enclose::SD)();
            return csi() + std::to_string(lines) + endChar;
        }

        /// Erase in Display (editor function). Clears part of the screen.
        [[nodiscard]] std::string getED(ParamType::EDType type = ParamType::EDType::ToEndOfScreen) const noexcept {
            // "ESC[ $n J"
            const char endChar = Enclose(Enclose::ED)();
            return csi() + ParamType::ToString(type) + endChar;
        }

        /// Erase in Line (editor function). Erases part of the line (do not change cursor position).
        [[nodiscard]] std::string getEL(ParamType::ELType type = ParamType::ELType::ToEndOfLine) const noexcept {
            // "ESC[ $n K"
            const char endChar = Enclose(Enclose::EL)();
            return csi() + ParamType::ToString(type) + endChar;
        }

        /// Save Current Cursor Position (private sequence). Saves the cursor state in SCO console mode.
        [[nodiscard]] std::string getSCP() const noexcept {
            // "ESC[s"
            const char endChar = Enclose(Enclose::SCP)();
            return csi() + endChar;
        }

        /// Restore Saved Cursor Position (private sequence). Restores the cursor state in SCO console mode.
        [[nodiscard]] std::string getRCP() const noexcept {
            // "ESC[u"
            const char endChar = Enclose(Enclose::RCP)();
            return csi() + endChar;
        }

        /// Check if custom ANSI CSI escape sequence is well-formed.
        static bool IsValid(const std::string_view sequence) {
            if (sequence.empty()) {
                return false;
            }

            const auto seq = std::string(sequence);
            const std::string sequenceWithNoEscKey = seq.substr(1);
            templates::Range<char> range{0x20, 0x7E};
            const auto isInANSIRange = strings::CheckRange(sequenceWithNoEscKey, range);

            if (!isInANSIRange) {
                return false;
            }

            const bool isMatchedSequence = IsMatchFound(seq);
            return isMatchedSequence;
        }

    private:
        static bool IsMatchFound(const std::string& seq) {
            const std::size_t CHECKS = 4U;
            const auto grammar = std::regex_constants::ECMAScript;
            std::smatch csiMatch;

            const std::array<std::string, CHECKS> patterns = {
                    std::string("^\033\\[[a-zA-Z]{1}$"),
                    std::string("^\033\\[[0-9]+[a-zA-Z]{1}$"),
                    std::string("^\033\\[[0-9]+;[0-9]+[a-zA-Z]{1}$"),
                    std::string("^\033\\[([0-9]+(;)+)+[0-9]+[a-zA-Z]{1}$"),
            };

            const std::array<std::regex, CHECKS> regexes = {
                    std::regex(*(patterns.begin() + 0), grammar),              // no byte
                    std::regex(*(patterns.begin() + 1), grammar),              // single byte
                    std::regex(*(patterns.begin() + 2), grammar),              // double byte
                    std::regex(*(patterns.begin() + 3), grammar),              // multi byte
            };

            const std::array<bool, CHECKS> matches = {
                    std::regex_match(seq, csiMatch, *(regexes.begin() + 0)),   // "ESC[ E"
                    std::regex_match(seq, csiMatch, *(regexes.begin() + 1)),   // "ESC[ $n E"
                    std::regex_match(seq, csiMatch, *(regexes.begin() + 2)),   // "ESC[ $n; $m E"
                    std::regex_match(seq, csiMatch, *(regexes.begin() + 3)),   // "ESC[ $a[;]+ … $z[;]+  E"
            };

            bool isNoByte = *matches.begin();
            bool isSingleByte = *(matches.begin() + 1);
            bool isDoubleByte = *(matches.begin() + 2);
            bool isMultiByte = *(matches.begin() + 3);
            bool isFound = isNoByte || isSingleByte || isDoubleByte || isMultiByte;
            return isFound;
        }

    };

    struct DisplayAttribute {
        enum class SGRDisplayAttribute : uint8_t {
            Reset = 0,                            ///< or normal
            Bold = 1,                             ///< or increased intensity
            Faint = 2,                            ///< or decreased intensity
            Italic = 3,
            NormalIntensity = 22,                 ///< neither bold nor faint
            Underline = 4,
            DoublyUnderlined = 21,                ///< or not bold
            NotUnderlined = 24,                   ///< neither singly or doubly underlined
            Overlined = 53,
            NotOverlined = 55,
            Strikeout = 9,                        ///< or cross out
            NotStrikeout = 29,                    ///< or not crossed out
            Blink = 5,                            ///< or slow blink
            NotBlinking = 25,
            Invert = 7,                           ///< or reverse video (swaps foreground and background colors)
            NotInverted = 27,                     ///< or not reversed
            Conceal = 8,                          ///< or hide
            Reveal = 28,                          ///< or not concealed
            /*
            Handled by standalone functions:
            ForegroundColor = 38,                 // followed by "5;n" or "2;r;g;b"
            BackgroundColor = 48,                 // followed by "5;n" or "2;r;g;b"
            */
        };

        inline static std::string ToString(SGRDisplayAttribute attr) noexcept {
            return std::to_string(static_cast<uint8_t>(attr));
        }

    };


    struct SGRNamedColor {
        enum class Foreground : uint8_t {
            Black = 30,
            Red = 31,
            Green = 32,
            Yellow = 33,
            Blue = 34,
            Magenta = 35,
            Cyan = 36,
            White = 37,
            Default = 39,
            Gray = 90,
            BrightRed = 91,
            BrightGreen = 92,
            BrightYellow = 93,
            BrightBlue = 94,
            BrightMagenta = 95,
            BrightCyan = 96,
            BrightWhite = 97
        };

        enum class Background : uint8_t {
            Black = 40,
            Red = 41,
            Green = 42,
            Yellow = 43,
            Blue = 44,
            Magenta = 45,
            Cyan = 46,
            White = 47,
            Default = 49,
            Gray = 100,
            BrightRed = 101,
            BrightGreen = 102,
            BrightYellow = 103,
            BrightBlue = 104,
            BrightMagenta = 105,
            BrightCyan = 106,
            BrightWhite = 107
        };

        inline static std::string ToString(Foreground fg) noexcept {
            return std::to_string(static_cast<uint8_t>(fg));
        }

        inline static std::string ToString(Background bg) noexcept {
            return std::to_string(static_cast<uint8_t>(bg));
        }

    };


    class SGRSequencer {
    private:
        const char endChar = Enclose{Enclose::SGR}();
        CSI csi{CSI::EscapeType::Octal};

    public:
        /// Sets only one display attribute.
        [[nodiscard]] std::string getDisplayAttribute(DisplayAttribute::SGRDisplayAttribute param) const noexcept {
            // "ESC[ $n m"
            return csi() + DisplayAttribute::ToString(param) + endChar;
        }

        /// Sets foreground and background color to defaults.
        [[nodiscard]] std::string getColorsReset() const noexcept {
            // "ESC[39;49m"
            return csi() + "39;49" + endChar;
        }

        /// Sets 4-bit color (16 colors mode).
        [[nodiscard]] std::string getColor(SGRNamedColor::Foreground fg, SGRNamedColor::Background bg) const noexcept {
            // "ESC[ $n;$m m"
            return csi() + SGRNamedColor::ToString(fg) + ';' + SGRNamedColor::ToString(bg) + endChar;
        }

        /// Sets 8-bit color (256 colors mode).
        [[nodiscard]] std::string getColor(uint8_t color) const noexcept {
            // "ESC[38;5; $n m"
            return csi() + "38;5;" + std::to_string(color) + endChar;
        }

        /// Sets 24-bit foreground color (true color).
        [[nodiscard]] std::string getForegroundColor(libs::types::RGBColor color, bool useColons = true) const noexcept {
            // "ESC[38;2; $r;$g;$b m" or "ESC[38:2:: $r:$g:$b m"
            const auto groundColors = getGroundColor(color, true);
            return useColons ? std::get<0>(groundColors) : std::get<1>(groundColors);
        }

        /// Sets 24-bit background color (true color).
        [[nodiscard]] std::string getBackgroundColor(libs::types::RGBColor color, bool useColons = true) const noexcept {
            // "ESC[48;2; $r;$g;$b m" or "ESC[48:2:: $r:$g:$b m"
            const auto groundColors = getGroundColor(color, false);
            return useColons ? std::get<0>(groundColors) : std::get<1>(groundColors);
        }

    private:
        using GroundColor = std::tuple<std::string, std::string>;

        [[nodiscard]] GroundColor getGroundColor(libs::types::RGBColor color, bool isFG = true) const noexcept {
            const std::string r = std::to_string(color.red);
            const std::string g = std::to_string(color.green);
            const std::string b = std::to_string(color.blue);
            const std::string code = isFG ? "38" : "48";
            const std::string withColons = csi() + code + ":2::" + r + ':' + g + ':' + b + endChar;
            const std::string withSemicolons = csi() + code + ";2;" + r + ';' + g + ';' + b + endChar;
            return std::make_tuple<>(withColons, withSemicolons);
        }

    };


    // TODO: SGR sequence check in SGRSequencer
    // 04-bit color "ESC[ $n m"                                     → single byte with 'm' ending (one param)
    //  4-bit color "ESC[ $n;$m m"                                  → double byte with 'm' ending (two params)
    //  attributes  "ESC[ $a; $b; … ;$z m"                          → multi byte with 'm' ending (many params)
    //  8-bit color "ESC[ [38|48];5; $n m"                          → 8-bit color only
    //  8-bit color "ESC[ [$a;]* [38|48];5; $n [;$z]* m"            → 8-bit color with other params
    // 24-bit color "ESC[ [38|48];2; $r;$g;$g m"                    → true color only
    // 24-bit color "ESC[ [38|48]:2::$r:$g:$g m"                    → true color only (colons)
    // 24-bit color "ESC[ [$a;]* [38|48];2; $r;$g;$g [;$z]* m"      → true color with other params
    // 24-bit color "ESC[ [$a;]* [38|48]:2::$r:$g:$g [;$z]* m"      → true color with other params (colons)


    class ANSISequencer {
    private:
        CSISequencer csiSequencer{};
        SGRSequencer sgrSequencer{};

    public:
        // TODO: more meaningful names from both sequencers
        // std::string moveCursor() const noexcept;
    };

}

#endif //ANSI_HPP
