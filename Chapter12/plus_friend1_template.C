#include <iostream>

template <typename T> class C {
    T x_;
    public:
    C(T x) : x_(x) {}
    template <typename U> friend C<U> operator+(const C<U>& lhs, const C<U>& rhs) { return C<U>(lhs.x_ + rhs.x_); }
    friend std::ostream& operator<<(std::ostream& out, const C& c) {
        out << c.x_;
        return out;
    }
};

int main() {
    C<int> c1(5), c2(7);
    std::cout << (c1 + c2) << std::endl;
    //std::cout << (c1 + 3) << std::endl;
    //std::cout << (3 + c1) << std::endl;
}

