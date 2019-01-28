#include <iostream>
#include <algorithm>
#include <vector>
#include <list>

struct yes { char c; };
struct no { char c; yes c1; };
template <typename T> yes test_have_sort(decltype(static_cast<void (T::*)()>(&T::sort)));
template <typename T> no test_have_sort(...);
template <typename T> yes test_have_range(decltype(static_cast<typename T::iterator (T::*)()>(&T::begin)), decltype(static_cast<typename T::iterator (T::*)()>(&T::end)));
template <typename T> no test_have_range(...);

template <typename T, bool have_sort, bool have_range> struct fast_sort_helper;

template <typename T>
struct fast_sort_helper<T, true, true> {
    static void fast_sort(T& x) {
        std::cout << "Sorting with T::sort, ignoring std::sort" << std::endl;
        x.sort();
    }
};

template <typename T>
struct fast_sort_helper<T, true, false> {
    static void fast_sort(T& x) {
        std::cout << "Sorting with T::sort" << std::endl;
        x.sort();
    }
};

template <typename T>
struct fast_sort_helper<T, false, true> {
    static void fast_sort(T& x) {
        std::cout << "Sorting with std::sort" << std::endl;
        std::sort(x.begin(), x.end());
    }
};

template <typename T>
struct fast_sort_helper<T, false, false> {
    static void fast_sort(T& x) {
        std::cout << "No sort available" << std::endl;
    }
};

template <typename T>
void fast_sort(T& x) {
    fast_sort_helper<T, sizeof(test_have_sort<T>(NULL)) == sizeof(yes), sizeof(test_have_range<T>(NULL, NULL)) == sizeof(yes)>::fast_sort(x);
}


class A {
    public:
    void sort() {}
};

class A1 {
    public:
    void sort() {}
    void sort(int i) {}
};

class B {
    public:
    typedef int* iterator;
    iterator begin() { return i; }
    iterator end() { return i + 10; }
    int i[10];
};

class AB {
    public:
    typedef int* iterator;
    void sort() {}
    iterator begin() { return i; }
    iterator end() { return i + 10; }
    int i[10];
};

class B1 {
    public:
    typedef int* iterator;
    typedef const int* const_iterator;
    iterator begin() { return i; }
    const_iterator begin() const { return i; }
    iterator end() { return i + 10; }
    const_iterator end() const { return i + 10; }
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
    A1 a1; fast_sort(a1);
    B1 b1; fast_sort(b1);
    std::vector<int> v; fast_sort(v);
    AB ab; fast_sort(ab);
    std::list<int> l; fast_sort(l);
}
