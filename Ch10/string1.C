#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <vector>

#include "benchmark/benchmark.h"

class simple_string {
    public:
    simple_string() : s_() {}
    explicit simple_string(const char* s) : s_(strdup(s)) {}
    simple_string(const simple_string& s) : s_(strdup(s.s_)) {}
    simple_string& operator=(const char* s) { free(s_); s_ = strdup(s); return *this; }
    simple_string& operator=(const simple_string& s) { free(s_); s_ = strdup(s.s_); return *this; }
    bool operator==(const simple_string& rhs) const { return strcmp(s_, rhs.s_) == 0; }
    ~simple_string() { free(s_); }
    private:
    char* s_;
};

class small_string {
    public:
    small_string() : s_() {}
    explicit small_string(const char* s) : s_(strlen(s) + 1 < sizeof(buf_) ? buf_ : strdup(s)) { if (s_ == buf_) strncpy(buf_, s, sizeof(buf_)); }
    small_string(const small_string& s) : s_(s.s_ == s.buf_ ? buf_ : strdup(s.s_)) { if (s_ == buf_) memcpy(buf_, s.buf_, sizeof(buf_)); }
    small_string& operator=(const char* s) { if (s_ != buf_) free(s_); s_ = strlen(s) + 1 < sizeof(buf_) ? buf_ : strdup(s); if (s_ == buf_) strncpy(buf_, s, sizeof(buf_)); return *this; }
    small_string& operator=(const small_string& s) { if (s_ != buf_) free(s_); s_ = s.s_ == s.buf_ ? buf_ : strdup(s.s_); if (s_ == buf_) memcpy(buf_, s.buf_, sizeof(buf_)); return *this; }
    bool operator==(const small_string& rhs) const { return strcmp(s_, rhs.s_) == 0; }
    ~small_string() { if (s_ != buf_) free(s_); }
    private:
    char* s_;
    char buf_[16];
};

#define REPEAT2(x) x x
#define REPEAT4(x) REPEAT2(x) REPEAT2(x)
#define REPEAT8(x) REPEAT4(x) REPEAT4(x)
#define REPEAT16(x) REPEAT8(x) REPEAT8(x)
#define REPEAT32(x) REPEAT16(x) REPEAT16(x)
#define REPEAT(x) REPEAT32(x)

template <typename T>
void BM_string_create_short(benchmark::State& state) {
    const char* s = "Simple string";
    for (auto _ : state) {
        REPEAT({ T S(s); benchmark::DoNotOptimize(S); })
    }
    state.SetItemsProcessed(32*state.iterations());
}

template <typename T>
void BM_string_copy_short(benchmark::State& state) {
    const T s("Simple string");
    for (auto _ : state) {
        REPEAT({ T S(s); benchmark::DoNotOptimize(S); })
    }
    state.SetItemsProcessed(32*state.iterations());
}

template <typename T>
void BM_string_assign_short(benchmark::State& state) {
    const T s("Simple string");
    T S;
    for (auto _ : state) {
        REPEAT({ benchmark::DoNotOptimize(S = s); })
    }
    state.SetItemsProcessed(32*state.iterations());
}

template <typename T>
void BM_string_compare_short(benchmark::State& state) {
    const size_t N = state.range(0);
    const T s("Simple string");
    std::vector<T> v1, v2;
    v1.reserve(N);
    v2.reserve(N);
    {
        std::vector<void*> v0(2*N);
        for (size_t i = 0; i < N; ++i) {
            v1.push_back(s);
            v0[i] = malloc(rand() % 32);
            v2.push_back(s);
            v0[N + i] = malloc(rand() % 32);
        }
        for (size_t i = 0; i < N; ++i) {
            free(v0[i]);
            free(v0[N + i]);
        }
    }
    for (auto _ : state) {
        for (size_t i = 0; i < N; ++i) benchmark::DoNotOptimize(v1[i] == v2[i]);
    }
    state.SetItemsProcessed(N*state.iterations());
}

template <typename T>
void BM_string_create_long(benchmark::State& state) {
    const char* s = "Slightly longer string";
    for (auto _ : state) {
        REPEAT({ T S(s); benchmark::DoNotOptimize(S); })
    }
    state.SetItemsProcessed(32*state.iterations());
}

BENCHMARK_TEMPLATE1(BM_string_create_short, simple_string);
BENCHMARK_TEMPLATE1(BM_string_create_short, small_string);
BENCHMARK_TEMPLATE1(BM_string_copy_short, simple_string);
BENCHMARK_TEMPLATE1(BM_string_copy_short, small_string);
BENCHMARK_TEMPLATE1(BM_string_assign_short, simple_string);
BENCHMARK_TEMPLATE1(BM_string_assign_short, small_string);
#define ARG Arg(1<<22)
BENCHMARK_TEMPLATE1(BM_string_compare_short, simple_string)->ARG;
BENCHMARK_TEMPLATE1(BM_string_compare_short, small_string)->ARG;
#undef ARG
BENCHMARK_TEMPLATE1(BM_string_create_long, simple_string);
BENCHMARK_TEMPLATE1(BM_string_create_long, small_string);

static const long max_threads = sysconf(_SC_NPROCESSORS_CONF);
BENCHMARK_TEMPLATE1(BM_string_create_short, simple_string)->ThreadRange(1, max_threads);
BENCHMARK_TEMPLATE1(BM_string_create_short, small_string)->ThreadRange(1, max_threads);

BENCHMARK_MAIN();

