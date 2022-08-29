#ifndef LIBS_STREAM_TO_FILE_HPP
#define LIBS_STREAM_TO_FILE_HPP

#include <fstream>
#include <streambuf>
#include <string>
#include <any>
#include <memory>
#include <functional>


namespace hlibs::io::file {

    /// Redirects std::cout, std::cerr, or std::clog to a file.
    class StreamToFile final : private std::filebuf {
      public:
        StreamToFile(std::ostream& stream, const std::string& path) : ref_stream(stream), ptr_buffer(stream.rdbuf())
        {
            open(path.c_str(), std::ios_base::out);
            if (!is_open()) throw std::ios_base::failure("!is_open()");
            stream.rdbuf(this);
        }

        ~StreamToFile() noexcept override
        {
            auto&& buffer = std::any_cast<decltype(ref_stream.get().rdbuf())>(ptr_buffer);
            ref_stream.get().rdbuf(buffer);
            ptr_buffer.reset();
            close();
        }

        StreamToFile(const StreamToFile& rhs) = delete;
        StreamToFile& operator=(const StreamToFile& rhs) = delete;

        StreamToFile(StreamToFile&& rhs) noexcept = delete;
        StreamToFile& operator=(StreamToFile&& rhs) noexcept = delete;

      private:
        std::reference_wrapper<std::ostream> ref_stream;
        std::any ptr_buffer;
    };

}

#endif //LIBS_STREAM_TO_FILE_HPP
