#!/bin/bash

PROJECT_PATH=../
SOURCES_DIR=../sources
GCC=/usr/bin/gcc-10
CPP_VER=-std=c++2a
GCC_FLAGS=" \
    -Wall -Wextra -Wfloat-equal -Wundef -Wshadow=global -Wpointer-arith -Wcast-align=strict -Wwrite-strings \
    -Wswitch-enum -Wconversion -Wunreachable-code -Wformat=2 -Wpedantic -Wno-unused-function -Wcast-qual \
    -fexceptions -fpie -fwrapv \
    -O2 $CPP_VER -ggdb"
BUILD_DIR=sonar_build
TESTS_DIR=cmake-build-debug/tests
TESTS_RUN=tests_target
EXECUTION_REPORT_NAME=SonarQube_execution_report.xml


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
        report_error_code 2
    else
        echo "BUILD SUCCESS"
    fi

    return 0
}

function remove_previous_build() {
    echo "REMOVE PREVIOUS BUILD"
    rm -f "$BUILD_DIR"/*.o || report_error_code 3
    rm -f "$BUILD_DIR"/*.txt || report_error_code 4
    rm -f "$BUILD_DIR"/*.xml || report_error_code 5
    return 0
}

function report_error_code() {
    echo "ERROR" "$1"
    exit "$1"
}

function build_sources() {
    echo "BUILD SOURCES"
    echo "main.cpp"
    "$GCC" $GCC_FLAGS -c "$SOURCES_DIR"/main.cpp -o "$BUILD_DIR"/main.o
    return 0
}

function build_sonarqube_report() {
    echo "BUILD SONARQUBE EXECUTION REPORT"
    cd "$TESTS_DIR" || report_error_code 1
    "$TESTS_RUN" --reporter sonarqube --success --out "$BUILD_DIR"/$EXECUTION_REPORT_NAME
    cd "$PROJECT_PATH" || report_error_code 4
    return 0
}

function build_all() {
    make_paths
    remove_previous_build
    dump_variables_resolution
    build_sources
    build_sonarqube_report
    print_build_info
    return 0
}


build_all
