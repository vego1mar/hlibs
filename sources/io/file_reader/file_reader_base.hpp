#ifndef LIBS_FILE_READER_BASE_HPP
#define LIBS_FILE_READER_BASE_HPP

#include <string>
#include <fstream>
#include <vector>

#include "../file/file_info.hpp"
#include "../../types/interfaces.hpp"


namespace hlibs::io::file_reader {

  class FileReaderBase : public hlibs::types::interfaces::IStringify {
      public:
        FileReaderBase() = delete;

        FileReaderBase(const FileReaderBase& rhs) = delete;

        FileReaderBase& operator=(const FileReaderBase& rhs) = delete;

        FileReaderBase(FileReaderBase&& rhs) noexcept = default;

        FileReaderBase& operator=(FileReaderBase&& rhs) noexcept = default;

        ~FileReaderBase() noexcept override = default;

        explicit FileReaderBase(const std::string& filePath) : path(filePath), stream(std::ifstream(filePath, std::ios::in))
        {
        }

        inline auto& get() noexcept
        {
            return content;
        }

        bool isReadClean() const
        {
            return static_cast<std::size_t>(hlibs::io::file::GetFileSize(path)) == content.size();
        }

        inline bool isOpened() const
        {
            return !stream.fail() && stream.is_open();
        }

        std::string toString() const noexcept override = 0;

      protected:
        inline const std::string& elicitPath() const noexcept
        {
            return path;
        }

        inline std::ifstream& elicitStream() noexcept
        {
            return stream;
        }

        inline std::vector<char>& elicitContent() noexcept
        {
            return content;
        }

        std::size_t elicitContentSize() const noexcept
        {
            return content.size();
        }

      private:
        std::string path;
        std::ifstream stream;
        std::vector<char> content;
    };

}

#endif //LIBS_FILE_READER_BASE_HPP
