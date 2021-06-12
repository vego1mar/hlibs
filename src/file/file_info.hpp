#ifndef FILE_INFO_HPP
#define FILE_INFO_HPP

#include <fstream>
#include <sys/stat.h>


namespace file {

    class FileInfo {
    public:
        FileInfo() = delete;

        FileInfo(const FileInfo &rhs) = delete;

        FileInfo(FileInfo &&rhs) noexcept = delete;

        FileInfo &operator=(const FileInfo &rhs) = delete;

        FileInfo &operator=(FileInfo &&rhs) noexcept = delete;

        virtual ~FileInfo() = delete;

        static bool isEOF(std::ifstream &file) {
            return file.peek() == std::ifstream::traits_type::eof() && file.eof();
        }

        static long getFileSize(const std::string &file) {
            struct stat stat_buffer{};
            int rc = stat(file.c_str(), &stat_buffer);
            return rc == 0 ? stat_buffer.st_size : -1;
        }

    };

}

#endif //FILE_INFO_HPP
