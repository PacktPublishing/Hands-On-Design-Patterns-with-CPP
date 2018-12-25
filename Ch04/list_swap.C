#include <list>
#include <utility>
#include <iostream>

typedef std::list<int> T;

int main() {
    T t1({1, 2, 3, 4});
    T t2({5, 6, 7, 8});
    std::cout << "\nInitial:" << std::endl;
    std::cout << "t1:"; for (auto x: t1) std::cout << " " << x; std::cout << std::endl;
    std::cout << "t2:"; for (auto x: t2) std::cout << " " << x; std::cout << std::endl;

    t1.swap(t2);
    std::cout << "\nAfter member swap:" << std::endl;
    std::cout << "t1:"; for (auto x: t1) std::cout << " " << x; std::cout << std::endl;
    std::cout << "t2:"; for (auto x: t2) std::cout << " " << x; std::cout << std::endl;

    swap(t1, t2);
    std::cout << "\nAfter non-member swap:" << std::endl;
    std::cout << "t1:"; for (auto x: t1) std::cout << " " << x; std::cout << std::endl;
    std::cout << "t2:"; for (auto x: t2) std::cout << " " << x; std::cout << std::endl;

    std::swap(t1, t2);
    std::cout << "\nAfter std::swap:" << std::endl;
    std::cout << "t1:"; for (auto x: t1) std::cout << " " << x; std::cout << std::endl;
    std::cout << "t2:"; for (auto x: t2) std::cout << " " << x; std::cout << std::endl;
}
