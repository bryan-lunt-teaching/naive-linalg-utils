#ifndef NAIVELINALG_H
#define NAIVELINALG_H

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

typedef double matrix_data_t;

typedef struct {
	matrix_data_t *data;
	int N;
	int M;
} matrix;

bool create_matrix(matrix *target, int N, int M);
bool destroy_matrix(matrix *target);

//implment such that target and source can be the same or different.
bool transpose(matrix *target, matrix *source);

bool value_matrix(matrix *target, double val);
bool eye_matrix(matrix *target);

//target and source may be the same.
bool scalar_multiply(matrix *target, matrix *source, double val);
bool scalar_add(matrix *target, matrix *source, double val);

bool copy_matrix(matrix *target, matrix *source);
bool hadamard_multiply(matrix *target, matrix *A, matrix *B);

bool naive_multiply(matrix *target, matrix *A, matrix *B);


#endif
