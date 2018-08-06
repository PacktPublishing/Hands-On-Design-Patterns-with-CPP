#include <iostream>
#include <typeinfo>

class A {
    public:
    A() {  whoami(); }
    virtual ~A() { whoami(); }
    virtual void whoami() const { std::cout << "A::whoami" << std::endl; }
};

class B : public A {
    public:
    B() { whoami(); }
    ~B() { whoami(); }
    void whoami() const { std::cout << "B::whoami" << std::endl; }
};

class C : public B {
    public:
    C() { whoami(); }
    ~C() { whoami(); }
    void whoami() const { std::cout << "C::whoami" << std::endl; }
};

int main() {
    C* c;
    c.whoami();
}
