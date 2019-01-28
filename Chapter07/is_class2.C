#include <iostream>

template<typename T>
class is_class {
    template<typename C> static char test(int C::*); // selected if C is a class type
    template<typename C> static int  test(...);      // selected otherwise
    public:
    //enum { value = sizeof(test<T>(NULL)) == 1 };
    static constexpr bool value = sizeof(test<T>(NULL)) == 1;
};

struct A {
};

int main() {
    std::cout << is_class<int>::value << std::endl;
    std::cout << is_class<A>::value << std::endl;
}
