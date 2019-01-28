#include <iostream>

void f(int i) { std::cout << "f(int)" << std::endl; }
void f(long i) { std::cout << "f(long)" << std::endl; }
template <typename T> void f(T i) { std::cout << "f(T)" << std::endl; }

int main() {
    f(5);
    f(5l);
    f(5.0);
}
