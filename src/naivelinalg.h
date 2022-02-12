/*
Copyright 2018 Bryan J. Lunt <bryan.j.lunt@gmail.com>

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef NAIVELINALG_H
#define NAIVELINALG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

//TODO: automagically detect the cache line size of the target machine.
#define NAIVE_LINALG_ALIGNMENT 64

typedef double matrix_data_t; //float = 32bits = 4bytes ; double = 64bits = 8 bytes.

typedef struct {
	matrix_data_t *data; /*!< A 1D array containing all the matrix data in row-major order */
	int N; /*!< The number of rows in the matrix */
	int M; /*!< The number of logical columns in the matrix */
	int Mallocated; /*!< The number of columns actually allocated in order to make every row start on a boundary. */
} matrix;

#define MATRIX_IS_ALLOCATED(matr) (NULL != (matr).data)
#define MATRIX_SIZE(matr) ((matr).N * (matr).M)
#define MATRIX_STORAGE_BYTES(matr) ((matr).N*(matr).Mallocated*sizeof(matrix_data_t))

#define MATRIX_SAME_SHAPE(lhs,rhs) ((lhs).N == (rhs).N && (lhs).M == (rhs).M)

#define ARRAY_ELEMENT(dptr, nRows, mCols_alloc, i, j) dptr[i*mCols_alloc + j]
#define MATPTR_ELEMENT(matptr, i, j) ARRAY_ELEMENT(matptr->data, matptr->N, matptr->Mallocated, i, j)
#define MATRIX_ELEMENT(matr, i, j) ARRAY_ELEMENT((matr).data, (matr).N, (matr).Mallocated, i, j)

bool matrix_create(matrix *target, const int N /*!< N rows */, const int M /*!< M cols */);
bool matrix_destroy(matrix *target);
bool matrix_copy(matrix *target, const matrix *source);

//implement such that target and source can be the same or different.
bool matrix_transpose(matrix *target, matrix *source);

bool matrix_set(matrix *target, const matrix_data_t val);
bool matrix_set_diag(matrix *target, const matrix_data_t val); //TODO: An optional parameter that tells which diagonal to set
bool matrix_eye(matrix *target);

//target and source may be the same.
bool matrix_mult_scalar(matrix *target, matrix *source, const matrix_data_t val);
bool matrix_add_scalar(matrix *target, matrix *source, const matrix_data_t val);

bool matrix_hadamard_mult(matrix *target, matrix *A, matrix *B);

bool matrix_mult_naive(matrix *target, const matrix *A, const matrix *B);

void matrix_fprint(FILE *outfile, const matrix *toprint);

bool matrix_equality(const matrix *lhs, const matrix *rhs, const double epsilon);

#ifdef __cplusplus
}
#endif

#endif
