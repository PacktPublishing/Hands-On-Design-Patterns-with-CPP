#include <iostream>

template <typename D> class B {
    public:
    friend bool operator!=(const D& lhs, const D& rhs) { return !(lhs == rhs); }
};

template <typename T> class C : public B<C<T>> {
    T x_;
    public:
    C(T x) : x_(x) {}
    friend bool operator==(const C& lhs, const C& rhs) { return lhs.x_ == rhs.x_; }
    friend std::ostream& operator<<(std::ostream& out, const C& c) {
        out << c.x_;
        return out;
    }
};

int main() {
    C<int> c1(5), c2(7);
    std::cout << (c1 == c2) << (c1 != c2) << std::endl;
    std::cout << (c1 == 5) << (c1 != 5) << std::endl;
    std::cout << (7 == c2) << (7 != c2) << std::endl;
}

