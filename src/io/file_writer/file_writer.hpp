#ifndef LIBS_FILE_WRITER_HPP
#define LIBS_FILE_WRITER_HPP

#include <string>
#include <string_view>
#include <fstream>

#include "../../types/interfaces.hpp"
#include "../../types/types.hpp"


namespace libs::io::file_writer {

    class FileWriter : libs::types::interfaces::IStringify {
      public:
        explicit FileWriter(const std::string& path) : out_path(path), out_file(path, std::ios::out | std::ios::trunc)
        {
            if (!isOpened()) {
                throw std::ios_base::failure("!isOpened()");
            }

            out_file.exceptions(std::ofstream::badbit);
        }

        FileWriter(const FileWriter&) = delete;

        FileWriter& operator=(const FileWriter&) = delete;

        FileWriter(FileWriter&& rhs) noexcept : out_path(std::move(rhs.out_path)), out_file(std::move(rhs.out_file))
                                                , out_writes_size(rhs.out_writes_size)
        {
        }

        FileWriter& operator=(FileWriter&& rhs) noexcept
        {
            out_path = std::move(rhs.out_path);
            out_file = std::move(rhs.out_file);
            out_writes_size = rhs.out_writes_size;
            return *this;
        }

        ~FileWriter() noexcept override
        {
            out_file.flush();
            out_file.close();
        }

        bool isOpened() const
        {
            return out_file.is_open() && !out_file.fail();
        }

        /// Inserts unformatted blocks of characters.
        void write(const std::string_view text)
        {
            const auto str = std::string(text);
            const auto size = static_cast<std::streamsize>(str.size());
            out_file.write(str.c_str(), size);
            out_writes_size += str.size();
        }

        template<typename T>
        requires libs::types::Character<T>
        void write(const T character)
        {
            out_file << character;
            ++out_writes_size;
        }

        FileWriter& operator<<(const std::string_view& rhs)
        {
            out_file << rhs;
            out_writes_size += rhs.size();
            return *this;
        }

        template<typename T>
        requires types::Character<T>
        FileWriter& operator<<(const T rhs)
        {
            out_file << rhs;
            ++out_writes_size;
            return *this;
        }

        /// Returns the output position indicator.
        long getCursorPosition()
        {
            const auto pos = static_cast<long>(out_file.tellp());
            return pos;
        }

        /// Sets the output position indicator to absolute value position.
        void setCursorPosition(const long position)
        {
            const auto pos = std::ofstream::pos_type(position);
            out_file.seekp(pos);
        }

        /// Sets the output position indicator to offset relative to direction.
        void setCursorPosition(const long offset, std::ios::seekdir direction)
        {
            std::ofstream::off_type off(offset);
            out_file.seekp(off, direction);
        }

        [[nodiscard]] std::string toString() const noexcept override
        {
            const std::string fileName = out_path.substr(out_path.find_last_of("/\\") + 1);
            const std::string numberOfBytes = std::to_string(out_writes_size);
            return '{' + fileName + ", " + numberOfBytes + " B}";
        }

      private:
        std::string out_path;
        std::ofstream out_file;
        std::size_t out_writes_size = 0;
    };

}

#endif //LIBS_FILE_WRITER_HPP
