#ifndef LIBS_FILE_WRITER_HPP
#define LIBS_FILE_WRITER_HPP

#include <string>
#include <fstream>

#include "../../types/interfaces.hpp"


namespace libs::io::file_writer {

    class FileWriter : libs::types::interfaces::IStringify {
      public:
        explicit FileWriter(const std::string& path) : out_path(path), out_file(path, std::ios::out | std::ios::trunc)
        {
            if (!isOpened()) {
                throw std::ios_base::failure("!isOpened()");
            }
        }

        ~FileWriter() noexcept override
        {
            out_file.flush();
            out_file.close();
        }

        [[nodiscard]] bool isOpened() const
        {
            return out_file.is_open() && !out_file.fail();
        }

        [[nodiscard]] std::string toString() const noexcept override
        {
            const std::string fileName = out_path.substr(out_path.find_last_of("/\\") + 1);
            const std::string bufferedMessages = std::to_string(out_buffered_messages);
            return '{' + fileName + ", " + bufferedMessages + '}';
        }

      private:
        std::string out_path;
        std::ofstream out_file;
        std::size_t out_buffered_messages = 0;
    };

}

#endif //LIBS_FILE_WRITER_HPP
