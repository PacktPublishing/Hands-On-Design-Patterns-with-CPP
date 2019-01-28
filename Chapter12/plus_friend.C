#include <iostream>

class C {
    int x_;
    public:
    C(int x) : x_(x) {}
    friend C operator+(const C& lhs, const C& rhs);
    friend std::ostream& operator<<(std::ostream& out, const C& c) {
        out << c.x_;
        return out;
    }
};
C operator+(const C& lhs, const C& rhs) { return C(lhs.x_ + rhs.x_); }

int main() {
    C c1(5), c2(7);
    std::cout << (c1 + c2) << std::endl;
    std::cout << (c1 + 3) << std::endl;
    std::cout << (3 + c1) << std::endl;
}
