#ifndef LIBS_FILE_READER_BUFFERED_HPP
#define LIBS_FILE_READER_BUFFERED_HPP

#include "file_reader_base.hpp"
#include "../file_info.hpp"


namespace hlibs::io::file_reader {

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
            return !hlibs::io::IsEOF(elicitStream());
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

#endif //LIBS_FILE_READER_BUFFERED_HPP
