#ifndef FILE_READER_BASE_HPP
#define FILE_READER_BASE_HPP

#include <string>
#include <fstream>
#include <vector>

#include "file_info.hpp"


namespace file::reader {

    class FileReaderBase {
    protected:
        std::string path;
        std::ifstream stream;
        std::vector<char> content;

    public:
        explicit FileReaderBase(const std::string &filePath)
                : path(filePath), stream(std::ifstream(filePath, std::ios::in)), content({}) {
        }

        inline auto &get() noexcept {
            return content;
        }

        bool isReadClean() const {
            return static_cast<std::size_t>(info::get_file_size(path)) == content.size();
        }

        inline bool isOpened() const {
            return !stream.fail() && stream.is_open();
        }

        virtual std::string toString() const noexcept = 0;
    };

}

#endif //FILE_READER_BASE_HPP
