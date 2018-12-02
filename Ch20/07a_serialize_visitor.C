// 07 with wrapper functions
#include <iostream>
#include <sstream>
#include <exception>
#include <string.h>

class Point {
    public:
    Point() = default;
    Point(double x, double y) : x_(x), y_(y) {}
    template <typename This, typename Visitor>
    static void accept(This& t, Visitor& v) {
        v(t.x_);
        v(t.y_);
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
        v(t.c_);
        v(t.r_);
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
        v(t.p1_);
        v(t.p2_);
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
        v(t.g1_);
        v(t.g2_);
    }
    private:
    G1 g1_;
    G2 g2_;
};

class StreamVisitor {
    public:
    StreamVisitor(std::ostream& out) : out_(out), indent_(0) {}
    void operator()(double x) { indent(); out_ << x << std::endl; }
    template <typename T> void operator()(const T& t) {
        indent_ += 2;
        T::accept(t, *this);
        indent_ -= 2;
    }

    private:
    void indent() { for (int i = 0; i < indent_; ++i) out_ << " "; }
    std::ostream& out_;
    int indent_;
};

template <typename T>
void write(const T& t, std::ostream& out = std::cout) {
    StreamVisitor v(out);
    v(t);
}

class StringSerializeVisitor {
    public:
    void operator()(double x) { S << x << " "; }
    template <typename T> void operator()(const T& t) { T::accept(t, *this); }

    std::string str() const { return S.str(); }
    private:
    std::stringstream S;
};

template <typename T>
void serialize(const T& t, std::string& s) {
    StringSerializeVisitor v;
    v(t);
    s += v.str();
}

class StringDeserializeVisitor {
    public:
    StringDeserializeVisitor(const std::string& s) { S.str(s); }
    void operator()(double& x) { S >> x; }
    template <typename T> void operator()(T& t) { T::accept(t, *this); }
    private:
    std::stringstream S;
};

template <typename V, typename T>
void visitation(V& v, T& t) {
    v(t);
}
template <typename V, typename T, typename ... U>
void visitation(V& v, T& t, U& ... u) {
    v(t);
    visitation(v, u ...);
}
template <typename ... U>
void deserialize(const std::string& s, U& ... u) {
    StringDeserializeVisitor v(s);
    visitation(v, u ...);
}

class BinarySerializeVisitor {
    public:
    BinarySerializeVisitor(char* buffer, size_t size) : buf_(buffer), size_(size) {
    }
    void operator()(double x) {
        if (size_ < sizeof(x)) 
            throw std::runtime_error("Buffer overflow");
        memcpy(buf_, &x, sizeof(x));
        buf_ += sizeof(x);
        size_ -= sizeof(x);
    }
    template <typename T> void operator()(const T& t) { T::accept(t, *this); }

    private:
    char* buf_;
    size_t size_;
};

template <typename ... U>
void serialize(char* buffer, size_t size, U& ... u) {
    BinarySerializeVisitor v(buffer, size);
    visitation(v, u ...);
}


class BinaryDeserializeVisitor {
    public:
    BinaryDeserializeVisitor(const char* buffer, size_t size) : buf_(buffer), size_(size) {}
    void operator()(double& x) {
        if (size_ < sizeof(x)) 
            throw std::runtime_error("Buffer overflow");
        memcpy(&x, buf_, sizeof(x));
        buf_ += sizeof(x);
        size_ -= sizeof(x);
    }
    template <typename T> void operator()(T& t) { T::accept(t, *this); }
    private:
    const char* buf_;
    size_t size_;
};

template <typename ... U>
void deserialize(char* buffer, size_t size, U& ... u) {
    BinaryDeserializeVisitor v(buffer, size);
    visitation(v, u ...);
}

int main() {
    Line l(Point(1, 2), Point(5, 2));
    Circle c(Point(1, 2), 3);
    Intersection<Circle, Circle> x(Circle(Point(1, 2), 3), Circle(Point(2, 1), 2));
    double y(11);

    std::cout << "Line l:\n"; write(l);
    std::cout << "Circle c:\n"; write(c);
    std::cout << "Intersection x:\n"; write(x);
    std::cout << "y:\n"; write(y);

    {
        std::string s;
        serialize(l, s);
        serialize(c, s);
        serialize(x, s);
        serialize(y, s);

        Line l1;
        Circle c1;
        Intersection<Circle, Circle> x1;
        double y1(0);
        deserialize(s, l1, c1, x1, y1);
        std::cout << "Line l1:\n"; write(l1);
        std::cout << "Circle c1:\n"; write(c1);
        std::cout << "Intersection x1:\n"; write(x1);
        std::cout << "y1:\n"; write(y1);
    }

    {
        char buffer[1024];
        serialize(buffer, sizeof(buffer), l, c, x, y);

        Line l2;
        Circle c2;
        Intersection<Circle, Circle> x2;
        double y2(0);
        deserialize(buffer, sizeof(buffer), l2, c2, x2, y2);
        std::cout << "Line l2:\n"; write(l2);
        std::cout << "Circle c2:\n"; write(c2);
        std::cout << "Intersection x2:\n"; write(x2);
        std::cout << "y2:\n"; write(y2);
    }
}
