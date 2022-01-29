cmake_minimum_required (VERSION 3.5)
project(NaiveLinalg VERSION 0.2.0 LANGUAGES C CXX)

# Location of additional cmake modules
#
set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${PROJECT_SOURCE_DIR}/cmake
    )

add_library(NaiveLinalg STATIC "./src/naivelinalg.c" "./src/naivelinalg.h")
target_include_directories(NaiveLinalg PUBLIC "./src")

# add the executable
add_executable(test ./test/test.c)
target_link_libraries(test NaiveLinalg)


add_executable(testcpp ./test/testcpp.cpp)
target_link_libraries(testcpp NaiveLinalg)
