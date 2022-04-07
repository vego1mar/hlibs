#!/bin/bash

PROJECT_PATH=../
SOURCES_DIR=../sources
GCC=/usr/bin/gcc-10
CPP_VER=-std=c++20
GCC_FLAGS=" \
    -Wall -Wextra -Wfloat-equal -Wundef -Wshadow=global -Wpointer-arith -Wcast-align=strict -Wwrite-strings \
    -Wswitch-enum -Wconversion -Wunreachable-code -Wformat=2 -Wpedantic -Wno-unused-function -Wcast-qual \
    -fexceptions -fpie -fwrapv -ggdb -O2 $CPP_VER"
BUILD_DIR=sonar_build
TESTS_DIR=cmake-build-debug-coverage/tests
TESTS_RUN=tests_target
REPORT_TEST_EXECUTION=sonar-test-execution-report.xml
REPORT_COVERAGE=sonar-coverage-report.xml


function make_paths() {
    local reversed_pwd;
    local project_path;

    reversed_pwd=$(echo "$PWD" | rev)
    project_path=$(echo "${reversed_pwd#*/}" | rev)
    PROJECT_PATH="$project_path"
    SOURCES_DIR="$PROJECT_PATH"/sources

    local buildDirCopy="$BUILD_DIR"
    local testsDirCopy="$TESTS_DIR"
    local testsRunFileName="$TESTS_RUN"
    BUILD_DIR="$PROJECT_PATH"/"$buildDirCopy"
    TESTS_DIR="$PROJECT_PATH"/"$testsDirCopy"
    TESTS_RUN="$TESTS_DIR"/"$testsRunFileName"

    return 0
}

function dump_variables_resolution() {
    local variablesFileName="$BUILD_DIR"/sonar_build_var.txt

    {
        echo -e PROJECT_PATH="$PROJECT_PATH"
        echo -e SOURCES_DIR="$SOURCES_DIR"
        echo -e BUILD_DIR="$BUILD_DIR"
        echo -e TESTS_DIR="$TESTS_DIR"
        echo -e TESTS_RUN="$TESTS_RUN"
    } >> "$variablesFileName"

    return 0
}

function print_build_info() {
    local rc=$?

    if [ $rc -ne 0 ]; then
        # invert video
        echo -e "\033[7mBUILD FAILURE\033[27m"
        report_error_code 1
    else
        echo "BUILD SUCCESS"
    fi

    return 0
}

function remove_previous_build() {
    echo "REMOVE PREVIOUS BUILD"
    rm -f "$BUILD_DIR"/*.o || report_error_code 2
    rm -f "$BUILD_DIR"/*.txt || report_error_code 3
    rm -f "$BUILD_DIR"/*.xml || report_error_code 4
    return 0
}

function report_error_code() {
    echo "ERROR" "$1"
    exit "$1"
}

function build_sources() {
    echo "BUILD SOURCES"
    echo "main.cpp"
    # shellcheck disable=SC2086
    "$GCC" $GCC_FLAGS -c "$SOURCES_DIR"/main.cpp -o "$BUILD_DIR"/main.o
    return 0
}

function build_test_execution_report() {
    # https://github.com/catchorg/Catch2/blob/devel/docs/reporters.md
    # https://docs.sonarqube.org/latest/analysis/coverage/
    echo "BUILD TEST EXECUTION REPORT"
    cd "$TESTS_DIR" || report_error_code 5
    "$TESTS_RUN" --reporter sonarqube --success --out "$BUILD_DIR"/$REPORT_TEST_EXECUTION
    return 0
}

function build_coverage_report() {
    # https://stackoverflow.com/questions/37957583/how-to-use-gcov-with-cmake
    # https://stackoverflow.com/questions/32249416/gcovr-generate-a-0-coverage-report-if-cpp-gcno-and-gcda-files-are-not-present
    # https://gcovr.github.io/guide.html
    echo "BUILD COVERAGE REPORT"
    cd "$BUILD_DIR"/coverage_build || report_error_code 6
    # shellcheck disable=SC2034
    dump=$(find "$TESTS_DIR"/CMakeFiles/tests_target.dir/ -name '*.gcda' -type f -print0 -nowarn | xargs -0 gcov -bclp)
    gcovr . --root "$SOURCES_DIR" --use-gcov-files --keep --sonarqube > "$REPORT_COVERAGE"
    sed -i "s.path=\".path=\"${SOURCES_DIR}/.g" "$REPORT_COVERAGE"
}

function copy_reports() {
    cd "$BUILD_DIR"/reports || report_error_code 7
    cp "../coverage_build/$REPORT_COVERAGE" .
    cp "../$REPORT_TEST_EXECUTION" .
}

function build_all() {
    make_paths
    remove_previous_build
    dump_variables_resolution
    build_sources
    build_test_execution_report
    build_coverage_report
    copy_reports
    print_build_info
    return 0
}


build_all
