// Visitable container (based on 01a)
#include <iostream>
#include <memory>
#include <vector>

class Cat;
class Dog;

class PetVisitor {
    public:
    virtual void visit(Cat* c) = 0;
    virtual void visit(Dog* d) = 0;
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

class Cat : public Pet {
    public:
    Cat(const std::string& color) : Pet(color) {}
    void accept(PetVisitor& v) override { v.visit(this); }
};

class Dog : public Pet {
    public:
    Dog(const std::string& color) : Pet(color) {}
    void accept(PetVisitor& v) override { v.visit(this); }
};

class FeedingVisitor : public PetVisitor {
    public:
    void visit(Cat* c) override { std::cout << "Feed tuna to the " << c->color() << " cat" << std::endl; }
    void visit(Dog* d) override { std::cout << "Feed steak to the " << d->color() << " dog" << std::endl; }
};

class PlayingVisitor : public PetVisitor {
    public:
    void visit(Cat* c) override { std::cout << "Play with feather with the " << c->color() << " cat" << std::endl; }
    void visit(Dog* d) override { std::cout << "Play fetch with the " << d->color() << " dog" << std::endl; }
};

class Shelter {
    public:
    void add(Pet* p) {
        pets_.emplace_back(p);
    }
    void accept(PetVisitor& v) {
        for (auto& p : pets_) {
            p->accept(v);
        }
    }
    private:
    std::vector<std::unique_ptr<Pet>> pets_;
};

int main() {
    Shelter s;
    s.add(new Cat("orange"));
    s.add(new Dog("brown"));

    FeedingVisitor fv;
    s.accept(fv);

    PlayingVisitor pv;
    s.accept(pv);
}
