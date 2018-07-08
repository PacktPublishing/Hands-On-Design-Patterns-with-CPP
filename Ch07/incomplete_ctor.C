class Base {
    public:
    Base() : i_() {}
    virtual void increment(long v) { i_ += v; }
    private:
    long i_;
};

class Derived : public Base {
    public:
    Derived() : Base(), j_() {}
    Derived(long i, long j) : Base(i), j_(j) {}
    void increment(long v) { j_ += v; Base::increment(v); }
    private:
    long j_;
};

int main() {
    Derived d;
    d.increment(5);
}
