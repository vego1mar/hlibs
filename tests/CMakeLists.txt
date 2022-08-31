find_package(Catch2 3 REQUIRED)

add_executable(
        tests_target

        facilities/strings_test.cpp ${src}/facilities/strings.hpp
        facilities/timestamp_test.cpp ${src}/facilities/timestamp.hpp

        io/free_functions_test.cpp ${src}/io/free_functions.hpp
        io/helper_objects_test.cpp ${src}/io/helper_objects.hpp

        logging/logger_test.cpp ${src}/logging/logger.hpp

        standard/ansi_test.cpp ${src}/standard/ansi.hpp

        types/interfaces_test.cpp ${src}/types/interfaces.hpp
        types/types_test.cpp ${src}/types/types.hpp
)

target_link_libraries(tests_target PRIVATE Catch2::Catch2WithMain)
include(Catch)
catch_discover_tests(tests_target)
