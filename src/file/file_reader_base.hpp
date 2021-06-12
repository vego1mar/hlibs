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

        FileReaderBase() : path(), stream(), content({}) {
        }

    public:

        FileReaderBase(const FileReaderBase &rhs) = delete;

        FileReaderBase(FileReaderBase &&rhs) noexcept = delete;

        FileReaderBase &operator=(const FileReaderBase &rhs) = delete;

        FileReaderBase &operator=(FileReaderBase &&rhs) noexcept = delete;

        virtual ~FileReaderBase() = default;

        inline bool open(const std::string &filePath, std::ios_base::openmode openModes = std::ios::out) {
            if (stream.is_open()) {
                stream.close();
            }

            path = std::string(filePath);
            stream = std::ifstream(filePath, openModes);
            return stream.fail();
        }

        inline void close() {
            if (stream.is_open()) {
                stream.close();
            }
        }

        inline const auto &getPath() const {
            return path;
        }

        inline auto &getContent() const {
            return content;
        }

        bool isReadClean() const {
            return static_cast<std::size_t>(FileInfo::getFileSize(path)) == content.size();
        }

    };

}

#endif //FILE_READER_BASE_HPP
