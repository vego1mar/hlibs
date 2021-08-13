#ifndef LIBS_FILE_READER_HPP
#define LIBS_FILE_READER_HPP

#include "file_reader_base.hpp"


namespace libs::io::file_reader {

    class FileReader : public FileReaderBase {
      public:
        using FileReaderBase::FileReaderBase;

        void readFile()
        {
            auto first = (std::istreambuf_iterator<char>(elicitStream()));
            auto last = (std::istreambuf_iterator<char>());
            elicitContent() = std::vector<char>(first, last);
        }

        std::string toString() const noexcept override
        {
            auto mangledName = std::string(typeid(this).name());
            mangledName.append("{ ");
            mangledName.append("path=\"" + elicitPath() + "\", ");
            mangledName.append("content.size()=" + std::to_string(elicitContentSize()));
            mangledName.append(" }");
            return mangledName;
        }
    };

}

#endif //LIBS_FILE_READER_HPP
