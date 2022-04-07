#!/bin/bash

PROJECT_PATH=../


function report_error_code() {
    echo "ERROR" "$1"
    exit "$1"
}

function remove_file_by_extension() {
    rm ./*."$1"
}

function make_paths() {
    local reversed_pwd;
    local project_path;

    reversed_pwd=$(echo "$PWD" | rev)
    project_path=$(echo "${reversed_pwd#*/}" | rev)
    PROJECT_PATH="$project_path"

    echo PROJECT_PATH="$PROJECT_PATH"
}

function clean_build_directory() {
    local sonarBuildPath="$PROJECT_PATH"/sonar_build

    cd "$sonarBuildPath" || report_error_code 1
    remove_file_by_extension o
    remove_file_by_extension txt
    remove_file_by_extension xml

    cd "$sonarBuildPath/coverage_build" || report_error_code 2
    remove_file_by_extension xml
    remove_file_by_extension gcov

    cd "$sonarBuildPath/reports" || report_error_code 3
    remove_file_by_extension xml

    cd "$sonarBuildPath/.scannerwork" || report_error_code 4
    remove_file_by_extension txt
    remove_file_by_extension sonar_lock

    cd "$sonarBuildPath/bw_outputs" || report_error_code 5
    remove_file_by_extension log
    remove_file_by_extension json
}

function clean_all() {
    make_paths
    clean_build_directory
}


clean_all
