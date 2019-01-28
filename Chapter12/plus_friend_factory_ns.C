#include <iostream>

namespace NS {
template <typename T> class C {
    T x_;
    public:
    C(T x) : x_(x) {}
    friend C operator+(const C& lhs, const C& rhs) { return C(lhs.x_ + rhs.x_); }
    friend std::ostream& operator<<(std::ostream& out, const C& c) {
        out << c.x_;
        return out;
    }
};
}

int main() {
    NS::C<int> c1(5), c2(7);
    std::cout << (c1 + c2) << std::endl;
    std::cout << (c1 + 3) << std::endl;
    std::cout << (3 + c1) << std::endl;
}

