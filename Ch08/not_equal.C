#include <iostream>

template <typename D> struct not_equal {
    bool operator!=(const D& rhs) const { return !static_cast<const D*>(this)->operator==(rhs); }
};

class C : public not_equal<C> {
    int i_;
    public:
    C(int i) : i_(i) {}
    bool operator==(const C& rhs) const { return i_ == rhs.i_; }
};

int main() {
    C c1(1);
    C c2(1);
    C c3(2);
    std::cout << "c1/c2: " << (c1 == c2) << " " << (c1 != c2) << std::endl;
    std::cout << "c1/c3: " << (c1 == c3) << " " << (c1 != c3) << std::endl;
}
