cmake_minimum_required (VERSION 3.5)
project(naivelinalg-benchmarks VERSION 1.0.0 LANGUAGES CXX)

#Benchmarking
find_package(GBench REQUIRED)
if(GBENCH_FOUND)

else(GBENCH_FOUND)
	message(FATAL_ERROR "DANGER: Could not find GoogleBenchmark")
endif(GBENCH_FOUND)



if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")



#Benchmark targets
add_executable(bench_example "bench_example.cpp")
target_link_libraries(bench_example pthread GoogleBenchmark)

add_subdirectory("optim")

add_executable(bench2 "benchmarks.cpp")
target_link_libraries(bench2 pthread GoogleBenchmark naivelinalg)
target_link_libraries(bench2 hand_optimized naivelinalg)
