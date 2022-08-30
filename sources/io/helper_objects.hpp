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

#include "./free_functions.hpp"


namespace hlibs::io {

    /// Redirects std::cout, std::cerr, or std::clog to a file.
    class StreamToFile final : private std::filebuf {
      public:
        StreamToFile(std::ostream& stream, const std::string& path) : ref_stream(stream), ptr_buffer(stream.rdbuf())
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

        StreamToFile(StreamToFile&& rhs) noexcept = delete;
        StreamToFile& operator=(StreamToFile&& rhs) noexcept = delete;

      private:
        std::reference_wrapper<std::ostream> ref_stream;
        std::any ptr_buffer;
    };


    /// Reads the whole text file at once (a sink for the data).
    class FileLoader final {
      public:
        explicit FileLoader(const std::filesystem::path& path) : path(path), file(std::ifstream(path, std::ios_base::in))
        {
            if (!isOpened()) {
                file.close();
                throw std::ios::failure("!isOpened()");
            }
        }

        FileLoader(const FileLoader& rhs) = delete;
        FileLoader& operator=(const FileLoader& rhs) = delete;

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
        std::string toString() const noexcept
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
        explicit FileReader(const std::filesystem::path& path) : path(path), file(std::ifstream(path, std::ios::in))
        {
            if (file.fail() || !file.is_open()) {
                file.close();
                throw std::ios::failure("file.fail() || !file.is_open()");
            }
        }

        FileReader(const FileReader& rhs) = delete;
        FileReader& operator=(const FileReader& rhs) = delete;

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
        std::string toString() const noexcept
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


    // TODO: BinaryReader
    // https://stackoverflow.com/questions/15366319/how-to-read-the-binary-file-in-c
    // https://stackoverflow.com/questions/55777716/unable-to-read-a-binary-file-into-stdvectorstdbyte-in-c
    // https://www.positioniseverything.net/cpp-read-binary-file


    /// TODO: one-line doc
    class FileWriter final {
      public:
        enum class OpenMode : bool {
            Truncate = true,
            Append = false
        };

        /// url:https://en.cppreference.com/w/cpp/io/ios_base/openmode
        explicit FileWriter(const std::filesystem::path& path, OpenMode mode = OpenMode::Truncate)
                : path(path)
                  , file(path, std::ios::out | ((mode == OpenMode::Truncate) ? std::ios::trunc : std::ios::ate))
        {
            if (!file.is_open() || file.fail()) {
                throw std::ios_base::failure("!file.is_open() || file.fail()");
            }

            file.exceptions(std::ofstream::badbit);
        }

        FileWriter(const FileWriter& rhs) = delete;
        FileWriter& operator=(const FileWriter& rhs) = delete;

        FileWriter(FileWriter&& rhs) noexcept = delete;
        FileWriter& operator=(FileWriter&& rhs) noexcept = delete;

        ~FileWriter() noexcept = default;

        /// url:https://en.cppreference.com/w/cpp/io/ios_base/fmtflags
        void write(std::string_view text, std::ios_base::fmtflags flags = std::ios_base::right)
        {
            file << flags << text;
            bytes_written += text.size();
        }

        template<typename T>
        requires std::is_same_v<T, char> || std::is_same_v<T, signed char> || std::is_same_v<T, unsigned char>
        void put(T ch)
        {
            file << ch;
            ++bytes_written;
        }

        template<typename T>
        requires std::is_floating_point_v<T>
        void floating(T number, int point = 4, std::ios_base::fmtflags flags = std::ios_base::floatfield)
        {
            file << std::setprecision(point) << flags << number;
            bytes_written += sizeof(number);
        }

        void boolean(bool b)
        {
            file << std::boolalpha << b << std::noboolalpha;
            ++bytes_written;
        }

        /// {"$Path","$FileAddress","$BytesWritten"}
        std::string toString() const noexcept
        {
            std::stringstream ss{"{"};
            ss << '"' << path << "\",";
            ss << '"' << &file << "\",";
            ss << '"' << std::to_string(bytes_written) + '"';
            ss << "}";
            return ss.str();
        }

        std::ofstream::pos_type position()
        {
            return file.tellp();
        }

        void position(std::ofstream::pos_type pos)
        {
            file.seekp(pos);
        }

        void position(std::ofstream::pos_type offset, std::ios::seekdir direction)
        {
            file.seekp(std::ofstream::off_type(offset), direction);
        }

        inline void sync()
        {
            file.flush();
        }

      private:
        std::filesystem::path path;
        std::ofstream file;
        std::size_t bytes_written = 0;
    };

}

#endif //LIBS_HELPER_OBJECTS_HPP
