#include <stdlib.h>

#include <vector>

#include "benchmark/benchmark.h"

void BM_malloc_free(benchmark::State& state) {
    const size_t S = state.range(0);
    for (auto _ : state) {
        void* p = malloc(S);
        benchmark::DoNotOptimize(p);
        free(p);
    }
    state.SetItemsProcessed(state.iterations());
}

//BENCHMARK(BM_malloc_free)->Arg(64);
BENCHMARK(BM_malloc_free)->RangeMultiplier(2)->Range(32, 256);

BENCHMARK_MAIN();

