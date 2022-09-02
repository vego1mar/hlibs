#ifndef LIBS_FREE_FUNCTIONS_HPP
#define LIBS_FREE_FUNCTIONS_HPP

#include <fstream>
#include <sys/stat.h>
#include <filesystem>


namespace hlibs::io {

    /// The most thorough way to test for End Of File.
    [[maybe_unused]] static bool IsEOF(std::ifstream& file)
    {
        return file.peek() == std::ifstream::traits_type::eof() && file.eof();
    }

    /// Portable for Windows, Mac, and Linux as part of C Runtime Library.
    [[maybe_unused]] static long GetFileSize(const std::filesystem::path& file)
    {
        struct stat stat_buffer{};
        int rc = stat(file.c_str(), &stat_buffer);
        return rc == 0 ? stat_buffer.st_size : -1;
    }

}

#endif //LIBS_FREE_FUNCTIONS_HPP
