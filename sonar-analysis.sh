#!/bin/bash

# https://docs.sonarqube.org/latest/analysis/scan/sonarscanner/
# https://sonarcloud.io/static/cpp/build-wrapper-linux-x86.zip

PARTITION_UUID="386407ee-f7b9-4feb-9332-f84151ba34f3"
PRE_PATH="/media/$USER/$PARTITION_UUID/INSTALL/sonarqube"
SCANNER_VERSION="4.6.2.2472"
BUILD_WRAPPER="$PRE_PATH/build-wrapper-linux-x86/build-wrapper-linux-x86-64"
SONAR_SCANNER="$PRE_PATH/sonar-scanner-$SCANNER_VERSION-linux/bin/sonar-scanner"
BW_OUTPUT="bw_outputs"
SONAR_BUILD="sonar-build.sh"


$BUILD_WRAPPER --out-dir $BW_OUTPUT ./$SONAR_BUILD
$SONAR_SCANNER
