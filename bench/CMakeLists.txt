cmake_minimum_required (VERSION 3.5)
project(naivelinalg-benchmarks VERSION 1.0.0 LANGUAGES CXX)

#Benchmarking
find_package(GBench REQUIRED)
if(GBENCH_FOUND)

else(GBENCH_FOUND)
	message(FATAL_ERROR "DANGER: Could not find GoogleBenchmark")
endif(GBENCH_FOUND)


#Benchmark targets
add_executable(bench_example "bench_example.cpp")
target_link_libraries(bench_example pthread GoogleBenchmark)
