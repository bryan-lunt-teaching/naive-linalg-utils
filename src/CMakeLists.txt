cmake_minimum_required (VERSION 3.5)
project(NaiveLinalg VERSION 0.2.0 LANGUAGES C)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_C_FLAGS "-Wall -Wextra")
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_RELEASE "-O3")

add_library(naivelinalg STATIC "naivelinalg.c" "naivelinalg.h")
target_include_directories(naivelinalg PUBLIC ".")
target_link_libraries(naivelinalg m)
