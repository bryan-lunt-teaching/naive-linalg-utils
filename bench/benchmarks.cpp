#include <cstdio>

#include <thread>

#include <iostream>
#include <random>

#include <chrono>
#include <ctime>


#include <benchmark/benchmark.h>

#include "naivelinalg.h"

//The cost of multiplying a NxK matrix with a KxM matrix
#define MATRIX_MATRIX_FLOPS(N,M,K) (2*int64_t(N)*int64_t(M)*int64_t(K))

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
  
  /*Are our matrices actually aligned to cache line boundaries?
  //Yes, we're alligned to the correct boundaries.
  printf("sizeof(char) %p \n",sizeof(char));
  printf("Address of A.data %p \n",A.data);
  printf("Address of B.data %p \n",B.data);
  printf("Address of C.data %p \n",C.data);
  */
  
  //TODO: Better prefetching and cache warming or cache trashing.
  
  //This will at least pull the pages into memory if they haven't already been.
  A.data[0] = B.data[0]*C.data[0];//Not the correct operation, but A gets overwritten anyway
  
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
  state.counters["mem"] = 3*int64_t(N)*int64_t(N)*sizeof(matrix_data_t);
  state.counters["r"] = N%8;
}
BENCHMARK(Matrix_Alignment_0)->MeasureProcessCPUTime()
->Iterations(3)
->ArgNames({"N"})
->DenseRange(15,127,16)
->DenseRange(16,128,16)
->DenseRange(17,129,16)


->DenseRange(256-2,256+2,1)

->DenseRange(512-2,512+2,1)

->DenseRange(528-32,528+32,1)

->DenseRange(1008-32,1008+32,1)

->DenseRange(1024-2,1024+2,1)

;
