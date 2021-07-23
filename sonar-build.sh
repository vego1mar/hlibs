#!/bin/bash

COMPILATOR=/usr/bin/gcc
CPP_STANDARD=-std=c++2a
FLAGS="-Wall -Wextra -Wfloat-equal -Wundef -Wshadow=global -Wpointer-arith -Wcast-align=strict -Wwrite-strings -Wcast-qual
       -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code -Wformat=2 -Wpedantic -Werror
       -fexceptions -fpie -fwrapv
       -O2
       $CPP_STANDARD -ggdb"
BUILD_DIR=sonar_build


function print_build_info() {
    local RC=$?

    if [ $RC -ne 0 ]; then
      echo "BUILD FAILURE"
    else
      echo "BUILD SUCCESS"
    fi
}

function remove_previous_build() {
    echo "REMOVE PREVIOUS BUILD"
    rm -rf $BUILD_DIR
}

function build() {
    echo "BUILDING"

    echo "./$BUILD_DIR"
    mkdir $BUILD_DIR
    "$COMPILATOR" $FLAGS -c src/main.cpp -o $BUILD_DIR/main.o

    local DATE_TIME_DIR="./$BUILD_DIR/date_time"
    echo $DATE_TIME_DIR
    # 'date_time' has only headers for now

    local FILE_DIR="./$BUILD_DIR/file"
    echo $FILE_DIR
    # 'file' has only headers for now

    local INTERFACES_DIR="./$BUILD_DIR/interfaces"
    echo $INTERFACES_DIR
    # 'interfaces' has only headers for now

    local LOGGING_DIR="./$BUILD_DIR/logging"
    echo $LOGGING_DIR
    # 'logging' has only headers for now

}


remove_previous_build
build
print_build_info
