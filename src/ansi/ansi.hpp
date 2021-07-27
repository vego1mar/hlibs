#ifndef ANSI_HPP
#define ANSI_HPP

#include <string>
#include <cinttypes>


namespace ansi {

    enum class EscapeCharacterType : bool {
        Octal,
        Hexadecimal
    };


    /// Control Sequence Introducer. Either "\033[" or "\\x1B[".
    class CSI {
    private:
        EscapeCharacterType type = EscapeCharacterType::Octal;
        std::string csi;

    public:
        explicit CSI(EscapeCharacterType charType = EscapeCharacterType::Octal) {
            set(charType);
        }

        void set(EscapeCharacterType charType) {
            csi.clear();

            if (charType == EscapeCharacterType::Hexadecimal) {
                csi.append("\\x1B[", 5);
                return;
            }

            csi.append("\\033[", 5);
        }

        inline std::string operator()() const noexcept {
            return csi;
        }

    };


    /// CSI end character
    enum class CSIEnclose : char {
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
    };


    struct Enclose {
        CSIEnclose enclose = CSIEnclose::SGR;

        inline char operator()() const noexcept {
            return static_cast<char>(enclose);
        }

    };


    /// Not for SGR control sequences.
    class CSISequencer {
    private:
        CSI csi{ EscapeCharacterType::Octal };

    public:
        explicit CSISequencer(EscapeCharacterType charType = EscapeCharacterType::Octal) {
            csi.set(charType);
        }

        /// Cursor Up. Moves the cursor n cells upward.
        std::string setCUU(std::size_t cells = 1U) {
            // "\033[ $n A"
            char endChar = Enclose{CSIEnclose::CUU}();
            std::string str(csi() + std::to_string(cells) + endChar);
            return str;
        }

//        void setCUD(uint8_t n);
//        void setCUF(uint8_t n);
//        void setCUB(uint8_t n);
//        void setCNL(uint8_t n);
//        void setCPL(uint8_t n);
//        void setCUP(uint8_t n);
//        void setCHA(uint8_t n);
//        void setHVP(uint8_t n);
//        void setED(uint8_t n);
//        void setEL(uint8_t n);
//        void setSU(uint8_t n);
//        void setSD(uint8_t n);

        // private sequences
//        void setSCP();                      // "ESC[s"
//        void setRCP();                      // "ESC[u"
//        void reset();                       // "ESC[0m"

        /// Send custom ANSI CSI escape sequence beside those anticipated.
        // range check of 0x20-0x7E; must starts with "ESC["; regex in future
        // CSISequenceType: (SingleByte, TwoBytes, MultiBytes)
        // <<csi_seq>> or throw()
//        void setXX(const std::string_view sequence);

    };


    // TODO: CSISequencer, SGRSequence -> Sequencer
    // Sequencer.moveCursor(Direction, cells) -> CSISequencer{ CUU, CUD, CUF, CUB }...

    // TODO: range check

}

#endif //ANSI_HPP
