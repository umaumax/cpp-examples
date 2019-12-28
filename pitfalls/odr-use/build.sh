#!/usr/bin/env bash

BLACK=$'\e[30m' RED=$'\e[31m' GREEN=$'\e[32m' YELLOW=$'\e[33m' BLUE=$'\e[34m' PURPLE=$'\e[35m' LIGHT_BLUE=$'\e[36m' WHITE=$'\e[37m' GRAY=$'\e[90m' DEFAULT=$'\e[0m'

function build() {
  $CXX --version |& grep -e version -e "g++"
  $CXX "$@" main.cpp
}

if [[ $(uname) == "Darwin" ]]; then
  compilers=(clang++ g++ g++-7 g++-9)
else
  compilers=(g++ clang++ clang++-3.8 clang++-5.0)
fi
for compiler in "${compilers[@]}"; do
  for ((i = 0; i <= 3; i++)); do
    CXX="$compiler" build "-std=c++11" "-O${i}"
    if [[ $? == 0 ]]; then
      echo "${BLUE}CXX: $compiler, opt: -std=c++11 -O${i}: OK${DEFAULT}"
    else
      echo "${RED}CXX: $compiler, opt: -std=c++11 -O${i}: NG${DEFAULT}"
    fi
  done
done
