#include "hand_optimized.h"

bool matrix_mult_naive_A(matrix* const target, matrix* const A, matrix* const B){
	if(NULL == target->data || NULL == A->data || NULL == B->data) return false;
	if(target->data == A->data || target->data == B->data) return false;

	if(A->M != B->N) return false;
	if(target->N != A->N || target->M != B->M) return false;
	
	register matrix_data_t tmpval;
	for(int i = 0;i<A->N;i++)
		for(int j = 0;j<B->M;j++){
			tmpval = 0.0;
			for(int k = 0;k<A->M;k++)
				tmpval += MATPTR_ELEMENT(A,i,k)*MATPTR_ELEMENT(B,k,j);
			MATPTR_ELEMENT(target,i,j) = tmpval;
		}

	return true;
}

bool matrix_mult_naive_B(matrix *target, matrix *A, matrix *B){
	if(NULL == target->data || NULL == A->data || NULL == B->data) return false;
	if(target->data == A->data || target->data == B->data) return false;

	if(A->M != B->N) return false;
	if(target->N != A->N || target->M != B->M) return false;
    
    matrix matT = *target;
    matrix matA = *A;
    matrix matB = *B;

	for(int i = 0;i<matA.N;i++)
		for(int j = 0;j<matB.M;j++){
			register matrix_data_t tmpval = 0.0;
			for(int k = 0;k<matA.M;k++)
				tmpval += MATRIX_ELEMENT(matA,i,k)*MATRIX_ELEMENT(matB,k,j);
			MATRIX_ELEMENT(matT,i,j) = tmpval;
		}

	return true;
}

bool matrix_mult_naive_C(matrix *target, matrix *A, matrix *B){
	if(NULL == target->data || NULL == A->data || NULL == B->data) return false;
	if(target->data == A->data || target->data == B->data) return false;

	if(A->M != B->N) return false;
	if(target->N != A->N || target->M != B->M) return false;
    
    const matrix matT = *target;
    const matrix matA = *A;
    const matrix matB = *B;
    
    const int N = matA.N;
    const int M = matB.M;
    const int K = matA.M;

	for( int i = 0;i<N;i++)
		for(int j = 0;j<M;j++){
			register matrix_data_t tmpval = 0.0;
			for(int k = 0;k<K;k++)
				tmpval += MATRIX_ELEMENT(matA,i,k)*MATRIX_ELEMENT(matB,k,j);
			MATRIX_ELEMENT(matT,i,j) = tmpval;
		}

	return true;
}

bool matrix_mult_naive_D(matrix *target, matrix *A, matrix *B){
	if(NULL == target->data || NULL == A->data || NULL == B->data) return false;
	if(target->data == A->data || target->data == B->data) return false;

	if(A->M != B->N) return false;
	if(target->N != A->N || target->M != B->M) return false;
    
    const matrix matT = *target;
    const matrix matA = *A;
    const matrix matB = *B;
    
    matrix_data_t* const dataT = matT.data;
    matrix_data_t* const dataA = matA.data;
    matrix_data_t* const dataB = matB.data;
    
    const int N = matA.N;
    const int M = matB.M;
    const int K = matA.M;

	for( int i = 0;i<N;i++)
		for(int j = 0;j<M;j++){
			register matrix_data_t tmpval = 0.0;
			for(int k = 0;k<K;k++)
				tmpval += ARRAY_ELEMENT(dataA,N,K,i,k)*ARRAY_ELEMENT(dataB,K,M,k,j);
			ARRAY_ELEMENT(dataT,N,M,i,j) = tmpval;
		}

	return true;
}
