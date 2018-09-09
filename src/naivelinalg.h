#ifndef NAIVELINALG_H
#define NAIVELINALG_H

typedef double matrix_data_t;

typedef struct {
	matrix_data_t *data = NULL;
	int N = 0;
	int M = 0;
} matrix;

bool create_matrix(matrix *target, int N, int M);
bool destroy_matrix(matrix *target);

//implment such that target and source can be the same or different.
bool transpose(matrix *target, matrix *source);
bool transpose(matrix *target); // calls above.

bool value_matrix(matrix *target, double val);
bool eye_matrix(matrix *target);

//target and source may be the same.
bool scalar_multiply(matrix *target, matrix *source, double val);
bool scalar_add(matrix *target, matrix *source, double val);

bool copy_matrix(matrix *target, matrix *source);
bool hadamard_multiply(matrix *target, matrix *A, matrix *B);

bool naive_multiply(matrix *target, matrix *A, matrix *B);


#endif
