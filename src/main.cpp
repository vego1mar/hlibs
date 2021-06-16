#include <iostream>
#include "file/file_reader.hpp"
#include "file/file_reader_buffered.hpp"


namespace quick_usage {
    void file_reader_run() {
        const std::string FILE_PATH = "../README.md";

        const auto info = [](const file::FileReaderBase &rdr) {
            std::cout << "isOpened(): " << std::boolalpha << rdr.isOpened() << std::endl;
            std::cout << "isReadClean(): " << std::boolalpha << rdr.isReadClean() << std::endl;
        };


        file::FileReader reader{FILE_PATH};
        reader.readFile();
        auto content = reader.get();
        info(reader);
        std::cout << "content.size(): " << content.size() << std::endl;


        file::FileReaderBuffered reader2{FILE_PATH};
        info(reader2);

        while (reader2.hasNextLine()) {
            auto line = reader2.getNextLine();
            std::cout << line.size() << ',';
        }

        std::cout << std::endl;
    }
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    quick_usage::file_reader_run();
    return 0;
}
