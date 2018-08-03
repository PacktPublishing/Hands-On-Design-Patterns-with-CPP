#include <stdlib.h>

#include <new>
#include <memory>

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
template <typename T>
struct deleter1 {
    void operator()(T* p) { delete p; }
};

void BM_rawptr(benchmark::State& state) {
    deleter d;
    for (auto _ : state) {
        int* p = new int(0);
        d(p);
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_uniqueptr_default(benchmark::State& state) {
    for (auto _ : state) {
        std::unique_ptr<int, deleter> p(new int(0));
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_uniqueptr(benchmark::State& state) {
    deleter d;
    for (auto _ : state) {
        std::unique_ptr<int, deleter> q(new int(0), d);
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_sharedptr_default(benchmark::State& state) {
    for (auto _ : state) {
        std::shared_ptr<int> p(new int(0));
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_sharedptr_makeshared(benchmark::State& state) {
    for (auto _ : state) {
        std::shared_ptr<int> p(std::make_shared<int>(0));
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_sharedptr(benchmark::State& state) {
    deleter d;
    for (auto _ : state) {
        std::shared_ptr<int> p(new int(0), d);
    }
    state.SetItemsProcessed(state.iterations());
}

extern std::shared_ptr<int> get_shared_ptr();
void BM_sharedptr_ext(benchmark::State& state) {
    for (auto _ : state) {
        std::shared_ptr<int> p(get_shared_ptr());
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_smartptr(benchmark::State& state) {
    deleter d;
    for (auto _ : state) {
        smartptr<int, deleter> p(new int(0), d);
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_smartptr_te(benchmark::State& state) {
    deleter d;
    for (auto _ : state) {
        smartptr_te<int> p(new int(0), d);
    }
    state.SetItemsProcessed(state.iterations());
}

void BM_smartptr_te_lb(benchmark::State& state) {
    deleter d;
    for (auto _ : state) {
        smartptr_te_lb<int> p(new int(0), d);
    }
    state.SetItemsProcessed(state.iterations());
}

extern smartptr_te<int> get_smartptr_te();
void BM_smartptr_te_ext(benchmark::State& state) {
    for (auto _ : state) {
        smartptr_te<int> p(get_smartptr_te());
    }
    state.SetItemsProcessed(state.iterations());
}

extern smartptr_te_lb<int> get_smartptr_te_lb();
void BM_smartptr_te_lb_ext(benchmark::State& state) {
    for (auto _ : state) {
        smartptr_te_lb<int> p(get_smartptr_te_lb());
    }
    state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_rawptr);
BENCHMARK(BM_uniqueptr_default);
BENCHMARK(BM_uniqueptr);
BENCHMARK(BM_sharedptr_default);
BENCHMARK(BM_sharedptr_makeshared);
BENCHMARK(BM_sharedptr);
BENCHMARK(BM_sharedptr_ext);
BENCHMARK(BM_smartptr);
BENCHMARK(BM_smartptr_te);
BENCHMARK(BM_smartptr_te_ext);
BENCHMARK(BM_smartptr_te_lb);
BENCHMARK(BM_smartptr_te_lb_ext);

BENCHMARK_MAIN();

