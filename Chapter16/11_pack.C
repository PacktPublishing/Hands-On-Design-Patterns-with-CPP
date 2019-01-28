// Variadic template policies
#include <cstdlib>
#include <iostream>
using std::cout;
using std::endl;
using std::cin;

template <typename T, typename V> 
struct ComparableSelf
{
    friend bool operator==(V lhs, V rhs) { return lhs.get() == rhs.get(); }
    friend bool operator!=(V lhs, V rhs) { return lhs.get() != rhs.get(); }
};

template <typename T, typename V> 
struct ComparableValue
{
    friend bool operator==(V lhs, T rhs) { return lhs.get() == rhs; }
    friend bool operator==(T lhs, V rhs) { return lhs == rhs.get(); }
    friend bool operator!=(V lhs, T rhs) { return lhs.get() != rhs; }
    friend bool operator!=(T lhs, V rhs) { return lhs != rhs.get(); }
};

template <typename T, typename V>
struct Comparable : public ComparableSelf<T, V>, public ComparableValue<T, V>
{
};

template <typename T, typename V>
struct AddSub
{
    friend V operator+(V lhs, V rhs) { return V(lhs.get() + rhs.get()); }
    friend V operator+(V lhs, T rhs) { return V(lhs.get() + rhs); }
    friend V operator+(T lhs, V rhs) { return V(lhs + rhs.get()); }
    friend V operator-(V lhs, V rhs) { return V(lhs.get() - rhs.get()); }
    friend V operator-(V lhs, T rhs) { return V(lhs.get() - rhs); }
    friend V operator-(T lhs, V rhs) { return V(lhs - rhs.get()); }
};

template <typename T, typename V>
struct MulDiv
{
    friend V operator*(V lhs, V rhs) { return V(lhs.get() * rhs.get()); }
    friend V operator*(V lhs, T rhs) { return V(lhs.get() * rhs); }
    friend V operator*(T lhs, V rhs) { return V(lhs * rhs.get()); }
    friend V operator/(V lhs, V rhs) { return V(lhs.get() / rhs.get()); }
    friend V operator/(V lhs, T rhs) { return V(lhs.get() / rhs); }
    friend V operator/(T lhs, V rhs) { return V(lhs / rhs.get()); }
};

template <typename T, typename V>
struct Incrementable
{
    V operator++() {
        V& v = static_cast<V&>(*this);
        ++(v.get());
        return v;
    }
    V operator++(int) {
        V& v = static_cast<V&>(*this);
        return V(v.get()++);
    }
    V operator--() {
        V& v = static_cast<V&>(*this);
        --(v.get());
        return v;
    }
    V operator--(int) {
        V& v = static_cast<V&>(*this);
        return V(v.get()--);
    }
    V& operator+=(V val) {
        V& v = static_cast<V&>(*this);
        v.get() += val.get();
        return v;
    }
    V& operator+=(T val) {
        V& v = static_cast<V&>(*this);
        v.get() += val;
        return v;
    }
    V& operator-=(V val) {
        V& v = static_cast<V&>(*this);
        v.get() -= val.get();
        return v;
    }
    V& operator-=(T val) {
        V& v = static_cast<V&>(*this);
        v.get() -= val;
        return v;
    }
};

template <typename T, typename V>
struct ExplicitConvertible
{
    explicit operator T() { return static_cast<V*>(this)->get(); }
    explicit operator const T() const { return static_cast<const V*>(this)->get(); }
};

template <typename T, typename V>
struct ImplicitConvertible
{
    //typedef V::value_type value_type; // Does not work: Value is an incomplete type when its base class is instantiated
    operator T() { return static_cast<V*>(this)->get(); }
    operator const T() const { return static_cast<const V*>(this)->get(); }
};

template <typename T, template <typename, typename> class ... Policies> // "class" -> "typename" in C++17
class Value : public Policies<T, Value<T, Policies ... >> ...
{
    public:
    typedef T value_type;
    explicit Value() : val_(T()) {}
    explicit Value(T v) : val_(v) {}
    Value(const Value& rhs) : val_(rhs.val_) {}
    Value& operator=(Value rhs) { val_ = rhs.val_; return *this; }
    Value& operator=(T rhs) { val_ = rhs; return *this; }
    friend std::ostream& operator<<(std::ostream& out, Value x) { out << x.val_; return out; }
    friend std::istream& operator>>(std::istream& in, Value& x) { in >> x.val_; return in; }

    T get() const { return val_; }
    T& get() { return val_; }

    template <typename U> using rebind_type = Value<U, Policies...>;

    private:
    // ??? friend class Policies<T, Value<T, Policies...>>...;
    T val_;
};

