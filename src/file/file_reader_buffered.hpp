#ifndef FILE_READER_BUFFERED_HPP
#define FILE_READER_BUFFERED_HPP

#include <functional>
#include "file_reader_base.hpp"
#include "file_info.hpp"


namespace file {

    class FileReaderBuffered : FileReaderBase {
    public:
        FileReaderBuffered() = default;

        FileReaderBuffered(const FileReaderBuffered &rhs) = delete;

        FileReaderBuffered &operator=(const FileReaderBuffered &rhs) = delete;

        FileReaderBuffered(FileReaderBuffered &&rhs) noexcept = delete;

        FileReaderBuffered &operator=(FileReaderBuffered &&rhs) noexcept = delete;

        ~FileReaderBuffered() override = default;

        inline std::string getNextLine() {
            if (!file.is_open()) {
                throw std::bad_function_call("!file.is_open()");
            }

            std::string line;
            std::getline(stream, line);
            return line;
        }

        bool hasNextLine() {
            return FileInfo::isEOF(stream);
        }

    };

}

#endif //FILE_READER_BUFFERED_HPP
