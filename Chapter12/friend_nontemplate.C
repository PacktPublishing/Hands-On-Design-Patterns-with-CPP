#include <iostream>

class C {
    int x_;
    public:
    C(int x) : x_(x) {}
    friend std::ostream& operator<<(std::ostream& out, const C& c);
    friend C increase(C c, int dx);
};
std::ostream& operator<<(std::ostream& out, const C& c) {
    out << c.x_;
    return out;
}
C increase(C c, int dx) { return C(c.x_ + dx); }

class D {
    int x_;
    public:
    D(int x) : x_(x) {}
    friend std::ostream& operator<<(std::ostream& out, const D& d) {
        out << d.x_;
        return out;
    }
    friend D increase(D c, int dx) { return D(c.x_ + dx); }
};

int main() {
    C c1(5);
    std::cout << c1 << " " << increase(c1, 1) << std::endl;
    D d1(5);
    std::cout << d1 << " " << increase(d1, 1) << std::endl;
}
