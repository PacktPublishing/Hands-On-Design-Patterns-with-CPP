#include <utility>
#include <iostream>

class A {
    public:
    explicit A(int i) : i_(i) {}
    A(const A& rhs) : i_(rhs.i_) { std::cout << "A(const A&)" << std::endl; }
    A& operator=(const A& rhs) { i_ = rhs.i_; std::cout << "A = A&" << std::endl; return *this; }
    int i_;
};


int main() {
    A a1(1), a2(2);
    std::cout << "\nInitial:" << std::endl;
    std::cout << "a1: " << a1.i_ << " a2: " << a2.i_ << std::endl;

    std::swap(a1, a2);  // Not the best way
    std::cout << "\nAfter swap:" << std::endl;
    std::cout << "a1: " << a1.i_ << " a2: " << a2.i_ << std::endl;
}
