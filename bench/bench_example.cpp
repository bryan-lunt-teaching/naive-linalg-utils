#include <random>
#include <chrono>
#include <ctime>
#include <thread>

#include <benchmark/benchmark.h>

#include <cstring>
#include <string>
#include <sstream>

#define BENCHMARK_NUM_EPOCHS 1
#define BENCHMARK_TILE_SIZE 2
#define BENCHMARK_NUM_THREADS 2

static void BM_memcpy(benchmark::State& state) {
  char* src = new char[state.range(0)];
  char* dst = new char[state.range(0)];
  memset(src, 'x', state.range(0));
  for (auto _ : state)
    memcpy(dst, src, state.range(0));
  state.SetBytesProcessed(int64_t(state.iterations()) *
                          int64_t(state.range(0)));
  delete[] src;
  delete[] dst;
}
BENCHMARK(BM_memcpy)->Arg(8)->Arg(64)->Arg(512)->Arg(1<<10)->Arg(8<<10);


static void BM_GS_Parallel(benchmark::State& state){
	int N_iters = state.range(0);
	int tile_size = state.range(1);
	int num_threads = state.range(2);

	std::stringstream ss1;

	for( auto _ : state ){
		//any object instantiation
		auto start = std::chrono::high_resolution_clock::now();

		//code to benchmark


		auto end   = std::chrono::high_resolution_clock::now();

		auto elapsed_seconds = std::chrono::duration_cast<
				std::chrono::duration<double> >(end - start);

		state.SetIterationTime(elapsed_seconds.count());
	}

	state.counters["epochs"] = N_iters;
	state.counters["tile_size"] = tile_size;
	state.counters["num_threads"] = num_threads;
}
BENCHMARK(BM_GS_Parallel)->UseManualTime()->ArgNames({"epochs", "tile_size", "num_threads"})
			->RangeMultiplier(2)
			->Ranges( { {BENCHMARK_NUM_EPOCHS, BENCHMARK_NUM_EPOCHS},
							{8, BENCHMARK_TILE_SIZE},
							{1, BENCHMARK_NUM_THREADS} } );
