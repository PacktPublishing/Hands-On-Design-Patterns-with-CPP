#include <iostream>

template <typename T> void f(T) { std::cout << "f(T) " << sizeof(T::i) << std::endl; }
void f(...) { std::cout << "f(...)" << std::endl; }

struct A {
    typedef int t;
    t i;
};

int main() {
    // This does not compile:
    // SFINAE does not suppress errors in the body of the function!
    f(0);
    A a{5};
    f(a);
}
