#include <stdlib.h>

#include <new>

#include "benchmark/benchmark.h"

template <typename T, typename Deleter>
class smartptr {
    public:
    smartptr(T* p, Deleter d) : p_(p), d_(d) {}
    ~smartptr() { d_(p_); }
    T* operator->() { return p_; }
    const T* operator->() const { return p_; }
    private:
    T* p_;
    Deleter d_;
};

template <typename T>
class smartptr_te {
    struct deleter_base {
        virtual void apply(void*) = 0;
        virtual ~deleter_base() {}
    };
    template <typename Deleter> struct deleter : public deleter_base {
        deleter(Deleter d) : d_(d) {}
        virtual void apply(void* p) { d_(static_cast<T*>(p)); }
        Deleter d_;
    };
    public:
    template <typename Deleter> smartptr_te(T* p, Deleter d) : p_(p), d_(new deleter<Deleter>(d)) {}
    ~smartptr_te() { d_->apply(p_); delete d_; }
    T* operator->() { return p_; }
    const T* operator->() const { return p_; }
    private:
    T* p_;
    deleter_base* d_;
};

template <typename T>
class smartptr_te_lb {
    struct deleter_base {
        virtual void apply(void*) = 0;
        virtual ~deleter_base() {}
    };
    template <typename Deleter> struct deleter : public deleter_base {
        deleter(Deleter d) : d_(d) {}
        virtual void apply(void* p) { d_(static_cast<T*>(p)); }
        Deleter d_;
    };
    public:
    template <typename Deleter> smartptr_te_lb(T* p, Deleter d) :
        p_(p),
        d_(sizeof(Deleter) > sizeof(buf_) ? new deleter<Deleter>(d) : new (buf_) deleter<Deleter>(d)) 
    {}
    ~smartptr_te_lb() {
        d_->apply(p_);
        if (static_cast<void*>(d_) == static_cast<void*>(buf_)) {
            d_->~deleter_base();
        } else {
            delete d_;
        }
    }
    T* operator->() { return p_; }
    const T* operator->() const { return p_; }
    private:
    T* p_;
    deleter_base* d_;
    char buf_[16];
};

struct deleter {
    template <typename T> void operator()(T* p) { delete p; }
};


void BM_rawptr(benchmark::State& state) {
    deleter d;
    for (auto _ : state) {
        int* p = new int;
        d(p);
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_smartptr(benchmark::State& state) {
    deleter d;
    for (auto _ : state) {
        smartptr<int, deleter> p(new int, d);
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_smartptr_te(benchmark::State& state) {
    deleter d;
    for (auto _ : state) {
        smartptr_te<int> p(new int, d);
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_smartptr_te_lb(benchmark::State& state) {
    deleter d;
    for (auto _ : state) {
        smartptr_te_lb<int> p(new int, d);
    }
    state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_rawptr);
BENCHMARK(BM_smartptr);
BENCHMARK(BM_smartptr_te);
BENCHMARK(BM_smartptr_te_lb);

BENCHMARK_MAIN();

