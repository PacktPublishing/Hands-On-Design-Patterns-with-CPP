#include <iostream>

template <typename T> class C {
    T x_;
    public:
    C(T x) : x_(x) {}
    template <typename U> friend C<U> operator+(const C<U>& lhs, const C<U>& rhs);
    friend std::ostream& operator<<(std::ostream& out, const C& c) {
        out << c.x_;
        return out;
    }
};
template <typename T> C<T> operator+(const C<T>& lhs, const C<T>& rhs) { return C<T>(lhs.x_ + rhs.x_); }

int main() {
    C<int> c1(5), c2(7);
    std::cout << (c1 + c2) << std::endl;
    //std::cout << (c1 + 3) << std::endl;
    //std::cout << (3 + c1) << std::endl;
}
