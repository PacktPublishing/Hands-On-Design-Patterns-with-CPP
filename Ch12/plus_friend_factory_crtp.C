#include <iostream>

template <typename D> class B {
    public:
    friend D operator+(const D& lhs, const D& rhs) { D res(lhs); res += rhs; return res; }
    friend std::ostream& operator<<(std::ostream& out, const D& d) {
        d.print(out);
        return out;
    }
};

template <typename T> class C : public B<C<T>> {
    T x_;
    public:
    C(T x) : x_(x) {}
    C operator+=(const C& incr) { x_ += incr.x_; return *this; }
    void print(std::ostream& out) const {
        out << x_;
    }
};

int main() {
    C<int> c1(5), c2(7);
    std::cout << (c1 + c2) << std::endl;
    std::cout << (c1 + 3) << std::endl;
    std::cout << (3 + c1) << std::endl;
}

