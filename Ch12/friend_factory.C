#include <iostream>

template <typename T> class C {
    T x_;
    public:
    C(T x) : x_(x) {}
    friend std::ostream& operator<<(std::ostream& out, const C& c) {
        out << c.x_;
        return out;
    }
    friend C increase(C c, T dx) { return C(c.x_ + dx); }
};

namespace ND {
template <typename T> class D {
    T x_;
    public:
    D(T x) : x_(x) {}
    friend std::ostream& operator<<(std::ostream& out, const D& d) {
        out << d.x_;
        return out;
    }
    friend D increase(D d, T dx) { return D(d.x_ + dx); }
};
}

int main() {
    C<int> c1(5);
    std::cout << c1 << " " << increase(c1, 1) << std::endl;
    ND::D<int> d1(5);
    std::cout << d1 << " " << increase(d1, 2) << std::endl;
}

