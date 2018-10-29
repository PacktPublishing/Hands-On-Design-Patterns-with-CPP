// 04b with composable decorators
#include <iostream>
#include <string>
#include <mutex>
using std::cout;
using std::endl;

template <typename Res> struct report {
    Res res;
    template <typename Callable, typename ... Args>
        report(const Callable& c, Args&& ... args) :
            res(c(std::forward<Args>(args) ...)) {
                cout << "Result: " << res << endl;
            }
    Res operator()() { return res; }
};
template <> struct report<void> {
    template <typename Callable, typename ... Args>
        report(const Callable& c, Args&& ... args) {
            c(std::forward<Args>(args) ...);
            cout << "Done" << endl;
        }
    void operator()() {}
};

template <typename Callable> class DebugDecorator {
    public:
    DebugDecorator(const Callable& c, const char* s) : c_(c), s_(s) {}
    template <typename ... Args> auto operator()(Args&& ... args) const {
        cout << "Invoking " << s_ << endl;
        using res_t = typename std::result_of<Callable(Args ...)>::type;
        report<res_t> res(c_, std::forward<Args>(args) ...);
        return res();
    }

    private:
    const Callable& c_;
    const std::string s_;
};

template <typename Callable>
auto decorate_debug(const Callable& c, const char* s) {
    return DebugDecorator<Callable>(c, s);
}

template <typename Callable> class LockDecorator {
    public:
    LockDecorator(const Callable& c, std::mutex& m) : c_(c), m_(m) {}
    template <typename ... Args> auto operator()(Args&& ... args) const {
        std::lock_guard<std::mutex> l(m_);
        return c_(std::forward<Args>(args) ...);
    }

    private:
    const Callable& c_;
    std::mutex& m_;
};

template <typename Callable>
auto decorate_lock(const Callable& c, std::mutex& m) {
    return LockDecorator<Callable>(c, m);
}

struct R {
    int value() const { return rand(); }
};

int main() {
    std::mutex m;
    auto f1 = decorate_lock(decorate_debug([](int i) { return i; }, "i->i"), m);
    f1(5);
    auto f2 = decorate_debug(decorate_lock([](int i, int j) { return i + j; }, m), "i+j");
    f2(5, 3);
    R r;
    auto f0 = decorate_lock([&]() { return r.value(); }, m);
    f0(); f0();
    auto incr = decorate_debug([](int& x) { ++x; }, "++x");
    int i = 0;
    incr(i);
}
