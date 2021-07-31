#ifndef STREAM_TO_FILE_HPP
#define STREAM_TO_FILE_HPP

#include <fstream>
#include <streambuf>
#include <string>
#include <any>


namespace libs::io::file {

    class StreamToFile final : std::filebuf {
      public:
        StreamToFile(std::ostream& stream, const std::string& path) : ref_stream(stream), ptr_buffer(stream.rdbuf())
        {
            open(path.c_str(), std::ios_base::out);

            if (!is_open()) {
                throw std::ios_base::failure("!is_open()");
            }

            stream.rdbuf(this);
        }

        ~StreamToFile() noexcept override
        {
            auto buffer = std::any_cast<decltype(ref_stream.rdbuf())>(ptr_buffer);
            ref_stream.rdbuf(buffer);
            ptr_buffer.reset();
            close();
        }

      private:
        std::ostream& ref_stream;
        std::any ptr_buffer;
    };

}

#endif //STREAM_TO_FILE_HPP
