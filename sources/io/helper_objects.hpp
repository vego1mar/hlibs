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

}

#endif //LIBS_HELPER_OBJECTS_HPP
