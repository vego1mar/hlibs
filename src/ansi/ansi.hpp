#ifndef ANSI_HPP
#define ANSI_HPP

#include <string>
#include <string_view>
#include <cinttypes>
#include <regex>


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
            // ANSISequenceCheckType::CSICheck: (SingleByte, TwoBytes, MultiBytes, Any)

            // TODO: range check of 0x20-0x7E → ::strings, std::range_error
            // RangeCheck(str, Range(a,b));
            // RangeCheck(str, RangeCheckType::Letters | Digits | Alphanumerics | Special | ControlChars | ASCII);
            // OrderCheck(str, OrderCheckType::Lexicographical | ASCIbetical | Alphabetical_Unicode);

            // no byte "ESC[ E"
            // single byte "ESC[ $n E"
            // double byte "ESC[ $n; $m E"
            // multi byte "ESC[ $a [;]+ $b [;]+ (…) E"

            const auto seq = std::string(sequence);
            std::smatch csiMatch;

            const std::string singleByte("^\033\\[[0-9]+[a-zA-Z]{1}$");
            const std::regex singleByteRegex(singleByte, std::regex_constants::ECMAScript);
            bool isSingleByte = std::regex_match(seq, csiMatch, singleByteRegex);

            const std::string csiSequence("^\033\\[[0-9]+[a-zA-Z]{1}$");
            const std::regex csiRegex(csiSequence, std::regex_constants::ECMAScript);

            if (std::regex_search(seq, csiMatch, csiRegex)) {
                const auto prefix = csiMatch.prefix();
                const auto str = csiMatch.str();
                const auto suffix = csiMatch.suffix();
                return false;
            }

            // change return clause
            return true;
        }

    };

    // TODO: CSISequencer, SGRSequence -> Sequencer
    // Sequencer.moveCursor(Direction, cells) -> CSISequencer{ CUU, CUD, CUF, CUB }...
    // TODO: range check
    // for SGR → std::string reset() → "ESC[0m"

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

}

#endif //ANSI_HPP
