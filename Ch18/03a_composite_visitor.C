// Visitable composite (based on 01a)
#include <iostream>
#include <memory>

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

class Family {
    public:
    Family(const char* cat_color, const char* dog_color) :
        cat_(cat_color), dog_(dog_color)
    {}
    void accept(PetVisitor& v) {
        cat_.accept(v);
        dog_.accept(v);
    }
    private:
    Cat cat_;
    Dog dog_;
};

int main() {
    Family f("orange", "brown");

    FeedingVisitor fv;
    f.accept(fv);

    PlayingVisitor pv;
    f.accept(pv);
}
