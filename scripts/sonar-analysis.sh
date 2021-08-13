#!/bin/bash

# https://docs.sonarqube.org/latest/analysis/scan/sonarscanner/
# https://sonarcloud.io/static/cpp/build-wrapper-linux-x86.zip

SQ_UTILS="/media/$USER/386407ee-f7b9-4feb-9332-f84151ba34f3/INSTALL/sonarqube"
BUILD_WRAPPER="$SQ_UTILS/build-wrapper-linux-x86/build-wrapper-linux-x86-64"
SONAR_SCANNER="$SQ_UTILS/sonar-scanner-4.6.2.2472-linux/bin/sonar-scanner"
BUILD_WRAPPER_OUTPUT="sonar_build/bw_outputs"
PROJECT_DIR=../


function make_paths() {
    local reversed_pwd;
    local project_path;

    # Assuming that PWD=${PROJECT_DIR}/scripts.
    reversed_pwd=$(echo "$PWD" | rev)
    project_path=$(echo "${reversed_pwd#*/}" | rev)
    PROJECT_DIR="$project_path"

    local bwOutputsCopy="$BUILD_WRAPPER_OUTPUT"
    BUILD_WRAPPER_OUTPUT="$PROJECT_DIR"/"$bwOutputsCopy"

    echo PROJECT_DIR="$PROJECT_DIR"
    echo BUILD_WRAPPER_OUTPUT="$BUILD_WRAPPER_OUTPUT"
    return 0
}

function run_build_wrapper() {
    local -r sonarBuildScript="$PROJECT_DIR"/scripts/sonar-build.sh
    cd "$PROJECT_DIR"/scripts || exit 2

    if ! $BUILD_WRAPPER --out-dir "$BUILD_WRAPPER_OUTPUT" "$sonarBuildScript"; then
        echo "BUILD FAILED"
        exit 1
    fi
}

function run_sonar_scanner() {
    # sonar-scanner must be run from project root directory
    cd "$PROJECT_DIR" || exit 1
    "$SONAR_SCANNER"
}

function build_and_run() {
    make_paths
    run_build_wrapper
    run_sonar_scanner
}


# cd scripts/ && ./sonar-analysis.sh
build_and_run
