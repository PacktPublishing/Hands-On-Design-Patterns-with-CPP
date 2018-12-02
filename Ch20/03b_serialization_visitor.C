// Visitable composite (based on 01a)
#include <iostream>
#include <sstream>
#include <exception>
#include <memory>
#include <string.h>

class Point;
class Circle;
class Line;

class Visitor {
    public:
    virtual void visit(double& x) = 0;
    virtual void visit(Point& p) = 0;
    virtual void visit(Circle& c) = 0;
    virtual void visit(Line& l) = 0;
};

class Geometry {
    public:
    virtual ~Geometry() {}
    virtual void accept(Visitor& v) = 0;
};

class Point : public Geometry {
    public:
    Point() = default;
    Point(double x, double y) : x_(x), y_(y) {}
    void accept(Visitor& v) override {
        v.visit(x_);
        v.visit(y_);
    }
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
    private:
    Point p1_;
    Point p2_;
};

class StreamVisitor : public Visitor {
    public:
    StreamVisitor(std::ostream& out) : out_(out), indent_(0) {}
    void visit(double& x) override { indent(); out_ << x << std::endl; }
    void visit(Point& p) override { visit_impl(p); }
    void visit(Circle& c) override { visit_impl(c); }
    void visit(Line& l) override { visit_impl(l); }

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
    void visit(double& x) override { S << x << " "; }
    void visit(Point& p) override { p.accept(*this); }
    void visit(Circle& c) override { c.accept(*this); }
    void visit(Line& l) override { l.accept(*this); }
    std::string str() const { return S.str(); }

    private:
    std::stringstream S;
};

class StringDeserializeVisitor : public Visitor {
    public:
    StringDeserializeVisitor(const std::string& s) { S.str(s); }
    void visit(double& x) override { S >> x; }
    void visit(Point& p) override { p.accept(*this); }
    void visit(Circle& c) override { c.accept(*this); }
    void visit(Line& l) override { l.accept(*this); }

    private:
    std::stringstream S;
};

class BinarySerializeVisitor : public Visitor {
    public:
    BinarySerializeVisitor(char* buffer, size_t size) : buf_(buffer), size_(size) {
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

    private:
    char* buf_;
    size_t size_;
};

class BinaryDeserializeVisitor : public Visitor {
    public:
    BinaryDeserializeVisitor(const char* buffer, size_t size) : buf_(buffer), size_(size) {}
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

    private:
    const char* buf_;
    size_t size_;
};

int main() {
    Line l(Point(1, 2), Point(5, 2));
    Circle c(Point(1, 2), 3);

    StreamVisitor writer(std::cout);
    std::cout << "Line l:\n"; writer.visit(l);
    std::cout << "Circle c:\n"; writer.visit(c);

    {
        StringSerializeVisitor serializer;
        serializer.visit(l);
        serializer.visit(c);
        std::string s(serializer.str());

        {
            Line l1;
            Circle c1;
            StringDeserializeVisitor deserializer(s);
            deserializer.visit(l1);
            deserializer.visit(c1);
            std::cout << "Line l1:\n"; writer.visit(l1);
            std::cout << "Circle c1:\n"; writer.visit(c1);
        }
    }

    {
        char buffer[1024];
        BinarySerializeVisitor serializer(buffer, sizeof(buffer));
        serializer.visit(l);
        serializer.visit(c);

        {
            Line l2;
            Circle c2;
            BinaryDeserializeVisitor deserializer(buffer, sizeof(buffer));
            deserializer.visit(l2);
            deserializer.visit(c2);
            std::cout << "Line l2:\n"; writer.visit(l2);
            std::cout << "Circle c2:\n"; writer.visit(c2);
        }
    }
}
