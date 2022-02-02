cmake_minimum_required (VERSION 3.5)
project(NaiveLinalg VERSION 0.2.0 LANGUAGES C)

add_library(naivelinalg STATIC "naivelinalg.c" "naivelinalg.h")
target_include_directories(naivelinalg PUBLIC ".")
target_link_libraries(naivelinalg m)
