#!/usr/bin/env bash
g++ -std=c++11 -g -O0 --coverage main.cpp
./a.out
gcov main.cpp
cat main.cpp.gcov | awk '/PASS_COV$/{pass_cov=1} /^ *#####:/{if (pass_cov==0) {printf "\033[31m%s\033[0m\n", $0;} else {print $0}; } ! /^ *#####:/{pass_cov=0; print $0}'
