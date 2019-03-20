cmake_minimum_required(VERSION 3.5.1)
project(libxml2_main)

set(${PROJECT_NAME}_SRCS main.cpp)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRCS})

find_package(LibXml2 REQUIRED)
include_directories(${LIBXML2_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${LIBXML2_LIBRARIES})

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
