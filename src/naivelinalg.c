#include "naivelinalg.h"


bool create_matrix(matrix *target, int N, int M){
target->N = N;
	target->M = M;

	if(N==0 || M==0){
		target->data = NULL;
	}

	target->data = malloc(sizeof(matrix_data_t)*N*M);
	if(NULL == target->data){
		target->N = 0;
		target->M = 0;
		return false;
	}

	target->N = N;
	target->M = M;
	return true;
}



bool destroy_matrix(matrix *target){
	free(target->data);
	target->data = NULL;
	target->N = 0;
	target->M = 0;
	return true;
}

//implement such that target and source can be the same or different.
bool transpose(matrix *target, matrix *source);

bool value_matrix(matrix *target, double val);
bool eye_matrix(matrix *target);

//target and source may be the same.
bool scalar_multiply(matrix *target, matrix *source, double val);
bool scalar_add(matrix *target, matrix *source, double val);

bool copy_matrix(matrix *target, matrix *source);
bool hadamard_multiply(matrix *target, matrix *A, matrix *B);

bool naive_multiply(matrix *target, matrix *A, matrix *B);
