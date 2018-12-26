// 03b with a polymorphic composite
#include <iostream>
#include <sstream>
#include <exception>
#include <memory>
#include <string.h>

class Point;
class Circle;
class Line;
class Intersection;

class Visitor;
class Geometry {
    public:
    virtual ~Geometry() {}
    virtual void accept(Visitor& v) = 0;
    enum type_tag { POINT = 100, CIRCLE, LINE, INTERSECTION };
    virtual type_tag tag() const = 0;
};

class Visitor {
    public:
    static Geometry* make_geometry(Geometry::type_tag tag);
    virtual void visit(Geometry::type_tag& tag) = 0;
    virtual void visit(double& x) = 0;
    virtual void visit(Point& p) = 0;
    virtual void visit(Circle& c) = 0;
    virtual void visit(Line& l) = 0;
    virtual void visit(Intersection& l) = 0;
};

class Point : public Geometry {
    public:
    Point() = default;
    Point(double x, double y) : x_(x), y_(y) {}
    void accept(Visitor& v) override {
        v.visit(x_);
        v.visit(y_);
    }
    type_tag tag() const override { return POINT; }
    private:
    double x_;
    double y_;
};

class Circle : public Geometry {
    public:
    Circle() = default;
    Circle(Point c, double r) : c_(c), r_(r) {}
    void accept(Visitor& v) override {
        v.visit(c_);
        v.visit(r_);
    }
    type_tag tag() const override { return CIRCLE; }
    private:
    Point c_;
    double r_;
};

class Line : public Geometry {
    public:
    Line() = default;
    Line(Point p1, Point p2) : p1_(p1), p2_(p2) {}
    void accept(Visitor& v) override {
        v.visit(p1_);
        v.visit(p2_);
    }
    type_tag tag() const override { return LINE; }
    private:
    Point p1_;
    Point p2_;
};

class Intersection : public Geometry {
    public:
    Intersection() = default;
    Intersection(Geometry* g1, Geometry* g2) : g1_(g1), g2_(g2) {}
    void accept(Visitor& v) override {
        Geometry::type_tag tag;
        if (g1_) tag = g1_->tag();
        v.visit(tag);
        if (!g1_) g1_.reset(Visitor::make_geometry(tag));
        g1_->accept(v);
        if (g2_) tag = g2_->tag();
        v.visit(tag);
        if (!g2_) g2_.reset(Visitor::make_geometry(tag));
        g2_->accept(v);
    }
    type_tag tag() const override { return INTERSECTION; }
    private:
    std::unique_ptr<Geometry> g1_;
    std::unique_ptr<Geometry> g2_;
};

Geometry* Visitor::make_geometry(Geometry::type_tag tag) {
    switch (tag) {
        case Geometry::POINT: return new Point;
        case Geometry::CIRCLE: return new Circle;
        case Geometry::LINE: return new Line;
        case Geometry::INTERSECTION: return new Intersection;
    }
    return NULL; // Never happens, suppress compiler warning
}

class StreamVisitor : public Visitor {
    public:
    StreamVisitor(std::ostream& out) : out_(out), indent_(0) {}
    void visit(Geometry::type_tag& tag) override {}
    void visit(double& x) override { indent(); out_ << x << std::endl; }
    void visit(Point& p) override { visit_impl(p); }
    void visit(Circle& c) override { visit_impl(c); }
    void visit(Line& l) override { visit_impl(l); }
    void visit(Intersection& x) override { visit_impl(x); }

    private:
    void indent() { for (int i = 0; i < indent_; ++i) out_ << " "; }
    template <typename T> void visit_impl(T& t) {
        indent_ += 2;
        t.accept(*this);
        indent_ -= 2;
    }
    std::ostream& out_;
    int indent_;
};

class StringSerializeVisitor : public Visitor {
    public:
    void visit(Geometry::type_tag& tag) override { S << size_t(tag) << " "; }
    void visit(double& x) override { S << x << " "; }
    void visit(Point& p) override { p.accept(*this); }
    void visit(Circle& c) override { c.accept(*this); }
    void visit(Line& l) override { l.accept(*this); }
    void visit(Intersection& x) override { x.accept(*this); }
    std::string str() const { return S.str(); }

    private:
    std::stringstream S;
};

