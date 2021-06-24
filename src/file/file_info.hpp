#ifndef FILE_INFO_HPP
#define FILE_INFO_HPP

#include <fstream>
#include <sys/stat.h>


namespace file::info {

    static bool is_eof(std::ifstream &file) {
        return file.peek() == std::ifstream::traits_type::eof() && file.eof();
    }

    // Portable for Windows, Mac and Linux as part of C Runtime Library.
    static long get_file_size(const std::string &file) {
        struct stat stat_buffer{};
        int rc = stat(file.c_str(), &stat_buffer);
        return rc == 0 ? stat_buffer.st_size : -1;
    }

}

#endif //FILE_INFO_HPP
