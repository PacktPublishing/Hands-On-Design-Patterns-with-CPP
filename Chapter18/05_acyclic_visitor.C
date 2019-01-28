// 01a as acyclic visitor
#include <iostream>
#include <memory>
#include <cassert>

class PetVisitor {
    public:
    virtual ~PetVisitor() {}
};

class Pet {
    public:
    virtual ~Pet() {}
    Pet(const std::string& color) : color_(color) {}
    const std::string& color() const { return color_; }
    virtual void accept(PetVisitor& v) = 0;
    private:
    std::string color_;
};

class Cat;
class CatVisitor {
    public:
    virtual void visit(Cat* c) = 0;
};

class Dog;
class DogVisitor {
    public:
    virtual void visit(Dog* d) = 0;
};

class Cat : public Pet {
    public:
    Cat(const std::string& color) : Pet(color) {}
    void accept(PetVisitor& v) override {
        if (CatVisitor* cv = dynamic_cast<CatVisitor*>(&v))
            cv->visit(this);
        else {  // Handle error
            assert(false);
        }
    }
};

class Dog : public Pet {
    public:
    Dog(const std::string& color) : Pet(color) {}
    void accept(PetVisitor& v) override {
        if (DogVisitor* dv = dynamic_cast<DogVisitor*>(&v))
            dv->visit(this);
        else {  // Handle error
            assert(false);
        }
    }
};

class FeedingVisitor : public PetVisitor, public CatVisitor, public DogVisitor {
    public:
    void visit(Cat* c) override { std::cout << "Feed tuna to the " << c->color() << " cat" << std::endl; }
    void visit(Dog* d) override { std::cout << "Feed steak to the " << d->color() << " dog" << std::endl; }
};

class PlayingVisitor : public PetVisitor, public CatVisitor, public DogVisitor {
    public:
    void visit(Cat* c) override { std::cout << "Play with feather with the " << c->color() << " cat" << std::endl; }
    void visit(Dog* d) override { std::cout << "Play fetch with the " << d->color() << " dog" << std::endl; }
};

class BathingVisitor : public PetVisitor, public DogVisitor {
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
