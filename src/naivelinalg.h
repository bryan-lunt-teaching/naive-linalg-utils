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

typedef double matrix_data_t;

typedef struct {
	matrix_data_t *data;
	int N;
	int M;
} matrix;

#define ARRAY_ELEMENT(dptr, nRows, mCols, i, j) dptr[i*mCols + j]
#define MATPTR_ELEMENT(matptr, i, j) ARRAY_ELEMENT(matptr->data, matptr->N, matptr->M, i, j)
#define MATRIX_ELEMENT(matr, i, j) ARRAY_ELEMENT(matr.data, matr.N, matr.M, i, j)

bool matrix_create(matrix *target, int N, int M);
bool matrix_destroy(matrix *target);
bool copy_matrix(matrix *target, matrix *source);

//implement such that target and source can be the same or different.
bool matrix_transpose(matrix *target, matrix *source);

bool matrix_set(matrix *target, matrix_data_t val);
bool matrix_set_diag(matrix *target, matrix_data_t val); //TODO: An optional parameter that tells which diagonal to set
bool matrix_eye(matrix *target);

//target and source may be the same.
bool matrix_mult_scalar(matrix *target, matrix *source, matrix_data_t val);
bool matrix_add_scalar(matrix *target, matrix *source, matrix_data_t val);

bool matrix_hadamard_mult(matrix *target, matrix *A, matrix *B);

bool matrix_mult_naive(matrix *target, matrix *A, matrix *B);

void matrix_fprint(FILE *outfile, matrix *toprint);

bool matrix_equality(matrix *lhs, matrix *rhs, double epsilon);

#ifdef __cplusplus
}
#endif

#endif
