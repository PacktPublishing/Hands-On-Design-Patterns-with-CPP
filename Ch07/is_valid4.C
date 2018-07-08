#include <cstdlib>
#include <iostream>

// Default constructor and destructor.
struct A {
};

// Default constructor and destructor, assignment operator, addition and subtraction.
struct B {
    int i;
    B& operator=(int j) { i = j; return *this; }
};
B operator+(const B& x, const B& y) { B t = { x.i + y.i }; return t; }
B operator+(const B& x, int y) { B t = { x.i + y }; return t; }
B operator+(int x, const B& y) { B t = { x + y.i }; return t; }
B operator-(const B& x, const B& y) { B t = { x.i - y.i }; return t; }
B operator-(const B& x, int y) { B t = { x.i - y }; return t; }
B operator-(int x, const B& y) { B t = { x - y.i }; return t; }

// No default constructor, destructor, cast to integer.
struct C {
    int i;
    C(const A&) {}
    operator int() const { return i; }
};

// Default constructor, no destructor.
class D {
    public:
    int i;
    D() : i(0) {}
    private:
    ~D() {}
};

// Not copyable.
class E {
    public:
    int i;
    E() : i(0) {}
    private:
    E(const E&);
    E& operator=(const E&);
};

// Smart pointer.
class Ptr {
    public:
    explicit Ptr(int* p = NULL) : p_(p) {}
    ~Ptr() { delete p_; }
    Ptr& operator=(int* p) { delete p_; p_ = p; return *this; }
    int* operator->() { return p_; }
    const int* operator->() const { return p_; }
    int& operator*() { return *p_; }
    const int& operator*() const { return *p_; }

    private:
    int* p_;
};

template <typename T> class SmartPtr {
    public:
    explicit SmartPtr(T* p = NULL) : p_(p) {}
    ~SmartPtr() { if (p_) delete p_; }
    T* operator->() { return p_; }
    const T* operator->() const { return p_; }
    T& operator*() { return *p_; }
    const T& operator*() const { return *p_; }

    private:
    T* p_;
};

#define EXPECT_TRUE(X) if (!(X)) std::cout << #X << " failed" << std::endl
#define EXPECT_FALSE(X) if (X) std::cout << "!" << #X << " failed" << std::endl

namespace IsValid {
struct compile_check_passes { char c; };
struct compile_check_fails { compile_check_passes x; char c[12345]; };
} // namespace IsValid

#define DEFINE_TEST2(NAME, EXPR) \
struct NAME { \
    template <typename T1, typename T2> IsValid::compile_check_passes operator()(const T1& x1, const T2& x2, char (*a)[sizeof(EXPR)] = NULL); \
    template <typename T1, typename T2> IsValid::compile_check_fails operator()(const T1& x1, const T2& x2, ...); \
}

DEFINE_TEST2(addable2, x1 + x2);

#define IS_VALID2(TEST, X1, X2) sizeof(TEST()(X1, X2, 0)) == sizeof(IsValid::compile_check_passes)

int main() {
    int i = 0;
    double x = 0;
    char* p = NULL;
    A a;
    B b;
    EXPECT_TRUE(IS_VALID2(addable2, i, x));
    EXPECT_TRUE(IS_VALID2(addable2, p, i));
    EXPECT_FALSE(IS_VALID2(addable2, p, x));
    EXPECT_FALSE(IS_VALID2(addable2, a, 1));
    EXPECT_FALSE(IS_VALID2(addable2, &a, &b));
    EXPECT_TRUE(IS_VALID2(addable2, b, 1));
}
