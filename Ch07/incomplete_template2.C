class Base {
    public:
    Base() : i_() {}
    virtual void increment(long v) { i_ += v; }
    private:
    long i_;
};

template <typename T>
class Derived : public Base {
    public:
    Derived() : Base(), j_() {}
    void increment(long v) { j_ += v; Base::increment(v); }
    // This does not compile even though multiply() is not used:
    // Base does not depend on the template parameter T
    void multiply(long v) { j_ *= v; Base::multiply(v); }
    private:
    T j_;
};

int main() {
    Derived<long> d;
    d.increment(5);
}
