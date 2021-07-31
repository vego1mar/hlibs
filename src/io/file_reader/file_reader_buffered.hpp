#ifndef LIBS_IO_FILE_READER_FILE_READER_BUFFERED_HPP
#define LIBS_IO_FILE_READER_FILE_READER_BUFFERED_HPP

#include "file_reader_base.hpp"
#include "../file/file_info.hpp"


namespace libs::io::file_reader {

    class FileReaderBuffered : public FileReaderBase {
      public:
        using FileReaderBase::FileReaderBase;


        const auto& getNextLine()
        {
            std::getline(elicitStream(), line);
            ++lines;
            return line;
        }

        bool hasNextLine()
        {
            return !libs::io::file::IsEOF(elicitStream());
        }

        std::string toString() const noexcept override
        {
            auto mangledName = std::string(typeid(this).name());
            mangledName.append("{ ");
            mangledName.append("path=\"" + elicitPath() + "\", ");
            mangledName.append("lines=" + std::to_string(lines));
            mangledName.append(" }");
            return mangledName;
        }

      private:
        std::string line;
        std::size_t lines = 0;
    };

}

#endif //LIBS_IO_FILE_READER_FILE_READER_BUFFERED_HPP
