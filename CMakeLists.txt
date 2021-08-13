cmake_minimum_required(VERSION 3.20)
project(hlibs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "--coverage")
string(APPEND src "${PROJECT_SOURCE_DIR}/sources")

add_executable(
        hlibs_target
        ${src}/main.cpp

        ${src}/facilities/strings.hpp
        ${src}/facilities/timestamp.hpp

        ${src}/io/file/file_info.hpp
        ${src}/io/file/stream_to_file.hpp
        ${src}/io/file_reader/file_reader_base.hpp
        ${src}/io/file_reader/file_reader_buffered.hpp
        ${src}/io/file_reader/file_reader.hpp
        ${src}/io/file_writer/file_writer.hpp

        ${src}/logging/logger.hpp
        ${src}/logging/string_logger.hpp
        ${src}/logging/stdout_logger.hpp
        ${src}/logging/console_logger.hpp

        ${src}/standard/ansi.hpp

        ${src}/types/interfaces.hpp
        ${src}/types/types.hpp
)

enable_testing()
add_subdirectory(tests)
