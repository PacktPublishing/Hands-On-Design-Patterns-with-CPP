#include <utility>
#include <iostream>

using std::swap;

namespace N {
class C {
    public:
    explicit C(int i) : i_(i) {}
    C(const C& rhs) : i_(rhs.i_) { std::cout << "C(const C&)" << std::endl; }
    C& operator=(const C& rhs) { i_ = rhs.i_; std::cout << "C = C&" << std::endl; return *this; }
    void swap(C& rhs) noexcept { std::cout << "C::swap(C&)" << std::endl; std::swap(i_, rhs.i_); }
    int i_;
};
}
namespace std {
void swap(N::C& lhs, N::C& rhs) noexcept { std::cout << "std::swap(C&, C&)" << std::endl; lhs.swap(rhs); }
}


int main() {
    N::C c1(1), c2(2);
    std::cout << "\nInitial:" << std::endl;
    std::cout << "c1: " << c1.i_ << " c2: " << c2.i_ << std::endl;

    std::swap(c1, c2);
    std::cout << "\nAfter swap:" << std::endl;
    std::cout << "c1: " << c1.i_ << " c2: " << c2.i_ << std::endl;

    swap(c1, c2);
    std::cout << "\nAfter swap:" << std::endl;
    std::cout << "c1: " << c1.i_ << " c2: " << c2.i_ << std::endl;
}
