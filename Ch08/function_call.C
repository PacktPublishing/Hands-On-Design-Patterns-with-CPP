#include <stdlib.h>

#include "benchmark/benchmark.h"

#define REPEAT2(x) x x
#define REPEAT4(x) REPEAT2(x) REPEAT2(x)
#define REPEAT8(x) REPEAT4(x) REPEAT4(x)
#define REPEAT16(x) REPEAT8(x) REPEAT8(x)
#define REPEAT32(x) REPEAT16(x) REPEAT16(x)
#define REPEAT(x) REPEAT32(x)

namespace no_polymorphism {
class A {
    public:
    A() : i_(0) {}
    void f(int i) { i_ += i; }
    int get() const { return i_; }
    protected:
    int i_;
};
} // namespace no_polymorphism

namespace dynamic_polymorphism {
class B {
    public:
    B() : i_(0) {}
    virtual ~B() {}
    virtual void f(int i) = 0;
    int get() const { return i_; }
    protected:
    int i_;
};
class D : public B {
    public:
    void f(int i) { i_ += i; }
};
} // namespace dynamic_polymorphism

namespace static_polymorphism {
template <typename D> class B {
    public:
    B() : i_(0) {}
    virtual ~B() {}
    void f(int i) { static_cast<D*>(this)->f(i); }
    int get() const { return i_; }
    protected:
    int i_;
};
class D : public B<D> {
    public:
    void f(int i) { i_ += i; }
};
} // namespace static_polymorphism

namespace static_polymorphism1 {
template <typename D> class B {
    public:
    B() : i_(0) {}
    void f(int i) { derived()->f(i); }
    int get() const { return i_; }
    protected:
    int i_;
    private:
    D* derived() { return static_cast<D*>(this); }
};
template <typename D> void apply(B<D>* b, int& i) { b->f(++i); }
class D : public B<D> {
    public:
    void f(int i) { i_ += i; }
};
} // namespace static_polymorphism1

void BM_none(benchmark::State& state) {
    no_polymorphism::A* a = new no_polymorphism::A;
    int i = 0;
    for (auto _ : state) {
        REPEAT(a->f(++i);)
    }
    benchmark::DoNotOptimize(a->get());
    state.SetItemsProcessed(32*state.iterations());
    delete a;
}

void BM_dynamic(benchmark::State& state) {
    dynamic_polymorphism::B* b = new dynamic_polymorphism::D;
    int i = 0;
    for (auto _ : state) {
        REPEAT(b->f(++i);)
    }
    benchmark::DoNotOptimize(b->get());
    state.SetItemsProcessed(32*state.iterations());
    delete b;
}

void BM_static(benchmark::State& state) {
    static_polymorphism::B<static_polymorphism::D>* b = new static_polymorphism::D;
    int i = 0;
    for (auto _ : state) {
        REPEAT(b->f(++i);)
    }
    benchmark::DoNotOptimize(b->get());
    state.SetItemsProcessed(32*state.iterations());
    delete b;
}

void BM_static1(benchmark::State& state) {
    static_polymorphism1::D d;
    static_polymorphism1::B<static_polymorphism1::D>* b = &d;
    int i = 0;
    for (auto _ : state) {
        REPEAT(apply(b, i);)
    }
    benchmark::DoNotOptimize(b->get());
    state.SetItemsProcessed(32*state.iterations());
}

BENCHMARK(BM_none);
BENCHMARK(BM_dynamic);
BENCHMARK(BM_static);
BENCHMARK(BM_static1);

BENCHMARK_MAIN();
