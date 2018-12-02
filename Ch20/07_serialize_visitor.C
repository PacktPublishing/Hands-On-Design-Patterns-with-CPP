// Serialization visitor (compile-time)
#include <iostream>
#include <sstream>
#include <exception>
#include <string.h>

class Point {
    public:
    Point() = default;
    Point(double x, double y) : x_(x), y_(y) {}
    // "This" can be Point& or const Point&
    template <typename This, typename Visitor>
    static void accept(This& t, Visitor& v) {
        v.visit(t.x_);
        v.visit(t.y_);
    }
    private:
    double x_;
    double y_;
};

class Circle {
    public:
    Circle() = default;
    Circle(Point c, double r) : c_(c), r_(r) {}
    template <typename This, typename Visitor>
    static void accept(This& t, Visitor& v) {
        v.visit(t.c_);
        v.visit(t.r_);
    }
    private:
    Point c_;
    double r_;
};

class Line {
    public:
    Line() = default;
    Line(Point p1, Point p2) : p1_(p1), p2_(p2) {}
    template <typename This, typename Visitor>
    static void accept(This& t, Visitor& v) {
        v.visit(t.p1_);
        v.visit(t.p2_);
    }
    private:
    Point p1_;
    Point p2_;
};

template <typename G1, typename G2>
class Intersection {
    public:
    Intersection() = default;
    Intersection(G1 g1, G2 g2) : g1_(g1), g2_(g2) {}
    template <typename This, typename Visitor>
    static void accept(This& t, Visitor& v) {
        v.visit(t.g1_);
        v.visit(t.g2_);
    }
    private:
    G1 g1_;
    G2 g2_;
};

class StreamVisitor {
    public:
    StreamVisitor(std::ostream& out) : out_(out), indent_(0) {}
    void visit(double x) { indent(); out_ << x << std::endl; }
    template <typename T> void visit(const T& t) {
        indent_ += 2;
        T::accept(t, *this);
        indent_ -= 2;
    }

    private:
    void indent() { for (int i = 0; i < indent_; ++i) out_ << " "; }
    std::ostream& out_;
    int indent_;
};

class StringSerializeVisitor {
    public:
    void visit(double x) { S << x << " "; }
    template <typename T> void visit(const T& t) { T::accept(t, *this); }

    std::string str() const { return S.str(); }
    private:
    std::stringstream S;
};

class StringDeserializeVisitor {
    public:
    StringDeserializeVisitor(const std::string& s) { S.str(s); }
    void visit(double& x) { S >> x; }
    template <typename T> void visit(T& t) { T::accept(t, *this); }
    private:
    std::stringstream S;
};

class BinarySerializeVisitor {
    public:
    BinarySerializeVisitor(char* buffer, size_t size) : buf_(buffer), size_(size) {
    }
    void visit(double x) {
        if (size_ < sizeof(x)) 
            throw std::runtime_error("Buffer overflow");
        memcpy(buf_, &x, sizeof(x));
        buf_ += sizeof(x);
        size_ -= sizeof(x);
    }
    template <typename T> void visit(const T& t) { T::accept(t, *this); }

    private:
    char* buf_;
    size_t size_;
};

class BinaryDeserializeVisitor {
    public:
    BinaryDeserializeVisitor(const char* buffer, size_t size) : buf_(buffer), size_(size) {}
    void visit(double& x) {
        if (size_ < sizeof(x)) 
            throw std::runtime_error("Buffer overflow");
        memcpy(&x, buf_, sizeof(x));
        buf_ += sizeof(x);
        size_ -= sizeof(x);
    }
    template <typename T> void visit(T& t) { T::accept(t, *this); }
    private:
    const char* buf_;
    size_t size_;
};

int main() {
    Line l(Point(1, 2), Point(5, 2));
    Circle c(Point(1, 2), 3);
    Intersection<Circle, Circle> x(Circle(Point(1, 2), 3), Circle(Point(2, 1), 2));

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
            StringDeserializeVisitor deserializer(s);
            Intersection<Circle, Circle> x1;
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
            Intersection<Circle, Circle> x2;
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
