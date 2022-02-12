cmake_minimum_required (VERSION 3.5)
project(NaiveLinalg VERSION 0.2.0 LANGUAGES C CXX)

# Location of additional cmake modules
#
set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${PROJECT_SOURCE_DIR}/cmake
    )

include(common)

#add_library(naivelinalg STATIC "./src/naivelinalg.c" "./src/naivelinalg.h")
#target_include_directories(naivelinalg PUBLIC "./src")
#target_link_libraries(naivelinalg m)
add_subdirectory("src")

add_subdirectory("test")

add_subdirectory("bench")
