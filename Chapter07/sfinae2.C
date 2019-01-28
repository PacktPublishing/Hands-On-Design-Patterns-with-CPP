#include <iostream>

template <typename T> void f(T* i) { std::cout << "f(T*)" << std::endl; }
template <typename T> void f(int T::* p) { std::cout << "f(T::*)" << std::endl; }

struct A {
    int i;
    int f() { return 0; }
};

int main() {
    A a;
    f(&a.i);
    f(&A::i);
}
