#ifndef LIBS_HELPER_OBJECTS_HPP
#define LIBS_HELPER_OBJECTS_HPP

#include <fstream>
#include <streambuf>
#include <string>
#include <any>
#include <memory>
#include <functional>
#include <vector>
#include <string_view>
#include <sstream>
#include <filesystem>
#include <iomanip>
#include <unordered_map>
#include <limits>
#include <array>

#include "./free_functions.hpp"


namespace hlibs::io {

    /// Redirects std::cout, std::cerr, or std::clog to a file.
    class StreamToFile final : private std::filebuf {
      public:
        StreamToFile(std::ostream& stream, const std::string& path): ref_stream(stream), ptr_buffer(stream.rdbuf())
        {
            open(path.c_str(), std::ios_base::out);
            if (!is_open()) throw std::ios_base::failure("!is_open()");
            stream.rdbuf(this);
        }

        ~StreamToFile() noexcept override
        {
            auto&& buffer = std::any_cast<decltype(ref_stream.get().rdbuf())>(ptr_buffer);
            ref_stream.get().rdbuf(buffer);
            ptr_buffer.reset();
            close();
        }

        StreamToFile(const StreamToFile& rhs) = delete;
        StreamToFile& operator=(const StreamToFile& rhs) = delete;

        // TODO: open class for move
        StreamToFile(StreamToFile&& rhs) noexcept = delete;
        StreamToFile& operator=(StreamToFile&& rhs) noexcept = delete;

        // TODO: provide [[nodiscard]] toString() const noexcept

      private:
        std::reference_wrapper<std::ostream> ref_stream;
        std::any ptr_buffer;
    };


    /// Reads the whole text file at once (sink for data).
    class FileLoader final {
      public:
        explicit FileLoader(const std::filesystem::path& p): path(p), file(std::ifstream(p, std::ios_base::in))
        {
            if (!isOpened()) {
                file.close();
                throw std::ios::failure("!isOpened()");
            }
        }

        FileLoader(const FileLoader& rhs) = delete;
        FileLoader& operator=(const FileLoader& rhs) = delete;

        // TODO: open class for move
        FileLoader(FileLoader&& rhs) noexcept = delete;
        FileLoader& operator=(FileLoader&& rhs) noexcept = delete;

        ~FileLoader() noexcept = default;

        void read()
        {
            auto first = (std::istreambuf_iterator<char>(file));
            auto last = (std::istreambuf_iterator<char>());
            content = std::vector<char>(first, last);
        }

        inline const std::vector<char>& data() const noexcept
        {
            return content;
        }

        /// {"$Path","$FileAddress","$DataSize"}
        [[nodiscard]] std::string toString() const noexcept
        {
            std::stringstream ss{"{"};
            ss << '"' + path.string() + "\",\"";
            ss << &file;
            ss << "\",\"" << std::to_string(content.size()) + "\"}";
            return ss.str();
        }

      private:
        inline bool isOpened() const noexcept
        {
            return !file.fail() && file.is_open();
        }

        std::filesystem::path path;
        std::ifstream file;
        std::vector<char> content{};
    };


    // Allows to read a text file line-by-line (and counts the number of lines already read).
    class FileReader final {
      public:
        explicit FileReader(const std::filesystem::path& p): path(p), file(std::ifstream(p, std::ios::in))
        {
            if (file.fail() || !file.is_open()) {
                file.close();
                throw std::ios::failure("file.fail() || !file.is_open()");
            }
        }

        FileReader(const FileReader& rhs) = delete;
        FileReader& operator=(const FileReader& rhs) = delete;

        // TODO: open class for move
        FileReader(FileReader&& rhs) noexcept = delete;
        FileReader& operator=(FileReader&& rhs) noexcept = delete;

        ~FileReader() noexcept = default;

        std::string getNextLine()
        {
            std::string line;
            std::getline(file, line);
            ++lines_read;
            return line;
        }

        inline bool hasNextLine() noexcept
        {
            return !hlibs::io::IsEOF(file);
        }

        /// {"$Path","$FileAddress","$NumberOfLinesRead"}
        [[nodiscard]] std::string toString() const noexcept
        {
            std::stringstream ss{"{"};
            ss << '"' << path << "\",";
            ss << '"' << &file << "\",";
            ss << '"' << std::to_string(lines_read) << '"';
            ss << '}';
            return ss.str();
        }

        inline std::size_t lines() const noexcept
        {
            return lines_read;
        }

      private:
        std::filesystem::path path;
        std::ifstream file;
        std::size_t lines_read = 0;
    };


