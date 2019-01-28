#include <iostream>

void f(int i, int j) { std::cout << "f(int, int)" << std::endl; }
template <typename T> void f(T i, T* p) { std::cout << "f(T, T*)" << std::endl; }
void f(...) { std::cout << "f(...)" << std::endl; }

int main() {
    f(5, 5);
    int i;
    f(5, &i);
    // This compiles but uses the ... overload
    // template deduction fails because 5l deduces T as long, &i deduces T as int
    f(5l, &i);
    f<int>(5l, &i);
    // This forces the template overload, but does not compile because &i is int*
    f<long>(5l, &i);
}
