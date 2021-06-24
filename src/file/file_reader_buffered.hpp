#ifndef FILE_READER_BUFFERED_HPP
#define FILE_READER_BUFFERED_HPP

#include "file_reader_base.hpp"
#include "file_info.hpp"


namespace file::reader {

    class FileReaderBuffered : public FileReaderBase {
    private:
        std::string line;
        std::size_t lines;

    public:

        explicit FileReaderBuffered(const std::string &filePath) : FileReaderBase(filePath), lines(0) {
        }

        const auto &getNextLine() {
            std::getline(stream, line);
            ++lines;
            return line;
        }

        bool hasNextLine() {
            return !info::is_eof(stream);
        }

        std::string toString() const noexcept override {
            auto mangledName = std::string(typeid(this).name());
            mangledName.append("{ ");
            mangledName.append("path=\"" + path + "\", ");
            mangledName.append("lines=" + std::to_string(lines));
            mangledName.append(" }");
            return mangledName;
        }
    };

}

#endif //FILE_READER_BUFFERED_HPP
