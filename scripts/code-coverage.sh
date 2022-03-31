#!/bin/bash

# * find . -name '*.gcda' | xargs gcov
#
#  gcovr .../path/To/GCDA -r .../path/to/src/ [rest desired flags]
#  gcovr --sonarqube coverage.xml
#  gcovr sonar_build/gcc_coverage/ --root sources/ -g --keep --sonarqube > sonar_build/gcovr_sonarqube.xml
#
#  gcov -b -l -p -c *.gcno
#  gcovr -g -k -r . --html --html-details -o tp.html
#
#  code-coverage.sh:
#  * Catch3 test execution report
#  * gcovr code coverage → SonarQube + XML + native
#
#  * update clean-target.sh
#  * update sonar-build.sh
#
#  * build update-search.sh
#
#*/

