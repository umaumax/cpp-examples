cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 11)
project(eigen-hello)

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

add_executable(eigen-hello hello.cpp)
add_executable(matrix-to-quaternion matrix-to-quaternion.cpp)

install(TARGETS eigen-hello DESTINATION bin)
