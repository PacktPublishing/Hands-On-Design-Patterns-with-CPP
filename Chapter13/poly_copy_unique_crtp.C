#include <iostream>
#include <memory>

template <typename Derived> class Base {
    public:
    virtual std::unique_ptr<Derived> clone() const = 0;
    std::unique_ptr<Derived> clone1() const {
        return std::unique_ptr<Derived>(new Derived(*static_cast<const Derived*>(this)));
    }
};

class Derived : public Base<Derived> {
    public:
    std::unique_ptr<Derived> clone() const { return std::unique_ptr<Derived>(new Derived(*this)); }
};

int main() {
    std::unique_ptr<Derived> b0(new Derived);
    std::unique_ptr<Derived> b1 = b0->clone1();
}

