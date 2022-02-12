# naive-linalg-utils

Naive implementations of basic Matrix operations and some typedefs.

This code can serve as a starting point for student projects, or as a baseline code.
It is deliberately naive about how to handle various operations.

The goal of this code is to provide easily tested, easily understood,
known correct matrix code. That code can serve as the baseline for
benchmarks when teaching an HPC class, or it could serve as a baseline of correctness
when teaching a linear algebra class.

## Usage
This code is inappropriate for "real" HPC applications. Use a well-tuned BLAS library.
This code _is_ appropriate as simple example code for computer programming classes.


### Cmake

You can copy the `src` directory to your own CMake project and use
`add_subdirectory(wherever_you_put_it)`.

### Make and others

If you are using another build system, you can just copy `src/naivelinalg.c` and `src/naivelinalg.h` and setup your build/linkage targets. Appropriate copyright headers are already in those files.

## LICENSE

See the LICENSE-2.0.txt file for more information about the license.
You may use any and all parts of this project according to the license, but the project is mostly designed/intended for you to just copy out the `src` directory to your own project.
