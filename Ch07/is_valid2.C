#include <cstdlib>
#include <type_traits>

// Default constructor and destructor.
struct A {
};

// Default constructor and destructor, assignment operator, addition and subtraction.
struct B {
    int i;
    B& operator=(int j) { i = j; return *this; }
};
B operator+(const B& x, const B& y) { B t = { x.i + y.i }; return t; }
B operator+(const B& x, int y) { B t = { x.i + y }; return t; }
B operator+(int x, const B& y) { B t = { x + y.i }; return t; }
B operator-(const B& x, const B& y) { B t = { x.i - y.i }; return t; }
B operator-(const B& x, int y) { B t = { x.i - y }; return t; }
B operator-(int x, const B& y) { B t = { x - y.i }; return t; }

// No default constructor, destructor, cast to integer.
struct C {
    int i;
    C(const A&) {}
    operator int() const { return i; }
};

// Default constructor, no destructor.
class D {
    public:
    int i;
    D() : i(0) {}
    private:
    ~D() {}
};

// Not copyable.
class E {
    public:
    int i;
    E() : i(0) {}
    private:
    E(const E&) = delete;
    E& operator=(const E&) = delete;
};

// Smart pointer.
class Ptr {
    public:
    explicit Ptr(int* p = NULL) : p_(p) {}
    ~Ptr() { delete p_; }
    Ptr& operator=(int* p) { delete p_; p_ = p; return *this; }
    int* operator->() { return p_; }
    const int* operator->() const { return p_; }
    int& operator*() { return *p_; }
    const int& operator*() const { return *p_; }

    private:
    int* p_;
};

template <typename T> class SmartPtr {
    public:
    explicit SmartPtr(T* p = NULL) : p_(p) {}
    ~SmartPtr() { if (p_) delete p_; }
    T* operator->() { return p_; }
    const T* operator->() const { return p_; }
    T& operator*() { return *p_; }
    const T& operator*() const { return *p_; }

    private:
    T* p_;
};

// ------------------
template <typename Lambda> struct is_valid_helper {
    template <typename... LambdaArgs> constexpr auto test(int) -> decltype(std::declval<Lambda>()(std::declval<LambdaArgs>()...), std::true_type()) {
        return std::true_type();
    }

    template <typename... LambdaArgs> constexpr std::false_type test(...) {
        return std::false_type();
    }

    template <typename... LambdaArgs> constexpr auto operator()(const LambdaArgs& ...) {
        return this->test<LambdaArgs...>(0);
    }
};

template <typename Lambda> constexpr auto is_valid(const Lambda&) 
{
    return is_valid_helper<Lambda>();
}

auto is_default_constructible = is_valid([](auto&& x) -> decltype(new typename std::remove_reference<decltype(x)>::type) {});
auto is_copyable = is_valid([](auto&& x) -> decltype(new typename std::remove_reference<decltype(x)>::type(x)) {});
auto is_destructible = is_valid([](auto&& x) -> decltype(delete (&x)) {});
auto is_assignable = is_valid([](auto&& x) -> decltype(x = x) {});
auto is_assignable0 = is_valid([](auto&& x) -> decltype(x = 0) {});
auto is_addable = is_valid([](auto&& x) -> decltype(x + x) {});
auto is_addable2 = is_valid([](auto&& x, auto&&y) -> decltype(x + y) {});
auto is_pointer = is_valid([](auto&& x) -> decltype(*x) {});

void check_default_constructible( int i, double x, int* p, const A& a, const B& b, const C& c, const D& d, const E& e, const Ptr& ptr, const SmartPtr<int>& iptr ) {
    static_assert(decltype(is_default_constructible(i))::value, "Feature check failed");
    static_assert(decltype(is_default_constructible(x))::value, "Feature check failed");
    static_assert(decltype(is_default_constructible(p))::value, "Feature check failed");
    static_assert(decltype(is_default_constructible(a))::value, "Feature check failed");
    static_assert(decltype(is_default_constructible(b))::value, "Feature check failed");
    static_assert(!decltype(is_default_constructible(c))::value, "Feature check failed");
    static_assert(decltype(is_default_constructible(d))::value, "Feature check failed");
    static_assert(decltype(is_default_constructible(e))::value, "Feature check failed");
    static_assert(decltype(is_default_constructible(ptr))::value, "Feature check failed");
    static_assert(decltype(is_default_constructible(iptr))::value, "Feature check failed");
}

void check_copyable( int i, double x, int* p, const A& a, const B& b, const C& c, const D& d, const E& e, const Ptr& ptr, const SmartPtr<int>& iptr ) {
    static_assert(decltype(is_copyable(i))::value, "Feature check failed");
    static_assert(decltype(is_copyable(x))::value, "Feature check failed");
    static_assert(decltype(is_copyable(p))::value, "Feature check failed");
    static_assert(decltype(is_copyable(a))::value, "Feature check failed");
    static_assert(decltype(is_copyable(b))::value, "Feature check failed");
    static_assert(decltype(is_copyable(c))::value, "Feature check failed");
    static_assert(decltype(is_copyable(d))::value, "Feature check failed");
    static_assert(!decltype(is_copyable(e))::value, "Feature check failed");
    static_assert(decltype(is_copyable(ptr))::value, "Feature check failed");
    static_assert(decltype(is_copyable(iptr))::value, "Feature check failed");
}

