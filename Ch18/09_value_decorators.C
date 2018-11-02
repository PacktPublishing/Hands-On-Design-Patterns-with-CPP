// Value type decorators
#include <iostream>
using std::cout;
using std::endl;

template <typename T> class Value {
    public:
    typedef T basic_type;
    typedef Value value_type;
    explicit Value() : val_(T()) {}
    explicit Value(T v) : val_(v) {}
    Value(const Value& rhs) : val_(rhs.val_) {}
    Value& operator=(Value rhs) { val_ = rhs.val_; return *this; }
    Value& operator=(basic_type rhs) { val_ = rhs; return *this; }
    friend std::ostream& operator<<(std::ostream& out, Value x) { out << x.val_; return out; }
    friend std::istream& operator>>(std::istream& in, Value& x) { in >> x.val_; return in; }

    protected:
    T val_;
};

template <typename V> class Comparable : public V {
    public:
    using V::V;
    typedef typename V::value_type value_type;
    typedef typename value_type::basic_type basic_type;
    Comparable(value_type v) : V(v) {}
    friend bool operator==(Comparable lhs, Comparable rhs) { return lhs.val_ == rhs.val_; }
    friend bool operator!=(Comparable lhs, Comparable rhs) { return lhs.val_ != rhs.val_; }
    friend bool operator==(Comparable lhs, basic_type rhs) { return lhs.val_ == rhs; }
    friend bool operator==(basic_type lhs, Comparable rhs) { return lhs == rhs.val_; }
    friend bool operator!=(Comparable lhs, basic_type rhs) { return lhs.val_ != rhs; }
    friend bool operator!=(basic_type lhs, Comparable rhs) { return lhs != rhs.val_; }
};

template <typename V> class Ordered : public V {
    public:
    using V::V;
    typedef typename V::value_type value_type;
    typedef typename value_type::basic_type basic_type;
    Ordered(value_type v) : V(v) {}
    friend bool operator<(Ordered lhs, Ordered rhs) { return lhs.val_ < rhs.val_; }
    friend bool operator<(basic_type lhs, Ordered rhs) { return lhs < rhs.val_; }
    friend bool operator<(Ordered lhs, basic_type rhs) { return lhs.val_ < rhs; }
    friend bool operator<=(Ordered lhs, Ordered rhs) { return lhs.val_ <= rhs.val_; }
    friend bool operator<=(basic_type lhs, Ordered rhs) { return lhs <= rhs.val_; }
    friend bool operator<=(Ordered lhs, basic_type rhs) { return lhs.val_ <= rhs; }
    friend bool operator>(Ordered lhs, Ordered rhs) { return lhs.val_ > rhs.val_; }
    friend bool operator>(basic_type lhs, Ordered rhs) { return lhs > rhs.val_; }
    friend bool operator>(Ordered lhs, basic_type rhs) { return lhs.val_ > rhs; }
    friend bool operator>=(Ordered lhs, Ordered rhs) { return lhs.val_ >= rhs.val_; }
    friend bool operator>=(basic_type lhs, Ordered rhs) { return lhs >= rhs.val_; }
    friend bool operator>=(Ordered lhs, basic_type rhs) { return lhs.val_ >= rhs; }
};

template <typename V> class Addable : public V {
    public:
    using V::V;
    typedef typename V::value_type value_type;
    typedef typename value_type::basic_type basic_type;
    Addable(value_type v) : V(v) {}
    friend Addable operator+(Addable lhs, Addable rhs) { return Addable(lhs.val_ + rhs.val_); }
    friend Addable operator+(Addable lhs, basic_type rhs) { return Addable(lhs.val_ + rhs); }
    friend Addable operator+(basic_type lhs, Addable rhs) { return Addable(lhs + rhs.val_); }
    friend Addable operator-(Addable lhs, Addable rhs) { return Addable(lhs.val_ - rhs.val_); }
    friend Addable operator-(Addable lhs, basic_type rhs) { return Addable(lhs.val_ - rhs); }
    friend Addable operator-(basic_type lhs, Addable rhs) { return Addable(lhs - rhs.val_); }
};

template <typename V> class Multipliable : public V {
    public:
    using V::V;
    typedef typename V::value_type value_type;
    typedef typename value_type::basic_type basic_type;
    Multipliable(value_type v) : V(v) {}
    friend Multipliable operator*(Multipliable lhs, Multipliable rhs) { return Multipliable(lhs.val_ * rhs.val_); }
    friend Multipliable operator*(Multipliable lhs, basic_type rhs) { return Multipliable(lhs.val_ * rhs); }
    friend Multipliable operator*(basic_type lhs, Multipliable rhs) { return Multipliable(lhs * rhs.val_); }
    friend Multipliable operator/(Multipliable lhs, Multipliable rhs) { return Multipliable(lhs.val_ / rhs.val_); }
    friend Multipliable operator/(Multipliable lhs, basic_type rhs) { return Multipliable(lhs.val_ / rhs); }
    friend Multipliable operator/(basic_type lhs, Multipliable rhs) { return Multipliable(lhs / rhs.val_); }
};

