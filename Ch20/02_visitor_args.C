// 01a with additional arguments to visitor functions
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <memory>

class Pet;
class Cat;
class Dog;

class PetVisitor {
    public:
    virtual void visit(Cat* c, Pet* p) = 0;
    virtual void visit(Dog* d, Pet* p) = 0;
};

class Pet {
    public:
    virtual ~Pet() {}
    Pet(const std::string& color) : color_(color) {}
    const std::string& color() const { return color_; }
    void add_child(Pet* p) { children_.push_back(p); }
    virtual void accept(PetVisitor& v, Pet* p = nullptr) = 0;
    private:
    friend class FamilyTreeVisitor;
    std::string color_;
    std::vector<Pet*> children_;
};

class Cat : public Pet {
    public:
    Cat(const std::string& color) : Pet(color) {}
    void accept(PetVisitor& v, Pet* p = nullptr) override { v.visit(this, p); }
};

class Dog : public Pet {
    public:
    Dog(const std::string& color) : Pet(color) {}
    void accept(PetVisitor& v, Pet* p = nullptr) override { v.visit(this, p); }
};

class FeedingVisitor : public PetVisitor {
    public:
    void visit(Cat* c, Pet*) override { std::cout << "Feed tuna to the " << c->color() << " cat" << std::endl; }
    void visit(Dog* d, Pet*) override { std::cout << "Feed steak to the " << d->color() << " dog" << std::endl; }
};

class PlayingVisitor : public PetVisitor {
    public:
    void visit(Cat* c, Pet*) override { std::cout << "Play with feather with the " << c->color() << " cat" << std::endl; }
    void visit(Dog* d, Pet*) override { std::cout << "Play fetch with the " << d->color() << " dog" << std::endl; }
};

class BirthVisitor : public PetVisitor {
    public:
    void visit(Cat* c, Pet* p) override { assert(dynamic_cast<Cat*>(p)); c->add_child(p); }
    void visit(Dog* d, Pet* p) override { assert(dynamic_cast<Dog*>(p)); d->add_child(p); }
};

class FamilyTreeVisitor : public PetVisitor {
    public:
    void visit(Cat* c, Pet*) override {
        std::cout << "Kittens: "; 
        for (auto k : c->children_) {
            std::cout << k->color() << " ";
        }
        std::cout << std::endl;
    }
    void visit(Dog* d, Pet*) override {
        std::cout << "Puppies: "; 
        for (auto p : d->children_) {
            std::cout << p->color() << " ";
        }
        std::cout << std::endl;
    }
};

void dispatch(Pet& p, PetVisitor& v, Pet* p1 = nullptr) { p.accept(v, p1); }

int main() {
    std::unique_ptr<Pet> c(new Cat("orange"));
    std::unique_ptr<Pet> d(new Dog("brown"));

    FeedingVisitor fv;
    dispatch(*c, fv);
    dispatch(*d, fv);

    PlayingVisitor pv;
    dispatch(*c, pv);
    dispatch(*d, pv);

    BirthVisitor bv;
    Cat c1("tabby"), c2("calico");
    Dog d1("black");
    dispatch(*c, bv, &c1);
    dispatch(*c, bv, &c2);
    dispatch(*d, bv, &d1);
    //dispatch(*d, bv, &c1); // Asserts

    FamilyTreeVisitor tv;
    c->accept(tv);
    dispatch(*c, tv);
    dispatch(*d, tv);
}
