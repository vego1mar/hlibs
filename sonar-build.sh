#!/bin/bash

COMPILATOR=/usr/bin/gcc-10
CPP_STANDARD=-std=c++2a
FLAGS="-Wall -Wextra -Wfloat-equal -Wundef -Wshadow=global -Wpointer-arith -Wcast-align=strict -Wwrite-strings
       -Wswitch-enum -Wconversion -Wunreachable-code -Wformat=2 -Wpedantic -Wno-unused-function -Wcast-qual
       -fexceptions -fpie -fwrapv
       -O2
       $CPP_STANDARD -ggdb"
BUILD_DIR=sonar_build
TEST_DIR=cmake-build-debug/catch2_tests/
TEST_RUN=Catch2_tests_run
SQ_TESTS=SonarQube_execution_report.xml


function print_info_and_return_status() {
    local RC=$?

    if [ $RC -ne 0 ]; then
      echo "BUILD FAILURE"
      exit 2
    else
      echo "BUILD SUCCESS"
      exit 0
    fi
}

function remove_previous_build() {
    echo "REMOVE PREVIOUS BUILD"
    rm -rf $BUILD_DIR
}

function report_error() {
    echo "ERROR"
    exit 1
}

function build() {
    echo "PWD=$PWD"

    echo "BUILD SOURCE"
    echo "./$BUILD_DIR"
    mkdir $BUILD_DIR
    "$COMPILATOR" $FLAGS -c src/main.cpp -o $BUILD_DIR/main.o

    echo "BUILD SONARQUBE EXECUTION REPORT"
    cd ./$TEST_DIR || report_error
    ./$TEST_RUN --reporter sonarqube --success --out ../../$BUILD_DIR/$SQ_TESTS
    cd ../../
}


remove_previous_build
build
print_info_and_return_status
