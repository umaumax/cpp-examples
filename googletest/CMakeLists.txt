cmake_minimum_required(VERSION 3.5.1)
project(googletest_example)

enable_testing()
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-O2 -Wall ${CMAKE_CXX_FLAGS}")

add_executable(${PROJECT_NAME} ./test/main.cpp)
target_link_libraries(${PROJECT_NAME} ${GTEST_LIBRARIES} ${GMOCK_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
target_include_directories(${PROJECT_NAME}
  PRIVATE
  ${GTEST_INCLUDE_DIRS}
  ${GMOCK_INCLUDE_DIRS}
  )

add_test(NAME ${PROJECT_NAME} COMMAND $<TARGET_FILE:${PROJECT_NAME}>)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
