#include <iostream>
#include <typeinfo>

class A {
    public:
    A() { std::cout << "A::A(): " << typeid(*this).name() << std::endl; }
    ~A() { std::cout << "A::~A(): " << typeid(*this).name() << std::endl; }
};

class B : public A {
    public:
    B() { std::cout << "B::B(): " << typeid(*this).name() << std::endl; }
    ~B() { std::cout << "B::~B(): " << typeid(*this).name() << std::endl; }
};

class C : public B {
    public:
    C() { std::cout << "C::C(): " << typeid(*this).name() << std::endl; }
    ~C() { std::cout << "C::~C(): " << typeid(*this).name() << std::endl; }
};

int main() {
    C c;
}
