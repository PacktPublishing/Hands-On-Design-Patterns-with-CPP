// Compile-time visitor
#include <iostream>

class Pet {
    std::string color_;
    public:
    Pet(const std::string& color) : color_(color) {}
    const std::string& color() const { return color_; }
    template <typename Visitable, typename Visitor>
        static void accept(Visitable& p, Visitor& v) { v.visit(p); }
};

class Cat : public Pet {
    public:
    using Pet::Pet;
};

class Dog : public Pet {
    public:
    using Pet::Pet;
};

class FeedingVisitor {
    public:
    void visit(Cat& c) { std::cout << "Feed tuna to the " << c.color() << " cat" << std::endl; }
    void visit(Dog& d) { std::cout << "Feed steak to the " << d.color() << " dog" << std::endl; }
};

class PlayingVisitor {
    public:
    void visit(Cat& c) { std::cout << "Play with feather with the " << c.color() << " cat" << std::endl; }
    void visit(Dog& d) { std::cout << "Play fetch with the " << d.color() << " dog" << std::endl; }
};

int main() {
    Cat c("orange");
    Dog d("brown");

    FeedingVisitor fv;
    Pet::accept(c, fv);
    Pet::accept(d, fv);

    PlayingVisitor pv;
    Pet::accept(c, pv);
    Pet::accept(d, pv);
}
