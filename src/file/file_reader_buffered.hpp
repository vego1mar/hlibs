#ifndef FILE_READER_BUFFERED_HPP
#define FILE_READER_BUFFERED_HPP

#include <functional>
#include "file_reader_base.hpp"
#include "file_info.hpp"


namespace file {

    class FileReaderBuffered : public FileReaderBase {
    private:
        std::string line;

    public:
        FileReaderBuffered() = delete;

        explicit FileReaderBuffered(const std::string &filePath) : FileReaderBase(filePath) {
        }

        FileReaderBuffered(const FileReaderBuffered &rhs) = delete;

        FileReaderBuffered &operator=(const FileReaderBuffered &rhs) = delete;

        FileReaderBuffered(FileReaderBuffered &&rhs) noexcept = delete;

        FileReaderBuffered &operator=(FileReaderBuffered &&rhs) noexcept = delete;

        const auto &getNextLine() {
            std::getline(stream, line);
            return line;
        }

        bool hasNextLine() {
            return !FileInfo::isEOF(stream);
        }
    };

}

#endif //FILE_READER_BUFFERED_HPP
