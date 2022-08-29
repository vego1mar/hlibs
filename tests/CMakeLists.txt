find_package(Catch2 3 REQUIRED)

add_executable(
        tests_target

        facilities/strings_test.cpp ${src}/facilities/strings.hpp
        facilities/timestamp_test.cpp ${src}/facilities/timestamp.hpp

        io/file/file_info_test.cpp ${src}/io/file/file_info.hpp
        io/file/stream_to_file_test.cpp ${src}/io/file/stream_to_file.hpp
        io/file_reader/file_reader_base_test.cpp ${src}/io/file_reader/file_reader_base.hpp
        io/file_writer/file_writer_test.cpp ${src}/io/file_writer/file_writer.hpp

        logging/logger_test.cpp ${src}/logging/logger.hpp

        standard/ansi_test.cpp ${src}/standard/ansi.hpp

        types/interfaces_test.cpp ${src}/types/interfaces.hpp
        types/types_test.cpp ${src}/types/types.hpp
)

target_link_libraries(tests_target PRIVATE Catch2::Catch2WithMain)
include(Catch)
catch_discover_tests(tests_target)
