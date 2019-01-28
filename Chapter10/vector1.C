#include <stdlib.h>

#include "benchmark/benchmark.h"

class simple_vector {
    public:
    simple_vector() : n_(), p_() {}
    simple_vector(std::initializer_list<int> il) : n_(il.size()), p_(static_cast<int*>(malloc(sizeof(int)*n_))) {
        int* p = p_;
        for (auto x : il) *p++ = x;
    }
    ~simple_vector() {
        free(p_);
    }
    size_t size() const { return n_; }
    private:
    size_t n_;
    int* p_;
};

class small_vector {
    public:
    small_vector() : n_(), p_() {}
    small_vector(std::initializer_list<int> il) : n_(il.size()), p_(n_ < sizeof(buf_)/sizeof(buf_[0]) ? buf_ : static_cast<int*>(malloc(sizeof(int)*n_))) {
        int* p = p_;
        for (auto x : il) *p++ = x;
    }
    ~small_vector() {
        if (p_ != buf_) free(p_);
    }
    private:
    size_t n_;
    int* p_;
    int buf_[16];
};

#define REPEAT2(x) x x
#define REPEAT4(x) REPEAT2(x) REPEAT2(x)
#define REPEAT8(x) REPEAT4(x) REPEAT4(x)
#define REPEAT16(x) REPEAT8(x) REPEAT8(x)
#define REPEAT32(x) REPEAT16(x) REPEAT16(x)
#define REPEAT(x) REPEAT32(x)

template <typename T>
void BM_vector_create_short(benchmark::State& state) {
    std::initializer_list<int> il { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };
    for (auto _ : state) {
        REPEAT({ T v(il); benchmark::DoNotOptimize(v); })
    }
    state.SetItemsProcessed(32*state.iterations());
}

template <typename T>
void BM_vector_create_long(benchmark::State& state) {
    std::initializer_list<int> il { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199 };
    for (auto _ : state) {
        REPEAT({ T v(il); benchmark::DoNotOptimize(v); })
    }
    state.SetItemsProcessed(32*state.iterations());
}

BENCHMARK_TEMPLATE1(BM_vector_create_short, simple_vector);
BENCHMARK_TEMPLATE1(BM_vector_create_short, small_vector);
BENCHMARK_TEMPLATE1(BM_vector_create_long, simple_vector);
BENCHMARK_TEMPLATE1(BM_vector_create_long, small_vector);

BENCHMARK_MAIN();

