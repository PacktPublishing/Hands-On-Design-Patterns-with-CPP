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
    Positional(bool a, bool b, bool c, bool d);

    operator bool() const { return a_; }

    private:
    const bool a_;
    const bool b_;
    const bool c_;
    const bool d_;
};

class Named {
    public:
    class Options {
        public:
        Options() : a_(false), b_(false), c_(false), d_(false) {}
        Options& SetA(bool a) { a_ = a; return *this; }
        Options& SetB(bool b) { b_ = b; return *this; }
        Options& SetC(bool c) { c_ = c; return *this; }
        Options& SetD(bool d) { d_ = d; return *this; }

        private:
        friend class Named;
        bool a_;
        bool b_;
        bool c_;
        bool d_;
    };

    Named(Options options);

    operator bool() const { return options_.a_; }

    private:
    const Options options_;
};

class Aggregate {
    public:
    struct Options {
        Options() : a(false), b(false), c(false), d(false) {}
        bool a;
        bool b;
        bool c;
        bool d;
    };

    Aggregate(const Options& options);

    operator bool() const { return options_.a; }

    private:
    const Options options_;
};

void BM_positional_const(benchmark::State& state) {
    for (auto _ : state) {
        REPEAT(benchmark::DoNotOptimize(bool(Positional(true, false, true, false)));)
    }
    state.SetItemsProcessed(32*state.iterations());
}

void BM_named_const(benchmark::State& state) {
    for (auto _ : state) {
        REPEAT(benchmark::DoNotOptimize(bool(Named(Named::Options().SetA(true).SetC(true))));)
    }
    state.SetItemsProcessed(32*state.iterations());
}

void BM_aggregate_const(benchmark::State& state) {
    for (auto _ : state) {
        REPEAT({
                Aggregate::Options options;
                options.a = true;
                options.c = true;
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
                const bool a = v[i++];
                const bool b = v[i++];
                const bool c = v[i++];
                const bool d = v[i++];
                benchmark::DoNotOptimize(bool(Positional(a, b, c, d)));
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
                const bool a = v[i++];
                const bool b = v[i++];
                const bool c = v[i++];
                const bool d = v[i++];
                benchmark::DoNotOptimize(bool(Named(Named::Options().SetA(a).SetB(b).SetC(c).SetD(d))));
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
                options.a = v[i++];
                options.b = v[i++];
                options.c = v[i++];
                options.d = v[i++];
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
