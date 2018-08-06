#include <iostream>
#include <memory>

class Base {
    public:
    virtual Base* clone() const = 0;
};

template <typename Derived> class ClonerBase : public Base {
    public:
    Base* clone() const {
        return new Derived(*static_cast<const Derived*>(this));
    }
};

class Derived : public ClonerBase<Derived> {
    public:
};

int main() {
    Base* b0(new Derived);
    Base* b1 = b0->clone();
}

