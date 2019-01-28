#include <utility>
#include <iostream>

class C {
    public:
    explicit C(int i) : i_(i) {}
    void swap(C& rhs) noexcept {
        using std::swap;
        swap(i_, rhs.i_);
    }
    int i_;
};


int main() {
    C c1(1), c2(2);
    std::cout << "\nInitial:" << std::endl;
    std::cout << "c1: " << c1.i_ << " c2: " << c2.i_ << std::endl;

    //std::swap(c1, c2);  // Not the best way
    c1.swap(c2);
    std::cout << "\nAfter swap:" << std::endl;
    std::cout << "c1: " << c1.i_ << " c2: " << c2.i_ << std::endl;
}