    /// Allows to write formatted text and numbers (and holds the number of written bytes).
    class FileWriter final {
      public:
        enum class NewLine : int8_t {
            Unix = 1,
            Windows = 2,
            Mac = 4
        };

        /// url:https://en.cppreference.com/w/cpp/io/ios_base/openmode
        explicit FileWriter(const std::filesystem::path& p, std::ios_base::openmode mode = std::ios::trunc)
                : path(p), file(p, mode)
        {
            bool isAllowedOpenMode = mode == std::ios::trunc || mode == std::ios::app || mode == std::ios::out;

            if (bool isOpenedCorrectly = file.is_open() && file.good(); !isAllowedOpenMode || !isOpenedCorrectly) {
                file.close();
                throw std::ios_base::failure("!isAllowedOpenMode || !isOpenedCorrectly");
            }

            file.exceptions(std::ofstream::badbit);
        }

        FileWriter(const FileWriter& rhs) = delete;
        FileWriter& operator=(const FileWriter& rhs) = delete;

        // TODO: open class for move
        FileWriter(FileWriter&& rhs) noexcept = delete;
        FileWriter& operator=(FileWriter&& rhs) noexcept = delete;

        ~FileWriter() noexcept = default;

        void write(std::string_view text)
        {
            file << text;
            bytes_written += text.size();
        }

        template<typename T>
        requires std::is_same_v<T, char> || std::is_same_v<T, signed char> || std::is_same_v<T, unsigned char>
        void put(T ch)
        {
            file.put(ch);
            ++bytes_written;
        }

        /// url:https://en.wikipedia.org/wiki/Newline
        void newline(NewLine type = NewLine::Unix)
        {
            static const std::unordered_map<NewLine, std::array<char, 2>> separator = {
                    {NewLine::Unix,    std::array<char, 2>{'\n', '\0'}},
                    {NewLine::Windows, std::array<char, 2>{'\r', '\n'}},
                    {NewLine::Mac,     std::array<char, 2>{'\r', '\0'}},
            };

            file << separator.at(type).data();
            ++bytes_written;
            if (type == NewLine::Windows) ++bytes_written;
        }

        template<typename T>
        requires std::numeric_limits<T>::is_integer
        void integer(T integer, std::ios_base::fmtflags flags = std::ios_base::dec, std::streamsize width = 0)
        {
            std::ostringstream ss;
            ss.flags(flags);
            ss.width(width);
            ss << integer;
            auto str = ss.str();

            file << str;
            bytes_written += str.size();
        }

        /// url:https://en.cppreference.com/w/cpp/io/ios_base/fmtflags
        template<typename T>
        requires std::is_floating_point_v<T>
        void floating(T number, std::streamsize point = 8, std::ios_base::fmtflags flags = std::ios::fixed, std::streamsize width = 0)
        {
            std::ostringstream ss;
            ss.flags(flags);
            ss.precision(point);
            ss.width(width);
            ss << number;
            auto str = ss.str();

            file << str;
            bytes_written += str.size();
        }

        void boolean(bool b, std::streamsize width = 0)
        {
            std::ostringstream ss;
            ss.width(width);
            ss << std::boolalpha << b;
            auto str = ss.str();

            file << str;
            bytes_written += str.size();
        }

        /// {"$Path","$FileAddress","$BytesWritten"}
        [[nodiscard]] std::string toString() const noexcept
        {
            std::stringstream ss;
            ss << '{';
            ss << std::quoted(path.string()) << ',';
            ss << '"' << &file << "\",";
            ss << std::quoted(std::to_string(bytes_written));
            ss << "}";
            return ss.str();
        }

        /// url:https://en.cppreference.com/w/cpp/io/basic_ostream/tellp
        std::ofstream::pos_type position()
        {
            return file.tellp();
        }

        /// url:https://en.cppreference.com/w/cpp/io/ios_base/seekdir
        void position(std::ofstream::pos_type offset, std::ios::seekdir direction = std::ios::cur)
        {
            file.seekp(std::ofstream::off_type(offset), direction);
        }

        inline void sync()
        {
            file.flush();
        }

        inline std::size_t bytes() const noexcept
        {
            return bytes_written;
        }

      private:
        std::filesystem::path path;
        std::ofstream file;
        std::size_t bytes_written = 0;
    };


    // TODO: BinaryReader && BinaryWriter
    // https://stackoverflow.com/questions/15366319/how-to-read-the-binary-file-in-c
    // https://stackoverflow.com/questions/55777716/unable-to-read-a-binary-file-into-stdvectorstdbyte-in-c
    // https://www.positioniseverything.net/cpp-read-binary-file

}

#endif //LIBS_HELPER_OBJECTS_HPP