void check_destructible( int i, double x, int* p, const A& a, const B& b, const C& c, const D& d, const E& e, const Ptr& ptr, const SmartPtr<int>& iptr ) {
    static_assert(decltype(is_destructible(i))::value, "Feature check failed");
    static_assert(decltype(is_destructible(x))::value, "Feature check failed");
    static_assert(decltype(is_destructible(p))::value, "Feature check failed");
    static_assert(decltype(is_destructible(a))::value, "Feature check failed");
    static_assert(decltype(is_destructible(b))::value, "Feature check failed");
    static_assert(decltype(is_destructible(c))::value, "Feature check failed");
    static_assert(!decltype(is_destructible(d))::value, "Feature check failed");
    static_assert(decltype(is_destructible(e))::value, "Feature check failed");
    static_assert(decltype(is_destructible(ptr))::value, "Feature check failed");
    static_assert(decltype(is_destructible(iptr))::value, "Feature check failed");
}

void check_assignable( int i, double x, int* p, const A& a, const B& b, const C& c, const D& d, const E& e, const Ptr& ptr, const SmartPtr<int>& iptr ) {
    static_assert(decltype(is_assignable(i))::value, "Feature check failed");
    static_assert(decltype(is_assignable(x))::value, "Feature check failed");
    static_assert(decltype(is_assignable(p))::value, "Feature check failed");
    static_assert(decltype(is_assignable(a))::value, "Feature check failed");
    static_assert(decltype(is_assignable(b))::value, "Feature check failed");
    static_assert(decltype(is_assignable(c))::value, "Feature check failed");
    static_assert(decltype(is_assignable(d))::value, "Feature check failed");
    static_assert(!decltype(is_assignable(e))::value, "Feature check failed");
    static_assert(decltype(is_assignable(ptr))::value, "Feature check failed");
    static_assert(decltype(is_assignable(iptr))::value, "Feature check failed");
}

void check_assignable0( int i, double x, int* p, const A& a, const B& b, const C& c, const D& d, const E& e, const Ptr& ptr, const SmartPtr<int>& iptr ) {
    static_assert(decltype(is_assignable0(i))::value, "Feature check failed");
    static_assert(decltype(is_assignable0(x))::value, "Feature check failed");
    static_assert(decltype(is_assignable0(p))::value, "Feature check failed");
    static_assert(!decltype(is_assignable0(a))::value, "Feature check failed");
    static_assert(decltype(is_assignable0(b))::value, "Feature check failed");
    static_assert(!decltype(is_assignable0(c))::value, "Feature check failed");
    static_assert(!decltype(is_assignable0(d))::value, "Feature check failed");
    static_assert(!decltype(is_assignable0(e))::value, "Feature check failed");
    static_assert(decltype(is_assignable0(ptr))::value, "Feature check failed");
    static_assert(!decltype(is_assignable0(iptr))::value, "Feature check failed");
}

void check_addable( int i, double x, int* p, const A& a, const B& b, const C& c, const D& d, const E& e, const Ptr& ptr, const SmartPtr<int>& iptr ) {
    static_assert(decltype(is_addable(i))::value, "Feature check failed");
    static_assert(decltype(is_addable(x))::value, "Feature check failed");
    static_assert(!decltype(is_addable(p))::value, "Feature check failed");
    static_assert(!decltype(is_addable(a))::value, "Feature check failed");
    static_assert(decltype(is_addable(b))::value, "Feature check failed");
    static_assert(decltype(is_addable(c))::value, "Feature check failed");
    static_assert(!decltype(is_addable(d))::value, "Feature check failed");
    static_assert(!decltype(is_addable(e))::value, "Feature check failed");
    static_assert(!decltype(is_addable(ptr))::value, "Feature check failed");
    static_assert(!decltype(is_addable(iptr))::value, "Feature check failed");
}

void check_pointer( int i, double x, int* p, const A& a, const B& b, const C& c, const D& d, const E& e, const Ptr& ptr, const SmartPtr<int>& iptr ) {
    static_assert(!decltype(is_pointer(i))::value, "Feature check failed");
    static_assert(!decltype(is_pointer(x))::value, "Feature check failed");
    static_assert(decltype(is_pointer(p))::value, "Feature check failed");
    static_assert(!decltype(is_pointer(a))::value, "Feature check failed");
    static_assert(!decltype(is_pointer(b))::value, "Feature check failed");
    static_assert(!decltype(is_pointer(c))::value, "Feature check failed");
    static_assert(!decltype(is_pointer(d))::value, "Feature check failed");
    static_assert(!decltype(is_pointer(e))::value, "Feature check failed");
    static_assert(decltype(is_pointer(ptr))::value, "Feature check failed");
    static_assert(decltype(is_pointer(iptr))::value, "Feature check failed");
}

void check_addable2( int i, double x, int* p, const A& a, const B& b, const C& c, const D& d, const E& e, const Ptr& ptr, const SmartPtr<int>& iptr ) {
    static_assert(decltype(is_addable2(i, x))::value, "Feature check failed");
    static_assert(decltype(is_addable2(p, i))::value, "Feature check failed");
    static_assert(!decltype(is_addable2(p, x))::value, "Feature check failed");
    static_assert(decltype(is_addable2(b, b))::value, "Feature check failed");
    static_assert(decltype(is_addable2(b, c))::value, "Feature check failed");
    static_assert(!decltype(is_addable2(b, e))::value, "Feature check failed");
}

int main() {
}
