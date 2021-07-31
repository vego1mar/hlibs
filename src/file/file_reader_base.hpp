#ifndef FILE_READER_BASE_HPP
#define FILE_READER_BASE_HPP

#include <string>
#include <fstream>
#include <vector>

#include "file_info.hpp"
#include "../interfaces/i_stringify.hpp"


namespace file::reader {

    class FileReaderBase : public interfaces::IStringify {
      private:
        std::string path;
        std::ifstream stream;
        std::vector<char> content;

      public:
        FileReaderBase() = delete;

        FileReaderBase(const FileReaderBase& rhs) = delete;

        FileReaderBase& operator=(const FileReaderBase& rhs) = delete;

        FileReaderBase(FileReaderBase&& rhs) noexcept = default;

        FileReaderBase& operator=(FileReaderBase&& rhs) noexcept = default;

        ~FileReaderBase() noexcept override = default;

        explicit FileReaderBase(const std::string& filePath)
                : path(filePath), stream(std::ifstream(filePath, std::ios::in))
        {
        }

        inline auto& get() noexcept
        {
            return content;
        }

        bool isReadClean() const
        {
            return static_cast<std::size_t>(info::GetFileSize(path)) == content.size();
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
    };

}

#endif //FILE_READER_BASE_HPP
