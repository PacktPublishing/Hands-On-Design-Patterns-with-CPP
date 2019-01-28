class Base {
    public:
    Base() : i_() {}
    virtual void increment(long v) { i_ += v; }
    private:
    long i_;
};

template <typename T>
class Derived : public T {
    public:
    Derived() : T(), j_() {}
    void increment(long v) { j_ += v; T::increment(v); }
    // This will not compile if we try to call it but is OK by itself.
    void multiply(long v) { j_ *= v; T::multiply(v); }
    private:
    long j_;
};

int main() {
    Derived<Base> d;
    d.increment(5);
}
