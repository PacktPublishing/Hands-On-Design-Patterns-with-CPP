#include <stdlib.h>
#include <limits.h>


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
    small_vector() { short_.n = 0; }
    small_vector(std::initializer_list<int> il) {
        int* p;
        if (il.size() < sizeof(short_.buf)/sizeof(short_.buf[0])) {
            short_.n = il.size();
            p = short_.buf;
        } else {
            short_.n = UCHAR_MAX;
            long_.n = il.size();
            p = long_.p = static_cast<int*>(malloc(sizeof(int)*long_.n));
        }
        for (auto x : il) *p++ = x;
    }
    ~small_vector() {
        if (short_.n == UCHAR_MAX) free(long_.p);
    }
    private:
    union {
        struct {
            size_t n;
            int* p;
        } long_;
        struct {
            int buf[15];
            unsigned char n;
        } short_;
    };
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

