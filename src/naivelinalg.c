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

#include <stdlib.h>
#include <math.h>

#include "naivelinalg.h"


bool matrix_create(matrix *target, const int N, const int M){
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
	
	/* TODO: Automatically detect cache line size of machine. (At runtime?) */
	/* TODO: Should large matrices be aligned on page boundaries? Find out. */
	
	/* TODO: Could be a problem is the alignment size and the datatype size are coprime.
			But that will basically never happen. */
	int extra_bytes_per_row = (NAIVE_LINALG_ALIGNMENT - ((sizeof(matrix_data_t)*M)%NAIVE_LINALG_ALIGNMENT));
	int M_to_allocate = M + extra_bytes_per_row/sizeof(matrix_data_t) ; //Expressed as number of datatype.
	
	
	int memalign_result = posix_memalign(
						(void**)(&(target->data)),
	 					NAIVE_LINALG_ALIGNMENT,
						sizeof(matrix_data_t)*N*M_to_allocate
					);
	//I don't see that the documentation promises to return NULL on error.
	//ERROR allocating aligned memory.
	if(0 != memalign_result || NULL == target->data){
		target->data = NULL; //memalign_result was not 0.
		target->N = 0;
		target->M = 0;
		target->Mallocated = 0;
		return false;
	}

	target->N = N;
	target->M = M;
	target->Mallocated = M_to_allocate;
	return true;
}

bool matrix_destroy(matrix *target){
	free(target->data);
	target->data = NULL;
	target->N = 0;
	target->M = 0;
	target->Mallocated = 0;
	return true;
}

void matrix_fprint(FILE *outfile, const matrix *toprint){
	for(int i=0;i<toprint->N;i++){
		fprintf(outfile, "[ ");
		for(int j=0;j<toprint->M;j++){
			fprintf(outfile, "%e ", MATPTR_ELEMENT(toprint,i,j));
		}
		fprintf(outfile, "]\n");
	}
}


bool matrix_copy(matrix *target, const matrix *source){
	if(NULL == target || NULL == source){
		return false;
	}
	if(NULL == target->data || NULL == source->data){
		return false;
	}
	/* Deliberately omit test to prevent self-copy */
	
	if(target->N != source->N || target->M != source->M){
		return false;
	}

	//target->N = source->N;
	//target->M = source->M;

	//Deliberately Naive. Memcopy would be faster.
	for(int i = 0;i<source->N;i++)
		for(int j = 0;j<source->M;j++)
			MATPTR_ELEMENT(target, i, j) = MATPTR_ELEMENT(source, i, j);

	return true;
}


bool matrix_transpose(matrix *target, matrix *source){
	
	//Check that the target is allocated and the right shape.
	//NO resizing/reshaping. K.I.S.S.
	if(NULL==target->data ||
		NULL==source->data ||
		target->N != source->M || target->M != source->N){
		return false;
	}

	//Inplace, square only
	if(target == source){
		if (target->N != target->M){ return false;}//Not square //yes, check again.
		register matrix_data_t from_ij;
		register matrix_data_t from_ji;
		for(int i = 0;i<source->N;i++)
			for(int j = i+1;j<source->M;j++){
				from_ij = MATPTR_ELEMENT(source,i,j);
				from_ji = MATPTR_ELEMENT(source,j,i);
				MATPTR_ELEMENT(target,j,i) = from_ij;
				MATPTR_ELEMENT(target,i,j) = from_ji;
			}
		return true;
	}
	
	
	//out of place, any shape.
	for(int i = 0;i<source->N;i++)
		for(int j = 0;j<source->M;j++)
			MATPTR_ELEMENT(target,j,i) = MATPTR_ELEMENT(source,i,j);

	return true;
}

bool matrix_set(matrix *target, const matrix_data_t val){
	if(NULL == target->data)
		return false;
	
	for(int i=0; i<target->N; i++)
		for(int j=0; j<target->M; j++)
			MATPTR_ELEMENT(target,i,j) = val;
	return true;
}

//TODO: An optional parameter that tells which diagonal to set
bool matrix_set_diag(matrix *target, const matrix_data_t val){
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
			MATPTR_ELEMENT(target,i,j) = (i == j) ? 1.0 : 0.0; //1.0 on main diagonal, 0.0 otherwise

	return true;
}

//target and source may be the same.
bool matrix_mult_scalar(matrix *target, matrix *source, const matrix_data_t val){
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


bool matrix_add_scalar(matrix *target, matrix *source, const matrix_data_t val){
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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
bool matrix_hadamard_mult(matrix *target, matrix *A, matrix *B){
	if(NULL == target->data || NULL == A->data || NULL == B->data)
		return false; //Null matrices
	
	//Check shapes
	if( (target->N != A->N || A->N != B->N) ||
 		(target->M != A->M || A->M != B->M) ){
			return false; //not all the same shape.
		}
	//Don't care about aliasing.
	
	for(int i = 0;i<target->N;i++){
		for(int j = 0;j<target->M;j++){
			MATPTR_ELEMENT(target,i,j) = MATPTR_ELEMENT(A,i,j)*MATPTR_ELEMENT(B,i,j);
		}
	}
	
	return true;
}
#pragma GCC diagnostic pop

bool matrix_mult_naive(matrix *target, const matrix *A, const matrix *B){
	/* It turns out, after extensive comparative testing,
		the the compiler can optimize this naive code to be as fast as
		a fairly carefully written version (of the same algorithm)
		To get better performance, you need to use something like tiled mult.
		DONT WASTE TIME OPTIMIZING THIS CODE, USE -O3
		
		Total hours spent self-doubting here: 6hrs
			2022-2-4 Bryan J. Lunt 6hrs
	*/
	
	//Check that inputs are valid and that we aren't outputting to one of the inputs.
	if(NULL == target->data || NULL == A->data || NULL == B->data) return false; //Check validity
	if(target->data == A->data || target->data == B->data) return false; //Check for deeper aliasing

	//Check that shapes are compatible for multiplication.
	if(A->M != B->N) return false; //A and B compatible.
	if(target->N != A->N || target->M != B->M) return false; //output correct shape

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

bool matrix_equality(const matrix *lhs, const matrix *rhs, const double epsilon) {
	if(lhs->N != rhs->N || lhs->M != rhs->M)
		return false;//Not the same shape
	/*
	TODO: Remove epsilon parameter, or make it optional, automatically
	choose machine epsilon.
	*/
	
	for(int i = 0;i<lhs->N;i++){
		for(int j = 0;j<lhs->M;j++){
			if( fabs(MATPTR_ELEMENT(lhs,i,j)) - fabs(MATPTR_ELEMENT(rhs,i,j)) > epsilon ) { return false; }
		}
	}
	return true;//Success
}
