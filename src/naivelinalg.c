#include <stdlib.h>
#include <math.h>

#include "naivelinalg.h"


bool matrix_create(matrix *target, int N, int M){
	target->N = N;
	target->M = M;

	if(N==0 || M==0){
		target->data = NULL;
	}

	//target->data = (matrix_data_t*)malloc(sizeof(matrix_data_t)*N*M);
	//target->data = (matrix_data_t*)calloc(sizeof(matrix_data_t),N*M);
	/*Neither of the above work because we need to be alligned
		on a cache line boundary
	*/
	
	int memalign_result = posix_memalign(
						(void**)(&(target->data)),
	 					NAIVE_LINALG_ALIGNMENT,
						sizeof(matrix_data_t)*N*M
					);
	//I don't see that the documentation promises to return NULL on error.
	//ERROR allocating aligned memory.
	if(0 != memalign_result || NULL == target->data){
		target->data = NULL; //memalign_result was not 0.
		target->N = 0;
		target->M = 0;
		return false;
	}

	target->N = N;
	target->M = M;
	return true;
}



bool matrix_destroy(matrix *target){
	free(target->data);
	target->data = NULL;
	target->N = 0;
	target->M = 0;
	return true;
}

void matrix_fprint(FILE *outfile, matrix *toprint){
	for(int i=0;i<toprint->N;i++){
		fprintf(outfile, "[ ");
		for(int j=0;j<toprint->M;j++){
			fprintf(outfile, "%e ", MATPTR_ELEMENT(toprint,i,j));
		}
		fprintf(outfile, "]\n");
	}
}


bool copy_matrix(matrix *target, matrix *source){
	if(NULL == target->data || NULL == source->data){
		return false;
	}

	if(target->N*target->M != source->N*source->M){
		matrix_destroy(target);
		bool recreate = matrix_create(target,source->N, source->M);
		if(!recreate)
			return false;
	}

	target->N = source->N;
	target->M = source->M;

	for(int i = 0;i<source->N;i++)
		for(int j = 0;j<source->M;j++)
			MATPTR_ELEMENT(target, i, j) = MATPTR_ELEMENT(source, i, j);

	return true;
}


//implement such that target and source can be the same or different.
static bool transpose_self(matrix *justme){
	matrix tmp_matrix;
	
	//TODO: WTF?
	//This library is _meant_ to be "naive", but this is not necessary.
	bool tmpcreate = matrix_create(&tmp_matrix, justme->M, justme->N);
	if(!tmpcreate)
		return false;

	for(int i = 0;i<justme->N;i++)
		for(int j = 0;j<justme->M;j++)
			MATRIX_ELEMENT(tmp_matrix,j,i) = MATPTR_ELEMENT(justme,i,j);

	matrix_destroy(justme);
	*justme = tmp_matrix;

	return true;
}

bool matrix_transpose(matrix *target, matrix *source){

	if(NULL==target->data ||
		NULL==source->data ||
		target->N != source->M || target->M != source->N){
		return false;
	}

	if(target == source){
		return transpose_self(source);
	}


	target->N = source->M;
	target->M = source->N;

	for(int i = 0;i<source->N;i++)
		for(int j = 0;j<source->M;j++)
			MATPTR_ELEMENT(target,j,i) = MATPTR_ELEMENT(source,i,j);

	return true;
}

bool matrix_set(matrix *target, matrix_data_t val){
	if(NULL == target->data)
		return false;
	int size = target->N*target->M;
	for(int i=0; i<size; i++)
		target->data[i] = val;
	return true;
}

//TODO: An optional parameter that tells which diagonal to set
bool matrix_set_diag(matrix *target, matrix_data_t val){
	if(NULL == target->data)
		return false;
	int main_diag_size = target->N < target->M ? target->N : target->M;

	for(int k=0;k<main_diag_size;k++)
		MATPTR_ELEMENT(target,k,k) = val;

	return true;
}

bool matrix_eye(matrix *target){
	if(NULL == target->data)
		return false;

	for(int i = 0;i<target->N;i++)
		for(int j = 0;j<target->M;j++)
			if(i == j)
				MATPTR_ELEMENT(target,i,j) = 1.0;
			else
				MATPTR_ELEMENT(target,i,j) = 0.0;

	return true;
}

//target and source may be the same.
bool matrix_mult_scalar(matrix *target, matrix *source, matrix_data_t val){
	//check that they are both allocated matrices
	if(NULL == target->data || NULL == source->data)
		return false;
	//check that they are the same shape
	if(target->N != source->N || target->M != source->M){
		return false;
	}


	for(int i = 0;i<source->N;i++)
		for(int j = 0;j<source->M;j++)
				MATPTR_ELEMENT(target,i,j) = val*MATPTR_ELEMENT(source,i,j);

	return true;//Success
}


bool matrix_add_scalar(matrix *target, matrix *source, matrix_data_t val){
	//check that they are both allocated matrices
	if(NULL == target->data || NULL == source->data)
		return false;
	//check that they are the same shape
	if(target->N != source->N || target->M != source->M){
		return false;
	}


	for(int i = 0;i<source->N;i++)
		for(int j = 0;j<source->M;j++)
				MATPTR_ELEMENT(target,i,j) = val + MATPTR_ELEMENT(source,i,j);

	return true;//Success
}

bool matrix_hadamard_mult(matrix *target, matrix *A, matrix *B){
	return false;//not implemented
}

bool matrix_mult_naive(matrix *target, matrix *A, matrix *B){
	/* It turns out, after extensive comparative testing,
		the the compiler can optimize this naive code to be as fast as
		a fairly carefully written version (of the same algorithm)
		To get better performance, you need to use something like tiled mult.
		DONT WASTE TIME OPTIMIZING THIS CODE, USE -O3
		
		Total hours spent self-doubting here: 6hrs
			2022-2-4 Bryan J. Lunt 6hrs
	*/
	
	//Check that inputs are valid and that we aren't outputting to one of the inputs.
	if(NULL == target->data || NULL == A->data || NULL == B->data) return false;
	if(target->data == A->data || target->data == B->data) return false;

	//Check that shapes are compatible for multiplication.
	if(A->M != B->N) return false;
	if(target->N != A->N || target->M != B->M) return false;

	//Making this explicitly a register is
	//the only necessary consession toward hand optimization.
	register matrix_data_t tmpval = 0.0;

	//Yes, we are dereferencing pointers. The compiler optimizes that away.
	for(int i = 0;i<A->N;i++) //Every row of output
		for(int j = 0;j<B->M;j++){ //Every column of output
			
			//The dot product of a row of A with a column of B
			//MATPTR_ELEMENT uses pointer dereferencing but complier optimizes.
			tmpval = 0.0;
			for(int k = 0;k<A->M;k++)
				tmpval += MATPTR_ELEMENT(A,i,k)*MATPTR_ELEMENT(B,k,j);
			MATPTR_ELEMENT(target,i,j) = tmpval;
		}

	return true;//Success
}

bool matrix_equality(matrix *lhs, matrix *rhs, double epsilon) {
	if(lhs->N != rhs->N || lhs->M != rhs->M)
		return false;//Not the same shape

	int END = lhs->N*lhs->M;//Just scan as a 1d array. Actually, fix this.
	matrix_data_t *ldata, *rdata;
	ldata = lhs->data;
	rdata = rhs->data;
	for(int i = 0;i<END;i++){
		if( fabs(ldata[i] - rdata[i]) > epsilon ) { return false; }
	}
	return true;//Success
}
