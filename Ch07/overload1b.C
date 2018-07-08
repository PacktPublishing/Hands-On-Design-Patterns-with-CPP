#include <iostream>

void f(int i) { std::cout << "f(int)" << std::endl; }
void f(long i, long j) { std::cout << "f(long, long)" << std::endl; }
void f(double i, double j = 0) { std::cout << "f(double, double = 0)" << std::endl; }

int main() {
    f(5);
    f(5l, 5);
    //f(5, 5);
    f(5.0);
    //f(5l);
    f(5.0f);
}
