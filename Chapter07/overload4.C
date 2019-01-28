#include <iostream>

void f(int i) { std::cout << "f(int)" << std::endl; }
void f(...) { std::cout << "f(...)" << std::endl; }

struct A {};
struct B {
    operator int() const { return 0; }
};

int main() {
    f(5);
    f(5l);
    f(5.0);
    A a;
    f(a);
    B b;
    f(b);
}
