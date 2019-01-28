// 05a with more template common code
#include <iostream>
#include <memory>
#include <cassert>

class VisitorBase {
    public:
    virtual ~VisitorBase() {}
};

template <typename Visitable>
class Visitor {
    public:
    virtual void visit(Visitable* p) = 0;
};

template <typename Base, typename Visitable>
class VisitableBase : public Base {
    public:
    using Base::Base;
    void accept(VisitorBase& vb) override {
        if (Visitor<Visitable>* v = dynamic_cast<Visitor<Visitable>*>(&vb))
            v->visit(static_cast<Visitable*>(this));
        else {  // Handle error
            assert(false);
        }
    }
};

class Pet {
    public:
    virtual ~Pet() {}
    Pet(const std::string& color) : color_(color) {}
    const std::string& color() const { return color_; }
    virtual void accept(VisitorBase& v) = 0;
    private:
    std::string color_;
};

template <typename Visitable>
using PetVisitable = VisitableBase<Pet, Visitable>;

class Cat : public PetVisitable<Cat> {
    using PetVisitable<Cat>::PetVisitable;
};

class Dog : public PetVisitable<Dog> {
    using PetVisitable<Dog>::PetVisitable;
};

class FeedingVisitor : public VisitorBase, public Visitor<Cat>, public Visitor<Dog> {
    public:
    void visit(Cat* c) override { std::cout << "Feed tuna to the " << c->color() << " cat" << std::endl; }
    void visit(Dog* d) override { std::cout << "Feed steak to the " << d->color() << " dog" << std::endl; }
};

class PlayingVisitor : public VisitorBase, public Visitor<Cat>, public Visitor<Dog> {
    public:
    void visit(Cat* c) override { std::cout << "Play with feather with the " << c->color() << " cat" << std::endl; }
    void visit(Dog* d) override { std::cout << "Play fetch with the " << d->color() << " dog" << std::endl; }
};

class BathingVisitor : public VisitorBase, public Visitor<Dog> {
    public:
    void visit(Dog* d) override { std::cout << "Wash the " << d->color() << " dog" << std::endl; }
};

int main() {
    std::unique_ptr<Pet> c(new Cat("orange"));
    std::unique_ptr<Pet> d(new Dog("brown"));

    FeedingVisitor fv;
    c->accept(fv);
    d->accept(fv);

    PlayingVisitor pv;
    c->accept(pv);
    d->accept(pv);

    BathingVisitor bv;
    //c->accept(bv);    // Error
    d->accept(bv);
}