class StringDeserializeVisitor : public Visitor {
    public:
    StringDeserializeVisitor(const std::string& s) { S.str(s); }
    void visit(Geometry::type_tag& tag) override {
        size_t t;
        S >> t;
        tag = Geometry::type_tag(t);
    }
    void visit(double& x) override { S >> x; }
    void visit(Point& p) override { p.accept(*this); }
    void visit(Circle& c) override { c.accept(*this); }
    void visit(Line& l) override { l.accept(*this); }
    void visit(Intersection& x) override { x.accept(*this); }

    private:
    std::stringstream S;
};

class BinarySerializeVisitor : public Visitor {
    public:
    BinarySerializeVisitor(char* buffer, size_t size) : buf_(buffer), size_(size) {}
    void visit(Geometry::type_tag& tag) override {
        size_t t = tag;
        if (size_ < sizeof(t)) 
            throw std::runtime_error("Buffer overflow");
        memcpy(buf_, &t, sizeof(t));
        buf_ += sizeof(t);
        size_ -= sizeof(t);
    }
    void visit(double& x) override {
        if (size_ < sizeof(x)) 
            throw std::runtime_error("Buffer overflow");
        memcpy(buf_, &x, sizeof(x));
        buf_ += sizeof(x);
        size_ -= sizeof(x);
    }
    void visit(Point& p) override { p.accept(*this); }
    void visit(Circle& c) override { c.accept(*this); }
    void visit(Line& l) override { l.accept(*this); }
    void visit(Intersection& x) override { x.accept(*this); }

    private:
    char* buf_;
    size_t size_;
};

class BinaryDeserializeVisitor : public Visitor {
    public:
    BinaryDeserializeVisitor(const char* buffer, size_t size) : buf_(buffer), size_(size) {}
    void visit(Geometry::type_tag& tag) override {
        size_t t;
        if (size_ < sizeof(t)) 
            throw std::runtime_error("Buffer overflow");
        memcpy(&t, buf_, sizeof(t));
        buf_ += sizeof(t);
        size_ -= sizeof(t);
        tag = Geometry::type_tag(t);
    }
    void visit(double& x) override {
        if (size_ < sizeof(x)) 
            throw std::runtime_error("Buffer overflow");
        memcpy(&x, buf_, sizeof(x));
        buf_ += sizeof(x);
        size_ -= sizeof(x);
    }
    void visit(Point& p) override { p.accept(*this); }
    void visit(Circle& c) override { c.accept(*this); }
    void visit(Line& l) override { l.accept(*this); }
    void visit(Intersection& x) override { x.accept(*this); }

    private:
    const char* buf_;
    size_t size_;
};

int main() {
    Line l(Point(1, 2), Point(5, 2));
    Circle c(Point(1, 2), 3);
    Intersection x(new Circle(Point(1, 2), 3), new Circle(Point(2, 1), 2));

    StreamVisitor writer(std::cout);
    std::cout << "Line l:\n"; writer.visit(l);
    std::cout << "Circle c:\n"; writer.visit(c);
    std::cout << "Intersection x:\n"; writer.visit(x);

    {
        StringSerializeVisitor serializer;
        serializer.visit(l);
        serializer.visit(c);
        serializer.visit(x);
        std::string s(serializer.str());

        {
            Line l1;
            Circle c1;
            Intersection x1;
            StringDeserializeVisitor deserializer(s);
            deserializer.visit(l1);
            deserializer.visit(c1);
            deserializer.visit(x1);
            std::cout << "Line l1:\n"; writer.visit(l1);
            std::cout << "Circle c1:\n"; writer.visit(c1);
            std::cout << "Intersection x1:\n"; writer.visit(x1);
        }
    }

    {
        char buffer[1024];
        BinarySerializeVisitor serializer(buffer, sizeof(buffer));
        serializer.visit(l);
        serializer.visit(c);
        serializer.visit(x);

        {
            Line l2;
            Circle c2;
            Intersection x2;
            BinaryDeserializeVisitor deserializer(buffer, sizeof(buffer));
            deserializer.visit(l2);
            deserializer.visit(c2);
            deserializer.visit(x2);
            std::cout << "Line l2:\n"; writer.visit(l2);
            std::cout << "Circle c2:\n"; writer.visit(c2);
            std::cout << "Intersection x2:\n"; writer.visit(x2);
        }
    }
}
