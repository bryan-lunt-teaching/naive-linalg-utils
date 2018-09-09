#include "naivelinalg.h"


bool create_matrix(matrix *target, int N, int M){
	if(N==0 || M==0){
		target->data = NULL;
	}
	target.data = malloc(sizeof(matrix_data_t)*N*M);
}
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
