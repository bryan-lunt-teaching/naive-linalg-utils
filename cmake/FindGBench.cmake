#Copyright 2022 Bryan J. Lunt <bryan.j.lunt@gmail.com>
#
#Licensed under the Apache License, Version 2.0 (the "License");
#you may not use this file except in compliance with the License.
#You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
#Unless required by applicable law or agreed to in writing, software
#distributed under the License is distributed on an "AS IS" BASIS,
#WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#See the License for the specific language governing permissions and
#limitations under the License.

### Version 2 of this cmake file ###

find_path(GBENCH_INCLUDE_DIR "benchmark/benchmark.h"
	HINTS ${GBENCH_ROOT}/include
	$ENV{GBENCH_ROOT}/include
)

#find_library(GBENCH_CORE_LIBRARIES_STATIC "benchmark")
#find_library(GBENCH_MAIN_LIBRARIES_STATIC "benchmark_main")


find_library(GBENCH_CORE_LIBRARIES_STATIC "${CMAKE_STATIC_LIBRARY_PREFIX}benchmark${CMAKE_STATIC_LIBRARY_SUFFIX}")
find_library(GBENCH_MAIN_LIBRARIES_STATIC "${CMAKE_STATIC_LIBRARY_PREFIX}benchmark_main${CMAKE_STATIC_LIBRARY_SUFFIX}")

#find_library(GBENCH_CORE_LIBRARIES_SHARED "${CMAKE_SHARED_LIBRARY_PREFIX}benchmark${CMAKE_SHARED_LIBRARY_SUFFIX}")
#find_library(GBENCH_MAIN_LIBRARIES_SHARED "${CMAKE_SHARED_LIBRARY_PREFIX}benchmark_main${CMAKE_SHARED_LIBRARY_SUFFIX}")


#mark_as_advanced(GBENCH_INCLUDE_DIR)

if(GBENCH_CORE_LIBRARIES_STATIC AND GBENCH_MAIN_LIBRARIES_STATIC AND GBENCH_INCLUDE_DIR)
	set(GBENCH_FOUND "TRUE")
	set(GBENCH_BOTH_LIBRARIES ${GBENCH_CORE_LIBRARIES_STATIC} ${GBENCH_MAIN_LIBRARIES_STATIC})
	set(GBENCH_INCLUDE_DIRS ${GBENCH_INCLUDE_DIR})
	#message("GBENCH: ${GBENCH_LIBRARY} ${GBENCH_LIBRARY_MAIN}")
	
	add_library(GoogleBenchmark::main STATIC IMPORTED )
	set_target_properties(GoogleBenchmark::main PROPERTIES IMPORTED_LOCATION ${GBENCH_MAIN_LIBRARIES_STATIC})
	#target_link_libraries(google-benchmark INTERFACE ${GBENCH_LIBRARIES_STATIC} ${GBENCH_MAIN_LIBRARIES_STATIC})
	#target_include_directories(google-benchmark INTERFACE ${GBENCH_INCLUDE_DIRS})
	
	add_library(GoogleBenchmark::core STATIC IMPORTED )
	set_target_properties(GoogleBenchmark::core PROPERTIES IMPORTED_LOCATION ${GBENCH_CORE_LIBRARIES_STATIC})
	#target_link_libraries(GoogleBenchmark::core INTERFACE ${GBENCH_LIBRARIES_STATIC})
	#target_include_directories(GoogleBenchmark::core INTERFACE ${GBENCH_INCLUDE_DIRS})
	
	add_library(GoogleBenchmark INTERFACE)
	target_link_libraries(GoogleBenchmark INTERFACE GoogleBenchmark::core)
	#TODO: Make linking (and even finding) main optional.
	target_link_libraries(GoogleBenchmark INTERFACE GoogleBenchmark::main)
	
	
	message("GBENCH: Found! ${GBENCH_BOTH_LIBRARIES} ${GBENCH_INCLUDE_DIRS}")
else()
	message("GBENCH: Not Found")
endif()
