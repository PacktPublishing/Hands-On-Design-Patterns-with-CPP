#include <iostream>

template <typename T> void f(T i, typename T::t& j) { std::cout << "f(T, T::t)" << std::endl; }
template <typename T> void f(T i, T j) { std::cout << "f(T, T)" << std::endl; }

struct A {
    struct t { int i; };
    t i;
};

int main() {
    A a{5};

    f(a, a.i);
    f(5, 7);
}
