#include <stdlib.h>

#include <vector>

#include "benchmark/benchmark.h"

#define REPEAT2(x) x x
#define REPEAT4(x) REPEAT2(x) REPEAT2(x)
#define REPEAT8(x) REPEAT4(x) REPEAT4(x)
#define REPEAT16(x) REPEAT8(x) REPEAT8(x)
#define REPEAT32(x) REPEAT16(x) REPEAT16(x)
#define REPEAT(x) REPEAT32(x)

class Positional {
    public:
    Positional(bool a = false, bool b = false, bool c = false, bool d = false,
               bool e = false, bool f = false, bool g = false, bool h = false);

    operator bool() const { return a_; }

    private:
    const bool a_;
    const bool b_;
    const bool c_;
    const bool d_;
    const bool e_;
    const bool f_;
    const bool g_;
    const bool h_;
};

class Named {
    public:
    class Options {
        public:
        Options() : a_(false), b_(false), c_(false), d_(false),
                    e_(false), f_(false), g_(false), h_(false) {}
        Options& SetA(bool a) { a_ = a; return *this; }
        Options& SetB(bool b) { b_ = b; return *this; }
        Options& SetC(bool c) { c_ = c; return *this; }
        Options& SetD(bool d) { d_ = d; return *this; }
        Options& SetE(bool e) { e_ = e; return *this; }
        Options& SetF(bool f) { f_ = f; return *this; }
        Options& SetG(bool g) { g_ = g; return *this; }
        Options& SetH(bool h) { h_ = h; return *this; }

        private:
        friend class Named;
        bool a_;
        bool b_;
        bool c_;
        bool d_;
        bool e_;
        bool f_;
        bool g_;
        bool h_;
    };

    Named(Options options);

    operator bool() const { return options_.a_; }

    private:
    const Options options_;
};

class Aggregate {
    public:
    struct Options {
        Options() : a(false), b(false), c(false), d(false),
                    e(false), f(false), g(false), h(false) {}
        bool a;
        bool b;
        bool c;
        bool d;
        bool e;
        bool f;
        bool g;
        bool h;
    };

    Aggregate(const Options& options);

    operator bool() const { return options_.a; }

    private:
    const Options options_;
};

void BM_positional_const(benchmark::State& state) {
    for (auto _ : state) {
        REPEAT(benchmark::DoNotOptimize(bool(Positional(false, false, false, false, false, false, false, true)));)
    }
    state.SetItemsProcessed(32*state.iterations());
}

void BM_named_const(benchmark::State& state) {
    for (auto _ : state) {
        REPEAT(benchmark::DoNotOptimize(bool(Named(Named::Options().SetH(true))));)
    }
    state.SetItemsProcessed(32*state.iterations());
}

void BM_aggregate_const(benchmark::State& state) {
    for (auto _ : state) {
        REPEAT({
                Aggregate::Options options;
                options.h = true;
                benchmark::DoNotOptimize(bool(Aggregate(options)));
                })
    }
    state.SetItemsProcessed(32*state.iterations());
}

void BM_positional_var(benchmark::State& state) {
    const size_t N = 1000;
    std::vector<int> v(N); 
    for (size_t i = 0; i < N; ++i) v[i] = rand();
    size_t i = 0;
    for (auto _ : state) {
        REPEAT({
                benchmark::DoNotOptimize(bool(Positional(false, false, false, false, false, false, false, v[i++])));
                if (i == N) i = 0;
                })
    }
    state.SetItemsProcessed(32*state.iterations());
}

void BM_named_var(benchmark::State& state) {
    const size_t N = 1000;
    std::vector<int> v(N); 
    for (size_t i = 0; i < N; ++i) v[i] = rand();
    size_t i = 0;
    for (auto _ : state) {
        REPEAT({
                benchmark::DoNotOptimize(bool(Named(Named::Options().SetH(v[i++]))));
                if (i == N) i = 0;
                })
    }
    state.SetItemsProcessed(32*state.iterations());
}

void BM_aggregate_var(benchmark::State& state) {
    const size_t N = 1000;
    std::vector<int> v(N); 
    for (size_t i = 0; i < N; ++i) v[i] = rand();
    size_t i = 0;
    for (auto _ : state) {
        REPEAT({
                Aggregate::Options options;
                options.h = v[i++];
                benchmark::DoNotOptimize(bool(Aggregate(options)));
                if (i == N) i = 0;
                })
    }
    state.SetItemsProcessed(32*state.iterations());
}

BENCHMARK(BM_positional_const);
BENCHMARK(BM_named_const);
BENCHMARK(BM_aggregate_const);
BENCHMARK(BM_positional_var);
BENCHMARK(BM_named_var);
BENCHMARK(BM_aggregate_var);

BENCHMARK_MAIN();
