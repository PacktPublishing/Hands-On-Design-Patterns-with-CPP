#include <iostream>

template <typename T> class C {
    T x_;
    public:
    C(T x) : x_(x) {}
    template <typename U> friend std::ostream& operator<<(std::ostream& out, const C<U>& c);
};
template <typename U> std::ostream& operator<<(std::ostream& out, const C<U>& c) {
    out << c.x_;
    return out;
}

template <typename T> class D {
    T x_;
    public:
    D(T x) : x_(x) {}
    template <typename U> friend std::ostream& operator<<(std::ostream& out, const D<U>& d) {
        out << d.x_;
        return out;
    }
};

int main() {
    C<int> c1(5);
    std::cout << c1 << std::endl;
    D<int> d1(5);
    std::cout << d1 << std::endl;
}
