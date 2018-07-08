#include <iostream>

void f(int i, int j) { std::cout << "f(int, int)" << std::endl; }
void f(long i, long j) { std::cout << "f(long, long)" << std::endl; }

int main() {
    f(5, 5);
    f(5l, 5l);
    //f(5, 5l);
    f(5, 5.0);
}
