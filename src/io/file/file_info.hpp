#ifndef LIBS_IO_FILE_FILE_INFO_HPP
#define LIBS_IO_FILE_FILE_INFO_HPP

#include <fstream>
#include <sys/stat.h>


namespace libs::io::file {

    static bool IsEOF(std::ifstream& file)
    {
        return file.peek() == std::ifstream::traits_type::eof() && file.eof();
    }

    /// Portable for Windows, Mac and Linux as part of C Runtime Library.
    static long GetFileSize(const std::string& file)
    {
        struct stat stat_buffer{};
        int rc = stat(file.c_str(), &stat_buffer);
        return rc == 0 ? stat_buffer.st_size : -1;
    }

}

#endif //LIBS_IO_FILE_FILE_INFO_HPP
