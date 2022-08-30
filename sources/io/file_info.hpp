#ifndef LIBS_FILE_INFO_HPP
#define LIBS_FILE_INFO_HPP

#include <fstream>
#include <sys/stat.h>


namespace hlibs::io {

    [[maybe_unused]] static bool IsEOF(std::ifstream& file)
    {
        return file.peek() == std::ifstream::traits_type::eof() && file.eof();
    }

    /// Portable for Windows, Mac and Linux as part of C Runtime Library.
    [[maybe_unused]] static long GetFileSize(const std::string& file)
    {
        struct stat stat_buffer{};
        int rc = stat(file.c_str(), &stat_buffer);
        return rc == 0 ? stat_buffer.st_size : -1;
    }

}

#endif //LIBS_FILE_INFO_HPP
