cmake_minimum_required(VERSION 3.20)
project(hlibs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

string(APPEND src "${PROJECT_SOURCE_DIR}/sources")

add_executable(
        hlibs_target
        ${src}/main.cpp

        ${src}/facilities/strings.hpp
        ${src}/facilities/timestamp.hpp

        ${src}/io/free_functions.hpp
        ${src}/io/helper_objects.hpp
        ${src}/io/file_reader/file_reader_base.hpp
        ${src}/io/file_reader/file_reader_buffered.hpp
        ${src}/io/file_reader/file_reader.hpp
        ${src}/io/file_writer/file_writer.hpp

        ${src}/logging/logger.hpp

        ${src}/standard/ansi.hpp

        ${src}/types/interfaces.hpp
        ${src}/types/types.hpp
)

enable_testing()
add_subdirectory(tests)
