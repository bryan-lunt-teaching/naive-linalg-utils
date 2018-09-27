cmake_minimum_required (VERSION 2.6)
project (NaiveLinalg)
# The version number.
set (NaiveLinalg_VERSION_MAJOR 0)
set (NaiveLinalg_VERSION_MINOR 1)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
	./src/naivelinalg.h
	./src/naivelinalg.c
	./test/test.c
 )

include_directories(
	./src
)

add_library(NaiveLinalg ./src/naivelinalg.c)

# add the executable
add_executable(test ./test/test.c)
target_link_libraries(test NaiveLinalg)
