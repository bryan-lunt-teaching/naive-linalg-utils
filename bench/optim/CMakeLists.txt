cmake_minimum_required (VERSION 3.5)
project(NaiveLinalg-bench-optim VERSION 0.2.0 LANGUAGES C)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_C_FLAGS "-Wall -Wextra")
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_RELEASE "-O3")

#hand optimized versions
add_library(hand_optimized STATIC "hand_optimized.c" "hand_optimized.h")
target_include_directories(hand_optimized PUBLIC ".")
target_link_libraries(hand_optimized naivelinalg)
