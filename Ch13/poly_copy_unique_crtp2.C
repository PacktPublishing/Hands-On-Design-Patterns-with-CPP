#include <iostream>
#include <memory>

class Base {
    public:
    virtual ~Base() {}
    Base* clone() const;
};

class ClonerBase {
    public:
    virtual Base* clone() const = 0;
};

Base* Base::clone() const {
    dynamic_cast<const ClonerBase*>(this)->clone();
};

template <typename Derived> class Cloner : public ClonerBase {
    public:
    Base* clone() const {
        return new Derived(*static_cast<const Derived*>(this));
    }
};

class Derived : public Base, public Cloner<Derived> {
    public:
};

int main() {
    Base* b0(new Derived);
    Base* b1 = b0->clone();
}

