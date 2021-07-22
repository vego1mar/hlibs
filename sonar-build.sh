#!/bin/bash

COMPILATOR=/usr/bin/gcc
CPP_STANDARD=-std=c++2a
FLAGS="-Wall -Wextra -Wfloat-equal -Wundef -Wshadow=global -Wpointer-arith -Wcast-align=strict -Wwrite-strings -Wcast-qual
       -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code -Wformat=2 -Wpedantic -Werror
       -fexceptions -fpie -fwrapv
       -O2
       $CPP_STANDARD -ggdb"


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
    rm -rf sonar-build
}

function build() {
   echo "BUILDING"

   mkdir sonar-build
   echo "./sonar-build"
   "$COMPILATOR" $FLAGS -c src/main.cpp -o sonar-build/main.o

   # echo "./sonar-build/file"
   # mkdir sonar-build/file
}

remove_previous_build
build
print_build_info
