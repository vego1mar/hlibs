#ifndef FILE_READER_HPP
#define FILE_READER_HPP

#include "file_reader_base.hpp"


namespace file {

    class FileReader : FileReaderBase {
    public:
        FileReader() = default;

        FileReader(const FileReader &rhs) = delete;

        FileReader(FileReader &&rhs) noexcept = delete;

        FileReader &operator=(const FileReader &rhs) = delete;

        FileReader &operator=(FileReader &&rhs) noexcept = delete;

        ~FileReader() override = default;

        inline bool readFile() {
            if (!file.is_open()) {
                return false;
            }

            auto first = (std::istreambuf_iterator<char>(stream));
            auto last = (std::istreambuf_iterator<char>());
            content = std::vector<char>(first, last);
            return true;
        }

    };

}

#endif //FILE_READER_HPP
