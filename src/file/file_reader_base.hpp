#ifndef FILE_READER_BASE_HPP
#define FILE_READER_BASE_HPP

#include <string>
#include <fstream>
#include <vector>

#include "file_info.hpp"


namespace file {

    class FileReaderBase {
    protected:
        std::string path;
        std::ifstream stream;
        std::vector<char> content;

    public:
        FileReaderBase() = delete;

        explicit FileReaderBase(const std::string &filePath)
                : path(filePath), stream(std::ifstream(filePath, std::ios::in)), content({}) {
        }

        FileReaderBase(const FileReaderBase &rhs) = delete;

        FileReaderBase(FileReaderBase &&rhs) noexcept = delete;

        FileReaderBase &operator=(const FileReaderBase &rhs) = delete;

        FileReaderBase &operator=(FileReaderBase &&rhs) noexcept = delete;

        virtual ~FileReaderBase() {
            stream.close();
        };

        inline const auto &getPath() const {
            return path;
        }

        inline auto &get() {
            return content;
        }

        bool isReadClean() const {
            return static_cast<std::size_t>(FileInfo::getFileSize(path)) == content.size();
        }

        bool isOpened() const {
            return !stream.fail() && stream.is_open();
        }

    };

}

#endif //FILE_READER_BASE_HPP
