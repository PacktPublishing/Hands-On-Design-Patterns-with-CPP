// 04a with support for void
#include <iostream>
#include <string>
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

struct R {
    int value() const { return rand(); }
};

int main() {
    auto f1 = decorate_debug([](int i) { return i; }, "i->i");
    f1(5);
    auto f2 = decorate_debug([](int i, int j) { return i + j; }, "i+j");
    f2(5, 3);
    R r;
    auto f0 = decorate_debug([&]() { return r.value(); }, "rand");
    f0(); f0();
    auto incr = decorate_debug([](int& x) { ++x; }, "++x");
    int i = 0;
    incr(i);
}