#define ERROR12
int main() {
    if (1) {
        using V = Value<int>;
        V i;
        V j(5);
        V k(j);
        k = j;
        i = 3;
        cout << "i=" << i << endl;
        cin >> i;
        cout << "i=" << i << endl;
#ifdef ERROR1
        if (i == j) cout << "i==j" << endl;
#endif
#ifdef ERROR8
        int x = i;
#endif
#ifdef ERROR9
        int x(i);
#endif
    }

    if (1) {
        using V = Value<int, ComparableSelf>;
        V i;
        V j(5);
        V k(j);
        if (i == j) cout << "i==j" << endl;
        if (i != j) cout << "i!=j" << endl;
        if (k == j) cout << "k==j" << endl;
#ifdef ERROR2
        if (k == 5) cout << "k==5" << endl;
#endif
    }

    if (1) {
        using V = Value<int, ComparableValue>;
        V i;
        V j(5);
        V k(j);
        if (k == 5) cout << "k==5" << endl;
        if (5 == k) cout << "5==k" << endl;
#ifdef ERROR3
        if (i == j) cout << "i==j" << endl;
#endif
    }

    if (1) {
        using V = Value<int, Comparable>;
        V i;
        V j(5);
        V k(j);
        if (i == j) cout << "i==j" << endl;
        if (i != j) cout << "i!=j" << endl;
        if (k == j) cout << "k==j" << endl;
        if (k == 5) cout << "k==5" << endl;
        if (5 == k) cout << "5==k" << endl;
    }

    if (1) {
        using V = Value<int, AddSub>;
        V i(3);
        V j(5);
        V k(i - j);
        k = i + j;
        cout << "k=" << k << endl;
        cout << "i + j - k=" << i + j - k << endl;
#ifdef ERROR4
        if (k == 5) cout << "k==5" << endl;
#endif
    }

    if (1) {
        using V = Value<int, AddSub, Comparable>;
        V i(3);
        V j(5);
        V k(i - j);
        k = i + j;
        cout << "k=" << k << endl;
        cout << "i + j - k=" << i + j - k << endl;
        if (k == 5) cout << "k==5" << endl;
        if (i != j) cout << "i!=j" << endl;
        if (i - j != k) cout << "i - j != k" << endl;
        if (i - j != k + j) cout << "i - j != k + j" << endl;
    }

    if (1) {
        using V = Value<int, Comparable, AddSub>;
        V i(3);
        V j(5);
        V k(i - j);
        k = i + j;
        cout << "k=" << k << endl;
        cout << "i + j - k=" << i + j - k << endl;
        if (k == 5) cout << "k==5" << endl;
        if (i != j) cout << "i!=j" << endl;
        if (i - j != k) cout << "i - j != k" << endl;
        if (i - j != k + j) cout << "i - j != k + j" << endl;
    }

    if (1) {
        using V = Value<int, MulDiv>;
        V i(3);
        V j(5);
        V k(i / j);
        k = i * j;
        cout << "k=" << k << endl;
        cout << "i * j / k=" << i * j / k << endl;
#ifdef ERROR5
        if (k == 5) cout << "k==5" << endl;
        k = i + j;
#endif
    }

    if (1) {
        using V = Value<int, MulDiv, AddSub>;
        V i(3);
        V j(5);
        V k(i / j);
        k = i * j;
        cout << "k=" << k << endl;
        cout << "i * j / k=" << i * j / k << endl;
        k = i + j;
        cout << "k=" << k << endl;
        cout << "i + j - k=" << i + j - k << endl;
        cout << "(i + j) * (k - i)" << (i + j) * (k - i) << endl;
        cout << "(i * j) + (k / i)" << (i * j) + (k / i) << endl;
#ifdef ERROR6
        if (k == 5) cout << "k==5" << endl;
#endif
    }

    if (1) {
        using V = Value<int, AddSub, MulDiv>;
        V i(3);
        V j(5);
        V k(i / j);
        k = i * j;
        cout << "k=" << k << endl;
        cout << "i * j / k=" << i * j / k << endl;
        k = i + j;
        cout << "k=" << k << endl;
        cout << "i + j - k=" << i + j - k << endl;
        cout << "(i + j) * (k - i)" << (i + j) * (k - i) << endl;
        cout << "(i * j) + (k / i)" << (i * j) + (k / i) << endl;
#ifdef ERROR7
        if (k == 5) cout << "k==5" << endl;
#endif
    }

    if (1) {
        using V = Value<int, Comparable, AddSub, MulDiv>;
        V i(3);
        V j(5);
        V k(i / j);
        k = i * j;
        cout << "k=" << k << endl;
        cout << "i * j / k=" << i * j / k << endl;
        k = i + j;
        cout << "k=" << k << endl;
        cout << "i + j - k=" << i + j - k << endl;
        if (k == 5) cout << "k==5" << endl;
        if (i != j) cout << "i!=j" << endl;
        if (i - j != k) cout << "i - j != k" << endl;
        if (i - j != k * j) cout << "i - j != k * j" << endl;
    }

    if (1) {
        using V = Value<int, Incrementable>;
        V i(3);
        V j(5);
        V k(i++);
        cout << "++i=" << ++i << endl;
        cout << "i=" << i << endl;
        cout << "k=" << k << endl;
        j += i;
        cout << "j=" << j << endl;
        j += 3;
        cout << "j=" << j << endl;
        k -= 4;
        cout << "k=" << k << endl;
        k -= j;
        cout << "k=" << k << endl;
        j--;
        cout << "j=" << k << endl;
    }

    if (1) {
        using V = Value<int, ExplicitConvertible>;
        V i(3);
        int j(i);
        cout << "i=" << i << " j=" << j << endl;
#ifdef ERROR10
        int x = i;
#endif
    }

    if (1) {
        using V = Value<int, ImplicitConvertible>;
        V i(3);
        int j(i);
        int k = i;
        cout << "i=" << i << " j=" << j << " k=" << k << endl;
    }

    if (1) {
        using V0 = Value<int, Comparable, AddSub, MulDiv>;
        using V = V0::rebind_type<double>;
        V i(3);
        V j(5);
        V k(i / j);
        cout << "k=" << k << endl;
        k = i * j;
        cout << "k=" << k << endl;
        cout << "i * j / k=" << i * j / k << endl;
        k = i + j;
        cout << "k=" << k << endl;
        cout << "i + j - k=" << i + j - k << endl;
        if (k == 5) cout << "k==5" << endl;
        if (i != j) cout << "i!=j" << endl;
        if (i - j != k) cout << "i - j != k" << endl;
        if (i - j != k * j) cout << "i - j != k * j" << endl;
#ifdef ERROR11
        ++i;
#endif
    }

}

