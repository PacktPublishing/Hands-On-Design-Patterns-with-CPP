#include <iostream>

class C {
    int x_;
    public:
    C(int x) : x_(x) {}
    C operator+(const C& rhs) const { return C(x_ + rhs.x_); }
    friend std::ostream& operator<<(std::ostream& out, const C& c) {
        out << c.x_;
        return out;
    }
};

int main() {
    C c1(5), c2(7);
    std::cout << (c1 + c2) << std::endl;
    std::cout << (c1 + 3) << std::endl;
    //std::cout << (3 + c1) << std::endl;
}
