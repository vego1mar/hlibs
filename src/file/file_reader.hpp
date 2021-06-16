#ifndef FILE_READER_HPP
#define FILE_READER_HPP

#include "file_reader_base.hpp"


namespace file {

    class FileReader : public FileReaderBase {
    public:
        FileReader() = delete;

        explicit FileReader(const std::string &filePath) : FileReaderBase(filePath) {
        }

        FileReader(const FileReader &rhs) = delete;

        FileReader(FileReader &&rhs) noexcept = delete;

        FileReader &operator=(const FileReader &rhs) = delete;

        FileReader &operator=(FileReader &&rhs) noexcept = delete;

        void readFile() {
            auto first = (std::istreambuf_iterator<char>(stream));
            auto last = (std::istreambuf_iterator<char>());
            content = std::vector<char>(first, last);
        }
    };

}

#endif //FILE_READER_HPP
