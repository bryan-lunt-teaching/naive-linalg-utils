
#include <thread>

#include <iostream>
#include <random>

#include <chrono>
#include <ctime>


#include <benchmark/benchmark.h>

#include "naivelinalg.h"

//The cost of multiplying a NxK matrix with a KxM matrix
#define MATRIX_MATRIX_FLOPS(N,M,K) (2*int64_t(N)*int64_t(M)*int64_t(K))


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
static void Matrix_Alignment_0(benchmark::State& state) {
  int N = state.range(0);
  int n_warmup = 3;
  int num_threads = 1; //state.range(1); //serial code, always 1

  /*     SETUP      */
  //Matrix creation
  matrix A,B,C;
  matrix_create(&A,N,N);
  matrix_create(&B,N,N);
  matrix_create(&C,N,N);
  //Populate matrices with random data.
  //No need to populate the target.
  //TODO: Populate B
  //TODO: Populate C
  
  //TODO: Check if we need to use our own timers or if the built in ones suffice.
  for (auto _ : state) {
			matrix_mult_naive(&A,&B,&C);
  }
  state.SetItemsProcessed(state.iterations()*MATRIX_MATRIX_FLOPS(N,N,N));

  //TEARDOWN
  matrix_destroy(&A);
  matrix_destroy(&B);
  matrix_destroy(&C);
  
  
  state.counters["N"] = N;
  state.counters["num_threads"] = num_threads;//Always 1
}
BENCHMARK(Matrix_Alignment_0)->MeasureProcessCPUTime()
->Iterations(3)
->ArgNames({"N", "num_threads"})
->Args({1023,1})
->Args({1024,1})
->Args({1025,1})
->Args({1026,1})
->Args({1027,1});

static void Matrix_Alignment_A(benchmark::State& state) {
  int N = state.range(0);
  int n_warmup = 3;
  int num_threads = 1; //state.range(1); //serial code, always 1

  /*     SETUP      */
  //Matrix creation
  matrix A,B,C;
  matrix_create(&A,N,N);
  matrix_create(&B,N,N);
  matrix_create(&C,N,N);
  //Populate matrices with random data.
  //No need to populate the target.
  //TODO: Populate B
  //TODO: Populate C
  
  //TODO: Check if we need to use our own timers or if the built in ones suffice.
  for (auto _ : state) {
			matrix_mult_naive_A(&A,&B,&C);
  }
  state.SetItemsProcessed(state.iterations()*MATRIX_MATRIX_FLOPS(N,N,N));

  //TEARDOWN
  matrix_destroy(&A);
  matrix_destroy(&B);
  matrix_destroy(&C);
  
  
  state.counters["N"] = N;
  state.counters["num_threads"] = num_threads;//Always 1
}
BENCHMARK(Matrix_Alignment_A)->MeasureProcessCPUTime()
->Iterations(3)
->ArgNames({"N", "num_threads"})
->Args({1023,1})
->Args({1024,1})
->Args({1025,1})
->Args({1026,1})
->Args({1027,1});


static void Matrix_Alignment_B(benchmark::State& state) {
  int N = state.range(0);
  int n_warmup = 3;
  int num_threads = 1; //state.range(1); //serial code, always 1

  /*     SETUP      */
  //Matrix creation
  matrix A,B,C;
  matrix_create(&A,N,N);
  matrix_create(&B,N,N);
  matrix_create(&C,N,N);
  //Populate matrices with random data.
  //No need to populate the target.
  //TODO: Populate B
  //TODO: Populate C
  
  //TODO: Check if we need to use our own timers or if the built in ones suffice.
  for (auto _ : state) {
			matrix_mult_naive_B(&A,&B,&C);
  }
  state.SetItemsProcessed(state.iterations()*MATRIX_MATRIX_FLOPS(N,N,N));

  //TEARDOWN
  matrix_destroy(&A);
  matrix_destroy(&B);
  matrix_destroy(&C);
  
  
  state.counters["N"] = N;
  state.counters["num_threads"] = num_threads;//Always 1
}
BENCHMARK(Matrix_Alignment_B)->MeasureProcessCPUTime()
->Iterations(3)
->ArgNames({"N", "num_threads"})
->Args({1023,1})
->Args({1024,1})
->Args({1025,1})
->Args({1026,1})
->Args({1027,1});

static void Matrix_Alignment_C(benchmark::State& state) {
  int N = state.range(0);
  int n_warmup = 3;
  int num_threads = 1; //state.range(1); //serial code, always 1

  /*     SETUP      */
  //Matrix creation
  matrix A,B,C;
  matrix_create(&A,N,N);
  matrix_create(&B,N,N);
  matrix_create(&C,N,N);
  //Populate matrices with random data.
  //No need to populate the target.
  //TODO: Populate B
  //TODO: Populate C
  
  //TODO: Check if we need to use our own timers or if the built in ones suffice.
  for (auto _ : state) {
			matrix_mult_naive_C(&A,&B,&C);
  }
  state.SetItemsProcessed(state.iterations()*MATRIX_MATRIX_FLOPS(N,N,N));

  //TEARDOWN
  matrix_destroy(&A);
  matrix_destroy(&B);
  matrix_destroy(&C);
  
  
  state.counters["N"] = N;
  state.counters["num_threads"] = num_threads;//Always 1
}
BENCHMARK(Matrix_Alignment_C)->MeasureProcessCPUTime()
->Iterations(3)
->ArgNames({"N", "num_threads"})
->Args({1023,1})
->Args({1024,1})
->Args({1025,1})
->Args({1026,1})
->Args({1027,1});

static void Matrix_Alignment_D(benchmark::State& state) {
  int N = state.range(0);
  int n_warmup = 3;
  int num_threads = 1; //state.range(1); //serial code, always 1

  /*     SETUP      */
  //Matrix creation
  matrix A,B,C;
  matrix_create(&A,N,N);
  matrix_create(&B,N,N);
  matrix_create(&C,N,N);
  //Populate matrices with random data.
  //No need to populate the target.
  //TODO: Populate B
  //TODO: Populate C
  
  //TODO: Check if we need to use our own timers or if the built in ones suffice.
  for (auto _ : state) {
			matrix_mult_naive_D(&A,&B,&C);
  }
  state.SetItemsProcessed(state.iterations()*MATRIX_MATRIX_FLOPS(N,N,N));

  //TEARDOWN
  matrix_destroy(&A);
  matrix_destroy(&B);
  matrix_destroy(&C);
  
  
  state.counters["N"] = N;
  state.counters["num_threads"] = num_threads;//Always 1
}
BENCHMARK(Matrix_Alignment_D)->MeasureProcessCPUTime()
->Iterations(3)
->ArgNames({"N", "num_threads"})
->Args({1023,1})
->Args({1024,1})
->Args({1025,1})
->Args({1026,1})
->Args({1027,1});
