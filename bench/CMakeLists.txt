cmake_minimum_required (VERSION 3.5)
project(naivelinalg-benchmarks VERSION 1.0.0 LANGUAGES CXX)




if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


#Benchmarking
find_package(GBench REQUIRED)

#Benchmark targets
add_executable(bench_example "bench_example.cpp")
target_link_libraries(bench_example GBench::gbench GBench::gbench_main)

add_subdirectory("optim")

add_executable(bench2 "benchmarks.cpp")
target_link_libraries(bench2 hand_optimized naivelinalg)
target_link_libraries(bench2 pthread GBench::gbench GBench::gbench_main)
