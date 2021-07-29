add_executable(
        Catch2_tests_run

        main_test.cpp

        ansi/ansi_test.cpp ../src/ansi/ansi.hpp

        date_time/date_time_test.cpp ../src/date_time/date_time.hpp

        file/file_info_test.cpp ../src/file/file_info.hpp
        file/file_reader_base_test.cpp ../src/file/file_reader_base.hpp
        file/stream_to_file_test.cpp ../src/file/stream_to_file.hpp

        interfaces/i_stringify_test.cpp ../src/interfaces/i_stringify.hpp

        logging/severity_level_test.cpp ../src/logging/logger.hpp
        logging/string_logger_test.cpp ../src/logging/string_logger.hpp
        logging/stdout_logger_test.cpp ../src/logging/stdout_logger.hpp

        strings/strings_test.cpp ../src/strings/strings.hpp

        templates/object_counter_test.cpp ../src/templates/object_counter.hpp
        templates/range_test.cpp ../src/templates/range.hpp

)

target_link_libraries(Catch2_tests_run)
