add_executable(
        Catch2_tests_run

        main_test.cpp

        file/file_info_test.cpp ../src/file/file_info.hpp
        file/file_reader_base_test.cpp ../src/file/file_reader_base.hpp

        interfaces/i_stringify_test.cpp ../src/interfaces/i_stringify.hpp
)

target_link_libraries(Catch2_tests_run)
