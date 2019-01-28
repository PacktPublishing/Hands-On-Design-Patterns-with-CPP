#include <stdlib.h>

#include <memory>

#include "benchmark/benchmark.h"

#define REPEAT2(x) x x
#define REPEAT4(x) REPEAT2(x) REPEAT2(x)
#define REPEAT8(x) REPEAT4(x) REPEAT4(x)
#define REPEAT16(x) REPEAT8(x) REPEAT8(x)
#define REPEAT32(x) REPEAT16(x) REPEAT16(x)
#define REPEAT(x) REPEAT32(x)

template <typename T>
class scoped_ptr {
    public:
    explicit scoped_ptr(T* p) : p_(p) {}
    ~scoped_ptr() { delete p_; }
    T* operator->() { return p_; }
    const T* operator->() const { return p_; }
    T& operator*() { return *p_; }
    const T& operator*() const { return *p_; }
    private:
    T* p_;
};

void BM_rawptr_construct(benchmark::State& state) {
    for (auto _ : state) {
        int* p = new int;
        delete p;
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_rawptr_dereference(benchmark::State& state) {
    int* p = new int;
    for (auto _ : state) {
        REPEAT(benchmark::DoNotOptimize(*p);)
    }
    delete p;
    state.SetItemsProcessed(32*state.iterations());
}

void BM_scoped_ptr_construct(benchmark::State& state) {
    for (auto _ : state) {
        scoped_ptr<int> p(new int);
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_scoped_ptr_dereference(benchmark::State& state) {
    scoped_ptr<int> p(new int);
    for (auto _ : state) {
        REPEAT(benchmark::DoNotOptimize(*p);)
    }
    state.SetItemsProcessed(32*state.iterations());
}

void BM_unique_ptr_construct(benchmark::State& state) {
    for (auto _ : state) {
        std::unique_ptr<int> p(new int);
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_unique_ptr_dereference(benchmark::State& state) {
    std::unique_ptr<int> p(new int);
    for (auto _ : state) {
        REPEAT(benchmark::DoNotOptimize(*p);)
    }
    state.SetItemsProcessed(32*state.iterations());
}

BENCHMARK(BM_rawptr_construct);
BENCHMARK(BM_scoped_ptr_construct);
BENCHMARK(BM_unique_ptr_construct);
BENCHMARK(BM_rawptr_dereference);
BENCHMARK(BM_scoped_ptr_dereference);
BENCHMARK(BM_unique_ptr_dereference);

BENCHMARK_MAIN();

