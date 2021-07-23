cmake_minimum_required(VERSION 3.20)
project(LIBS)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        LIBS
        src/main.cpp

        src/strings/strings.hpp

        src/date_time/date_time.hpp

        src/file/file_reader_base.hpp
        src/file/file_reader_buffered.hpp
        src/file/file_reader.hpp
        src/file/file_info.hpp

        src/interfaces/i_stringify.hpp

        src/logging/logger.hpp
        src/logging/string_logger.hpp
)

enable_testing()
add_subdirectory(catch2_tests)

