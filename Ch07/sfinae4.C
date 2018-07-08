#include <iostream>

template <typename T, size_t N = T::N> void f(T t, char(*)[N % 2] = NULL) { std::cout << "N=" << N << " is odd" << std::endl; }
template <typename T, size_t N = T::N> void f(T t, char(*)[1 - N % 2] = NULL) { std::cout << "N=" << N << " is even" << std::endl; }

struct A {
    enum {N = 5};
};
struct B {
    enum {N = 8};
};

int main() {
    A a;
    B b;
    f(a);
    f(b);
}
