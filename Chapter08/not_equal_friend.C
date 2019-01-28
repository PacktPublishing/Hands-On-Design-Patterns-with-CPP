#include <iostream>

template <typename D> struct not_equal {
    friend bool operator!=(const D& lhs, const D& rhs) { return !(lhs == rhs); }
};

class C : public not_equal<C> {
    int i_;
    public:
    C(int i) : i_(i) {}
    friend bool operator==(const C& lhs, const C& rhs) { return lhs.i_ == rhs.i_; }
};


class D : public not_equal<D> {
    int i_;
    public:
    D(int i) : i_(i) {}
    bool operator==(const D& rhs) const { return i_ == rhs.i_; }
};

int main() {
    C c1(1);
    C c2(1);
    C c3(2);
    std::cout << "c1/c2: " << (c1 == c2) << " " << (c1 != c2) << std::endl;
    std::cout << "c1/c3: " << (c1 == c3) << " " << (c1 != c3) << std::endl;

    D d1(1);
    D d2(1);
    D d3(2);
    std::cout << "d1/d2: " << (d1 == d2) << " " << (d1 != d2) << std::endl;
    std::cout << "d1/d3: " << (d1 == d3) << " " << (d1 != d3) << std::endl;
}
