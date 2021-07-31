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

        enum FsEndChar : char {
            RIS = 'c',                  ///< Reset to Initial State
        };

        enum FpEndChar : char {
            DECSC = '7',                ///< DEC Save Cursor
            DECRC = '8',                ///< DEC Restore Cursor
        };

        inline static char ToChar(CSIEndChar endChar) noexcept {
            return static_cast<char>(endChar);
        }

        inline static char ToChar(FsEndChar endChar) noexcept {
            return static_cast<char>(endChar);
        }

        inline static char ToChar(FpEndChar endChar) noexcept {
            return static_cast<char>(endChar);
        }

    };


    /// Produces CSI control sequences "ESC[" (Control Sequence Introducer) except SGR sequences (Fe sequences).
    struct CSISequencer {
        const std::string CSI = std::string("\033[", 2);

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

        /// Cursor Up (editor function). Moves the cursor n $cells upward.
        [[nodiscard]] std::string getCUU(std::size_t cells = 1U) const noexcept {
            // "\033[ $n A"
            const char endChar = Enclose::ToChar(Enclose::CUU);
            return CSI + std::to_string(cells) + endChar;
        }

        /// Cursor Down (editor function). Moves the cursor n $cells down.
        [[nodiscard]] std::string getCUD(std::size_t cells = 1U) const noexcept {
            // "\033[ $n B"
            const char endChar = Enclose::ToChar(Enclose::CUD);
            return CSI + std::to_string(cells) + endChar;
        }

        /// Cursor Forward (editor function). Moves the cursor n $cells forward.
        [[nodiscard]] std::string getCUF(std::size_t cells = 1U) const noexcept {
            // "\033[ $n C"
            const char endChar = Enclose::ToChar(Enclose::CUF);
            return CSI + std::to_string(cells) + endChar;
        }

        /// Cursor Back (editor function). Moves the cursor n $cells backward.
        [[nodiscard]] std::string getCUB(std::size_t cells = 1U) const noexcept {
            // "\033[ $n D"
            const char endChar = Enclose::ToChar(Enclose::CUB);
            return CSI + std::to_string(cells) + endChar;
        }

        /// Cursor Next Line (editor function). Moves cursor to beginning of the line $cells lines down.
        [[nodiscard]] std::string getCNL(std::size_t cells = 1U) const noexcept {
            // "\033[ $n E"
            const char endChar = Enclose::ToChar(Enclose::CNL);
            return CSI + std::to_string(cells) + endChar;
        }

        /// Cursor Previous Line (editor function). Moves cursor to beginning of the line $cells lines up.
        [[nodiscard]] std::string getCPL(std::size_t cells = 1U) const noexcept {
            // "\033[ $n F"
            const char endChar = Enclose::ToChar(Enclose::CPL);
            return CSI + std::to_string(cells) + endChar;
        }

        /// Cursor Horizontal Absolute (editor function). Moves the cursor to column $n.
        [[nodiscard]] std::string getCHA(std::size_t n = 1U) const noexcept {
            // "\033[ $n G"
            const char endChar = Enclose::ToChar(Enclose::CHA);
            return CSI + std::to_string(n) + endChar;
        }

        /// Cursor Position (editor function). Moves the cursor to ($row, $column).
        [[nodiscard]] std::string getCUP(std::size_t row = 1U, std::size_t column = 1U) const noexcept {
            // "ESC[ $n ; $m H"
            const char endChar = Enclose::ToChar(Enclose::CUP);
            return CSI + std::to_string(row) + ';' + std::to_string(column) + endChar;
        }

        /// Horizontal Vertical Position (format effector function). Moves the cursor to ($row, $column).
        [[nodiscard]] std::string getHVP(std::size_t row = 1U, std::size_t column = 1U) const noexcept {
            // "ESC[ $n ; $m f"
            const char endChar = Enclose::ToChar(Enclose::HVP);
            return CSI + std::to_string(row) + ';' + std::to_string(column) + endChar;
        }

        /// Scroll Up (editor function). Scroll whole page up by n $lines.
        [[nodiscard]] std::string getSU(std::size_t lines = 1U) const noexcept {
            // "ESC[ $n S"
            const char endChar = Enclose::ToChar(Enclose::SU);
            return CSI + std::to_string(lines) + endChar;
        }

        /// Scroll Down (editor function). Scroll whole page down by n $lines.
        [[nodiscard]] std::string getSD(std::size_t lines = 1U) const noexcept {
            // "ESC[ $n T"
            const char endChar = Enclose::ToChar(Enclose::SD);
            return CSI + std::to_string(lines) + endChar;
        }

        /// Erase in Display (editor function). Clears part of the screen.
        [[nodiscard]] std::string getED(ParamType::EDType type = ParamType::EDType::ToEndOfScreen) const noexcept {
            // "ESC[ $n J"
            const char endChar = Enclose::ToChar(Enclose::ED);
            return CSI + ParamType::ToString(type) + endChar;
        }

        /// Erase in Line (editor function). Erases part of the line (do not change cursor position).
        [[nodiscard]] std::string getEL(ParamType::ELType type = ParamType::ELType::ToEndOfLine) const noexcept {
            // "ESC[ $n K"
            const char endChar = Enclose::ToChar(Enclose::EL);
            return CSI + ParamType::ToString(type) + endChar;
        }

        /// Save Current Cursor Position (private sequence). Saves the cursor state in SCO console mode.
        [[nodiscard]] std::string getSCP() const noexcept {
            // "ESC[s"
            const char endChar = Enclose::ToChar(Enclose::SCP);
            return CSI + endChar;
        }

        /// Restore Saved Cursor Position (private sequence). Restores the cursor state in SCO console mode.
        [[nodiscard]] std::string getRCP() const noexcept {
            // "ESC[u"
            const char endChar = Enclose::ToChar(Enclose::RCP);
            return CSI + endChar;
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


    /// Produces Select Graphics Rendition control sequences (Fe sequences).
    struct SGRSequencer {
    private:
        const char END_CHAR = Enclose::ToChar(Enclose::SGR);
        const std::string FG = std::string("38", 2);
        const std::string BG = std::string("48", 2);
        const std::string CSI = std::string("\033[", 2);

    public:
        enum class ColorType : bool {
            Foreground,
            Background
        };

        /// Sets only one display attribute.
        [[nodiscard]] std::string getDisplayAttribute(DisplayAttribute::SGRDisplayAttribute param) const noexcept {
            // "ESC[ $n m"
            return CSI + DisplayAttribute::ToString(param) + END_CHAR;
        }

        /// Sets foreground and background colors to defaults.
        [[nodiscard]] std::string getColorsReset() const noexcept {
            // "ESC[39;49m"
            return CSI + "39;49" + END_CHAR;
        }

        /// Sets 4-bit color (16 colors mode).
        [[nodiscard]] std::string getColor(SGRNamedColor::Foreground fg, SGRNamedColor::Background bg) const noexcept {
            // "ESC[ $n;$m m"
            return CSI + SGRNamedColor::ToString(fg) + ';' + SGRNamedColor::ToString(bg) + END_CHAR;
        }

        /// Sets 8-bit foreground or background color (256 colors mode).
        /// @details 0-7 — standard colors ("ESC[ 30-37 m")
        /// @details 8-15 — high intensity colors ("ESC[ 90-97 m")
        /// @details 16-231 — 6x6x6 color cube: 16 + 36r + 6g + b, 0 ≤ r,g,b ≤ 5
        /// @details 232-255 — grayscale (from black to white in 24 steps)
        [[nodiscard]] std::string getColor(uint8_t color, ColorType type, bool useColons = true) const noexcept {
            // "ESC[38;5; $n m" or "ESC[38:5: $n m"
            const auto& code = static_cast<bool>(type) ? FG : BG;
            const auto withColons = CSI + code + ":5:" + std::to_string(color) + END_CHAR;
            const auto withSemiColons = CSI + code + ";5;" + std::to_string(color) + END_CHAR;
            return useColons ? withColons : withSemiColons;
        }

        /// Sets 24-bit foreground or background color (true color).
        /// @details 2 — red-green-blue format - "ESC[g:2::r:g:bm"
        [[nodiscard]] std::string getColor(libs::types::RGBColor color, ColorType type, bool useColons = true) const noexcept {
            const auto fg = getGroundColor(color, true);
            const auto bg = getGroundColor(color, false);

            if (type == ColorType::Foreground) {
                // "ESC[38;2; $r;$g;$b m" or "ESC[38:2:: $r:$g:$b m"
                return useColons ? std::get<0>(fg) : std::get<1>(fg);
            }

            // "ESC[48;2; $r;$g;$b m" or "ESC[48:2:: $r:$g:$b m"
            return useColons ? std::get<0>(bg) : std::get<1>(bg);
        }

        /// Check if custom ANSI SGR escape sequence is well-formed.
        static bool IsValid(const std::string_view sequence) {
            // TODO: implement
            // 04-bit color "ESC[ $n m"                                     → single byte with 'm' ending (one param)
            //  4-bit color "ESC[ $n;$m m"                                  → double byte with 'm' ending (two params)
            //  attributes  "ESC[ $a; $b; … ;$z m"                          → multi byte with 'm' ending (many params)
            //  8-bit color "ESC[ [38|48];5; $n m"                          → 8-bit color only
            //  8-bit color "ESC[ [$a;]* [38|48];5; $n [;$z]* m"            → 8-bit color with other params
            // 24-bit color "ESC[ [38|48];2; $r;$g;$g m"                    → true color only
            // 24-bit color "ESC[ [38|48]:2::$r:$g:$g m"                    → true color only (colons)
            // 24-bit color "ESC[ [$a;]* [38|48];2; $r;$g;$g [;$z]* m"      → true color with other params
            // 24-bit color "ESC[ [$a;]* [38|48]:2::$r:$g:$g [;$z]* m"      → true color with other params (colons)
            return false;
        }

    private:
        using GroundColor = std::tuple<std::string, std::string>;

        [[nodiscard]] GroundColor getGroundColor(libs::types::RGBColor color, bool isFG = true) const noexcept {
            const std::string r = std::to_string(color.red);
            const std::string g = std::to_string(color.green);
            const std::string b = std::to_string(color.blue);
            const std::string& code = isFG ? FG : BG;
            const std::string withColons = CSI + code + ":2::" + r + ':' + g + ':' + b + END_CHAR;
            const std::string withSemicolons = CSI + code + ";2;" + r + ';' + g + ';' + b + END_CHAR;
            return std::make_tuple<>(withColons, withSemicolons);
        }

    };


    /// Produces 'independent control functions' Fs sequences.
    struct FsSequencer {
        const std::string ESC = std::string("\033", 1);

        /// Triggers a full reset of the terminal to its original state.
        [[nodiscard]] std::string getRIS() const noexcept {
            // "ESC c"
            return ESC + Enclose::ToChar(Enclose::RIS);
        }

        // TODO: isValid
        // followed by a byte in the range( 0x60-0x7E ) -> ( `a-z{|}~ ), single byted
    };


    /// Produces Fp sequences, that were set apart for private-use control functions.
    struct FpSequencer {
        const std::string ESC = std::string("\033", 1);

        /// Saves the cursor position, encoding shift state and formatting attributes.
        [[nodiscard]] std::string getDECSC() const noexcept {
            // "ESC 7"
            return ESC + Enclose::ToChar(Enclose::DECSC);
        }

        /// Restores the cursor position with settings saved by DECSC, otherwise resets them all to their defaults.
        [[nodiscard]] std::string getDECRC() const noexcept {
            // "ESC 8"
            return ESC + Enclose::ToChar(Enclose::DECRC);
        }

    };


    class ANSISequencer {
        CSISequencer csi{};
        SGRSequencer sgr{};
        FsSequencer fs{};
        FpSequencer fp{};

    public:
        // TODO: more meaningful names from both sequencers
        /*
         moveCursor(std::size_t cells = 1U) const noexcept
         moveCursor(std::size_t row = 1U, std::size_t column = 1U)
         saveCursor(type = FeSeq_CSIPrivate | FpSequence)  // Fe::CSI::Private or Fp::DEC
         restoreCursor(type = FeSeq_CSIPrivate | FpSequence)
         scrollPage(std::size_t lines = 1U)
         getDisplayAttribute(DisplayAttribute::SGRDisplayAttribute param)
         getColor(SGRNamedColor::Foreground fg, SGRNamedColor::Background bg)
         getColor(uint8_t color, ColorType type, bool useColons = true)
         getColor(libs::types::RGBColor color, ColorType type, bool useColons = true)
         resetDisplay()  // SGR, all attributes
         resetColors()   // SGR, FG & BG
         resetConsole()  // Fs::RIS
        */

    };

}

#endif //ANSI_HPP
