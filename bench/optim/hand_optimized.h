#ifndef HAND_OPTIMIZED_H
#define HAND_OPTIMIZED_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "naivelinalg.h"

bool matrix_mult_naive_A(matrix* const target, matrix* const A, matrix* const B);
bool matrix_mult_naive_B(matrix* const target, matrix* const A, matrix* const B);
bool matrix_mult_naive_C(matrix* const target, matrix* const A, matrix* const B);
bool matrix_mult_naive_D(matrix* const target, matrix* const A, matrix* const B);



#ifdef __cplusplus
}
#endif

#endif
