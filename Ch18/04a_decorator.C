// Universal decorator
#include <iostream>
#include <string>
using std::cout;
using std::endl;

template <typename Callable> class DebugDecorator {
    public:
    DebugDecorator(const Callable& c, const char* s) : c_(c), s_(s) {}
    template <typename ... Args> auto operator()(Args&& ... args) const {
        cout << "Invoking " << s_ << endl;
        auto res = c_(std::forward<Args>(args) ...);
        cout << "Result: " << res << endl;
        return res;
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
}
