#include <cstdio>

#include <thread>

#include <iostream>
#include <random>

#include <chrono>
#include <ctime>


#include <benchmark/benchmark.h>

#include "naivelinalg.h"

#include "hand_optimized.h"

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
->Args({510,1})
->Args({511,1})
->Args({512,1})
->Args({513,1})
->Args({514,1})

->Args({1006,1})
->Args({1007,1})
->Args({1008,1})
->Args({1009,1})
->Args({1010,1})

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
