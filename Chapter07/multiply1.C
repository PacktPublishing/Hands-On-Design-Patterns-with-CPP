#include <iostream>

template <typename T> auto increase(const T& x, size_t n) -> decltype(T(x) *= n)
{
    T y(x);
    return y *= n;
}
template <typename T> auto increase(const T& x, size_t n) -> decltype(x * n)
{
    return x * n;
}

class A {
    public:
    explicit A(int i) : i_(i) {}
    A operator*=(size_t n) {
        std::cout << "A*=n" << std::endl;
        return A(i_ * n);
    }
    private:
    int i_;
};

class B {
    public:
    explicit B(int i) : i_(i) {}
    friend B operator*(const B& lhs, size_t rhs) {
        std::cout << "B*n" << std::endl;
        return B(lhs.i_ * rhs);
    }
    private:
    int i_;
};

class AB {
    public:
    explicit AB(int i) : i_(i) {}
    AB operator*=(size_t n) {
        std::cout << "AB*=n" << std::endl;
        return AB(i_ * n);
    }
    friend AB operator*(const AB& lhs, size_t rhs) {
        std::cout << "AB*n" << std::endl;
        return AB(lhs.i_ * rhs);
    }
    private:
    int i_;
};

int main() {
    A a(2); increase(a, 3);
    B b(2); increase(b, 3);
    //AB ab(2); increase(ab, 3);
}
