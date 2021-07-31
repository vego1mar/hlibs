#!/bin/bash

COMPILATOR=/usr/bin/gcc-10
CPP_STANDARD=-std=c++2a
FLAGS="-Wall -Wextra -Wfloat-equal -Wundef -Wshadow=global -Wpointer-arith -Wcast-align=strict -Wwrite-strings
       -Wswitch-enum -Wconversion -Wunreachable-code -Wformat=2 -Wpedantic -Wno-unused-function -Wcast-qual
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

}


remove_previous_build
build
print_build_info
