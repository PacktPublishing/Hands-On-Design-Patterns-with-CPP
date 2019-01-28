#include <iostream>

void f(int i) { std::cout << "f(int)" << std::endl; }
template <typename T> void f(T i) { std::cout << "f(T)" << std::endl; }
template <typename T> void f(T* i) { std::cout << "f(T*)" << std::endl; }

int main() {
    f(5);
    f(5l);
    int i = 0;
    f(&i);
    f(NULL);
    f(nullptr);
    void* p = &i;
    f(p);
}
