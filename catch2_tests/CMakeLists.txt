add_executable(
        Catch2_tests_run

        main_test.cpp

        date_time/date_time_test.cpp ../src/date_time/date_time.hpp

        file/file_info_test.cpp ../src/file/file_info.hpp
        file/file_reader_base_test.cpp ../src/file/file_reader_base.hpp

        interfaces/i_stringify_test.cpp ../src/interfaces/i_stringify.hpp

        logging/severity_level_test.cpp ../src/logging/logger.hpp
        logging/string_logger_test.cpp ../src/logging/string_logger.hpp
)

target_link_libraries(Catch2_tests_run)
