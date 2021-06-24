#ifndef FILE_READER_HPP
#define FILE_READER_HPP

#include "file_reader_base.hpp"


namespace file::reader {

    class FileReader : public FileReaderBase {
    public:
        explicit FileReader(const std::string &filePath) : FileReaderBase(filePath) {
        }

        void readFile() {
            auto first = (std::istreambuf_iterator<char>(stream));
            auto last = (std::istreambuf_iterator<char>());
            content = std::vector<char>(first, last);
        }

        std::string toString() const noexcept override {
            auto mangledName = std::string(typeid(this).name());
            mangledName.append("{ ");
            mangledName.append("path=\"" + path + "\", ");
            mangledName.append("content.size()=" + std::to_string(content.size()));
            mangledName.append(" }");
            return mangledName;
        }
    };

}

#endif //FILE_READER_HPP
