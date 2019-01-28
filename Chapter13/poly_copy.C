#include <iostream>
#include <memory>

class Base {
    public:
    virtual Base* clone() const = 0;
};

class Derived : public Base {
    public:
    Derived* clone() const { return new Derived(*this); }
};

int main() {
    Base* b0 = new Derived;
    Base* b1 = b0->clone();
}
