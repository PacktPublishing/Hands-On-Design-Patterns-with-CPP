#include <stdlib.h>

#include "benchmark/benchmark.h"

#define REPEAT2(x) x x
#define REPEAT4(x) REPEAT2(x) REPEAT2(x)
#define REPEAT8(x) REPEAT4(x) REPEAT4(x)
#define REPEAT16(x) REPEAT8(x) REPEAT8(x)
#define REPEAT32(x) REPEAT16(x) REPEAT16(x)
#define REPEAT(x) REPEAT32(x)

#include <iostream>

class Singleton {
    public:
    Singleton() {}
    int& get() { return value_; }

    private:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    private:
    static int value_;
};
int Singleton::value_ = 0;

void BM_singleton(benchmark::State& state) {
    Singleton S;
    for (auto _ : state) {
        REPEAT(benchmark::DoNotOptimize(++S.get());)
    }
    state.SetItemsProcessed(32*state.iterations());
}

void BM_singletons(benchmark::State& state) {
    for (auto _ : state) {
        REPEAT(benchmark::DoNotOptimize(++Singleton().get());)
    }
    state.SetItemsProcessed(32*state.iterations());
}

BENCHMARK(BM_singleton)->ThreadRange(1, 64);
BENCHMARK(BM_singletons)->ThreadRange(1, 64);

BENCHMARK_MAIN();
