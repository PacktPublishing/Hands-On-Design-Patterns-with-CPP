#include <functional>

#include "benchmark/benchmark.h"

#define REPEAT2(x) x x
#define REPEAT4(x) REPEAT2(x) REPEAT2(x)
#define REPEAT8(x) REPEAT4(x) REPEAT4(x)
#define REPEAT16(x) REPEAT8(x) REPEAT8(x)
#define REPEAT32(x) REPEAT16(x) REPEAT16(x)
#define REPEAT(x) REPEAT32(x)

namespace ScopeGuardTypeErased {
class ScopeGuard {
    public:
    template <typename Func> ScopeGuard(Func&& func) : commit_(false), func_(func) {}
    template <typename Func> ScopeGuard(const Func& func) : commit_(false), func_(func) {}
    ~ScopeGuard() { if (!commit_) func_(); }
    void commit() const noexcept { commit_ = true; }
    ScopeGuard(ScopeGuard&& other) : commit_(other.commit_), func_(other.func_) { other.commit(); }
    private:
    mutable bool commit_;
    std::function<void()> func_;
    ScopeGuard& operator=(const ScopeGuard&) = delete;
};
}

namespace ScopeGuardTemplate {
class ScopeGuardBase {
    public:
    ScopeGuardBase() : commit_(false) {}
    void commit() const noexcept { commit_ = true; }

    protected:
    ScopeGuardBase(ScopeGuardBase&& other) : commit_(other.commit_) { other.commit(); }
    ~ScopeGuardBase() {}
    mutable bool commit_;

    private:
    ScopeGuardBase& operator=(const ScopeGuardBase&) = delete;
};

template <typename Func>
class ScopeGuard : public ScopeGuardBase {
    public:
    ScopeGuard(Func&& func) : func_(func) {}
    ScopeGuard(const Func& func) : func_(func) {}
    ~ScopeGuard() { if (!commit_) func_(); }
    ScopeGuard(ScopeGuard&& other) : ScopeGuardBase(std::move(other)), func_(other.func_) {}
    private:
    Func func_;
};

template <typename Func>
ScopeGuard<Func> MakeGuard(Func&& func) {
    return ScopeGuard<Func>(std::forward<Func>(func));
}
}

inline void noop() {}

void BM_type_erased_noop(benchmark::State& state) {
    for (auto _ : state) {
        REPEAT({ScopeGuardTypeErased::ScopeGuard SG([&] { noop(); });})
    }
    state.SetItemsProcessed(32*state.iterations());
}

void BM_template_noop(benchmark::State& state) {
    for (auto _ : state) {
        REPEAT({auto SG = ScopeGuardTemplate::MakeGuard([&] { noop(); });})
    }
    state.SetItemsProcessed(32*state.iterations());
}

void BM_free(benchmark::State& state) {
    void* p = NULL;
    for (auto _ : state) {
        benchmark::DoNotOptimize(p = malloc(8));
        free(p);
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_type_erased_free(benchmark::State& state) {
    void* p = NULL;
    for (auto _ : state) {
        benchmark::DoNotOptimize(p = malloc(8));
        ScopeGuardTypeErased::ScopeGuard SG([&] { free(p); });
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_template_free(benchmark::State& state) {
    void* p = NULL;
    for (auto _ : state) {
        benchmark::DoNotOptimize(p = malloc(8));
        auto SG = ScopeGuardTemplate::MakeGuard([&] { free(p); });
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_count(benchmark::State& state) {
    volatile int i = 0;
    for (auto _ : state) {
        REPEAT({
        benchmark::DoNotOptimize(++i);
        --i;
        });
    }
    state.SetItemsProcessed(32*state.iterations());
    if (i) abort();
}

void BM_type_erased_count(benchmark::State& state) {
    volatile int i = 0;
    for (auto _ : state) {
        REPEAT({
        benchmark::DoNotOptimize(++i);
        ScopeGuardTypeErased::ScopeGuard SG([&] { --i; });
        });
    }
    state.SetItemsProcessed(32*state.iterations());
    if (i) abort();
}

void BM_template_count(benchmark::State& state) {
    volatile int i = 0;
    for (auto _ : state) {
        REPEAT({
        benchmark::DoNotOptimize(++i);
        auto SG = ScopeGuardTemplate::MakeGuard([&] { --i; });
        });
    }
    state.SetItemsProcessed(32*state.iterations());
    if (i) abort();
}

BENCHMARK(BM_type_erased_noop);
BENCHMARK(BM_template_noop);
BENCHMARK(BM_free);
BENCHMARK(BM_type_erased_free);
BENCHMARK(BM_template_free);
BENCHMARK(BM_count);
BENCHMARK(BM_type_erased_count);
BENCHMARK(BM_template_count);

BENCHMARK_MAIN();
