#include <iostream>

struct yes { char c; };
struct no { char c; yes c1; };

template <typename T> auto have_star_equal(const T& x, size_t n) -> decltype(T(x) *= n, yes());
no have_star_equal(...);

template <typename T> auto have_star(const T& x, size_t n) -> decltype(x * n, yes());
no have_star(...);

template <typename T> auto have_plus(const T& x) -> decltype(x + x, yes());
no have_plus(...);

template <typename T, bool have_star_equal, bool have_star, bool have_plus> struct increase_helper;

template <typename T, bool have_star, bool have_plus> struct increase_helper<T, true, have_star, have_plus> {
    static auto f(const T& x, size_t n) {
        std::cout << "T *= n" << std::endl;
        T y(x);
        return y *= n;
    }
};

template <typename T, bool have_plus> struct increase_helper<T, false, true, have_plus> {
    static auto f(const T& x, size_t n) {
        std::cout << "T * n" << std::endl;
        return x * n;
    }
};

template <typename T> struct increase_helper<T, false, false, true> {
    static auto f(const T& x, size_t n) {
        std::cout << "T + T + ... + T" << std::endl;
        T y(x);
        for (size_t i = 1; i < n; ++i) y = y + y;
        return y;
    }
};

template <typename T> struct increase_helper<T, false, false, false> {
    static auto f(const T& x, size_t n) {
        std::cout << "Cannot increase" << std::endl;
        return x;
    }
};

template <typename T> auto increase(const T& x, size_t n) {
    return increase_helper<T,
                           sizeof(have_star_equal(x, n)) == sizeof(yes),
                           sizeof(have_star(x, n)) == sizeof(yes),
                           sizeof(have_plus(x)) == sizeof(yes)
                          >::f(x, n);
}

class A {
    public:
    explicit A(int i) : i_(i) {}
    A operator*=(size_t n) {
        std::cout << "A*=n" << std::endl;
        return A(i_ * n);
    }
    private:
    int i_;
};

class B {
    public:
    explicit B(int i) : i_(i) {}
    friend B operator*(const B& lhs, size_t rhs) {
        std::cout << "B*n" << std::endl;
        return B(lhs.i_ * rhs);
    }
    private:
    int i_;
};

class AB {
    public:
    explicit AB(int i) : i_(i) {}
    AB operator*=(size_t n) {
        std::cout << "AB*=n" << std::endl;
        return AB(i_ * n);
    }
    friend AB operator*(const AB& lhs, size_t rhs) {
        std::cout << "AB*n" << std::endl;
        return AB(lhs.i_ * rhs);
    }
    private:
    int i_;
};

class C {
    public:
    explicit C(int i) : i_(i) {}
    friend C operator+(const C& lhs, const C& rhs) {
        std::cout << "C + C" << std::endl;
        return C(lhs.i_ * rhs.i_);
    }
    private:
    int i_;
};

int main() {
    A a(2); increase(a, 3);
    B b(2); increase(b, 3);
    AB ab(2); increase(ab, 3);
    C c(2); increase(c, 3);
}
