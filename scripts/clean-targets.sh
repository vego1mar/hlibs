#!/bin/bash

TESTS_SOURCE=tests
TESTS_TARGET=/cmake-build-debug/$TESTS_SOURCE/CMakeFiles/tests_target.dir
PROJECT_PATH=../


function report_error_code() {
    echo "ERROR" "$1"
    exit "$1"
}

function remove_file_by_extension() {
    rm -f ./*."$1"
}

function make_paths() {
    local reversed_pwd;
    local project_path;
    local tests_target_copy;

    reversed_pwd=$(echo "$PWD" | rev)
    project_path=$(echo "${reversed_pwd#*/}" | rev)
    tests_target_copy="$TESTS_TARGET"
    PROJECT_PATH="$project_path"
    TESTS_TARGET="$project_path""$tests_target_copy"

    echo PROJECT_PATH="$PROJECT_PATH"
    echo TESTS_TARGET="$TESTS_TARGET"
}

function clean_gcc_coverage_files() {
    local -r paths=(
        "$TESTS_TARGET"/facilities
        "$TESTS_TARGET"/io
        "$TESTS_TARGET"/io/file
        "$TESTS_TARGET"/io/file_reader
        "$TESTS_TARGET"/io/file_writer
        "$TESTS_TARGET"/logging
        "$TESTS_TARGET"/standard
        "$TESTS_TARGET"/types
    )

    for i in {0..7}; do
        cd "${paths[i]}" || report_error_code 1
        remove_file_by_extension gcda
        remove_file_by_extension gcno
    done
}

function clean_outputs_directory() {
    local outputsPath="$PROJECT_PATH"/outputs
    cd "$outputsPath" || report_error_code 2
    remove_file_by_extension txt
}

function clean_sonar_build_directory() {
    local bwOutputsPath="$PROJECT_PATH"/sonar_build/bw_outputs
    rm -rf "$bwOutputsPath" || report_error_code 4
    local sonarBuildPath="$PROJECT_PATH"/sonar_build
    cd "$sonarBuildPath" || report_error_code 3
    rm -f ./*
}

function clean_all() {
    make_paths
    clean_gcc_coverage_files
    clean_outputs_directory
    clean_sonar_build_directory
}


clean_all