int main() {
    {
        using V = Value<int>;
        V i, j(5), k(3);
        //i == j; // Not Comparable
    }
    {
        using V = Comparable<Value<int>>;
        V i, j(5), k(3);
        cout << "i == j: " << (i == j) << ", i != j: " << (i != j) << endl;
        cout << "k == 3: " << (k == 3) << ", 6 != k: " << (6 != k) << endl;
        //i < j; // Not Ordered
    }
    {
        using V = Ordered<Comparable<Value<int>>>;
        V i, j(5), k(3);
        cout << "i == j: " << (i == j) << ", i != j: " << (i != j) << endl;
        cout << "k == 3: " << (k == 3) << ", 6 != k: " << (6 != k) << endl;
        cout << "i > j: " << (i > j) << ", i < j: " << (i < j) << endl;
        cout << "k >= 3: " << (k >= 3) << ", 6 > k: " << (6 > k) << endl;
    }
    {
        using V = Comparable<Ordered<Value<int>>>;
        V i, j(5), k(3);
        cout << "i == j: " << (i == j) << ", i != j: " << (i != j) << endl;
        cout << "k == 3: " << (k == 3) << ", 6 != k: " << (6 != k) << endl;
        cout << "i > j: " << (i > j) << ", i < j: " << (i < j) << endl;
        cout << "k >= 3: " << (k >= 3) << ", 6 > k: " << (6 > k) << endl;
        //i + j; // Not Addable
    }
    {
        using V = Addable<Comparable<Ordered<Value<int>>>>;
        V i, j(5), k(3);
        cout << "i == j: " << (i == j) << ", i != j: " << (i != j) << endl;
        cout << "k == 3: " << (k == 3) << ", 6 != k: " << (6 != k) << endl;
        cout << "i > j: " << (i > j) << ", i < j: " << (i < j) << endl;
        cout << "k >= 3: " << (k >= 3) << ", 6 > k: " << (6 > k) << endl;
        cout << "i + j: " << (i + j) << ", i - k: " << (i - k) << endl;
        cout << "i + 3: " << (i + 3) << ", 7 - k: " << (7 - k) << endl;
        cout << "i + j > k: " << ((i + j) > k) << ", i - j != k: " << ((i - j) != k) << endl;
    }
    {
        using V = Comparable<Addable<Ordered<Value<int>>>>;
        V i, j(5), k(3);
        cout << "i == j: " << (i == j) << ", i != j: " << (i != j) << endl;
        cout << "k == 3: " << (k == 3) << ", 6 != k: " << (6 != k) << endl;
        cout << "i > j: " << (i > j) << ", i < j: " << (i < j) << endl;
        cout << "k >= 3: " << (k >= 3) << ", 6 > k: " << (6 > k) << endl;
        cout << "i + j: " << (i + j) << ", i - k: " << (i - k) << endl;
        cout << "i + 3: " << (i + 3) << ", 7 - k: " << (7 - k) << endl;
        cout << "i + j > k: " << ((i + j) > k) << ", i - j != k: " << ((i - j) != k) << endl;
    }
    {
        using V = Comparable<Ordered<Addable<Value<int>>>>;
        V i, j(5), k(3);
        cout << "i == j: " << (i == j) << ", i != j: " << (i != j) << endl;
        cout << "k == 3: " << (k == 3) << ", 6 != k: " << (6 != k) << endl;
        cout << "i > j: " << (i > j) << ", i < j: " << (i < j) << endl;
        cout << "k >= 3: " << (k >= 3) << ", 6 > k: " << (6 > k) << endl;
        cout << "i + j: " << (i + j) << ", i - k: " << (i - k) << endl;
        cout << "i + 3: " << (i + 3) << ", 7 - k: " << (7 - k) << endl;
        cout << "i + j > k: " << ((i + j) > k) << ", i - j != k: " << ((i - j) != k) << endl;
        //i*j; // Not Multipliable
    }
    {
        using V = Multipliable<Addable<Value<int>>>;
        V i, j(5), k(3);
        cout << "i + j: " << (i + j) << ", i - k: " << (i - k) << endl;
        cout << "i + 3: " << (i + 3) << ", 7 - k: " << (7 - k) << endl;
        cout << "i * j: " << (i * j) << ", i / k: " << (i / k) << endl;
        cout << "i * 3: " << (i * 3) << ", 7 / k: " << (7 / k) << endl;
        cout << "(i * j) + (k / 3): " << ((i * j) + (k / 3)) << endl;
        //cout << "(i + j) * (k + 3): " << ((i + j) * (k + 3)) << endl; // Order problem, i + j is Addable not Multipliable!
    }
    {
        using V = Addable<Multipliable<Value<int>>>;
        V i, j(5), k(3);
        cout << "i + j: " << (i + j) << ", i - k: " << (i - k) << endl;
        cout << "i + 3: " << (i + 3) << ", 7 - k: " << (7 - k) << endl;
        cout << "i * j: " << (i * j) << ", i / k: " << (i / k) << endl;
        cout << "i * 3: " << (i * 3) << ", 7 / k: " << (7 / k) << endl;
        //cout << "(i * j) + (k / 3): " << ((i * j) + (k / 3)) << endl; // Order problem, i + j is Multipliable not Addable!
        cout << "(i + j) * (k + 3): " << ((i + j) * (k + 3)) << endl;
    }
}
