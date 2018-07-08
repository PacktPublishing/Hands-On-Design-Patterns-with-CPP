#include <iostream>
#include <algorithm>

struct have_sort { char c; };
struct have_range { char c; have_sort c1; };
struct have_nothing { char c; have_range c1; };
template <typename T> have_sort test_sort(decltype(&T::sort), decltype(&T::sort));
template <typename T> have_range test_sort(decltype(&T::begin), decltype(&T::end));
template <typename T> have_nothing test_sort(...);

template <typename T>
typename std::enable_if<sizeof(test_sort<T>(NULL, NULL)) == sizeof(have_sort)>::type fast_sort(T& x) {
    std::cout << "Sorting with T::sort" << std::endl;
    x.sort();
}
template <typename T>
typename std::enable_if<sizeof(test_sort<T>(NULL, NULL)) == sizeof(have_range)>::type fast_sort(T& x) {
    std::cout << "Sorting with std::sort" << std::endl;
    std::sort(x.begin(), x.end());
}
template <typename T>
typename std::enable_if<sizeof(test_sort<T>(NULL, NULL)) == sizeof(have_nothing)>::type fast_sort(T& x) {
    std::cout << "No sort available" << std::endl;
}

class A {
    public:
    void sort() {}
};

class B {
    public:
    int* begin() { return i; }
    int* end() { return i + 10; }
    int i[10];
};

class C {
    public:
    void f() {}
};

int main() {
    A a; fast_sort(a);
    B b; fast_sort(b);
    C c; fast_sort(c);
}
