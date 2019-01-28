#include <iostream>
#include <memory>

class Base {
    public:
    virtual std::unique_ptr<Base> clone() const = 0;
};

class Derived : public Base {
    public:
    std::unique_ptr<Base> clone() const { return std::unique_ptr<Base>(new Derived(*this)); }
};

int main() {
    std::unique_ptr<Base> b0(new Derived);
    std::unique_ptr<Base> b1 = b0->clone();
}

