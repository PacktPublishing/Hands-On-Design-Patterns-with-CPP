#include <cstdlib>
#include <iostream>

template <typename D> class registry {
    public:
    static size_t count;
    static D* head;
    D* prev;
    D* next;
    protected:
    registry() {
        ++count;
        prev = nullptr;
        next = head;
        head = static_cast<D*>(this);
        if (next) next->prev = head;
    }
    registry(const registry&) {
        ++count;
        prev = nullptr;
        next = head;
        head = static_cast<D*>(this);
        if (next) next->prev = head;
    }
    ~registry() {
        --count;
        if (prev) prev->next = next;
        if (next) next->prev = prev;
        if (head == this) head = next;
    }

};
template <typename D> size_t registry<D>::count(0);
template <typename D> D* registry<D>::head(nullptr);


class C : public registry<C> {
    int i_;
    public:
    C(int i) : i_(i) {}
    friend std::ostream& operator<<(std::ostream& out, const C& c) { out << c.i_; return out; }
};

class D : public registry<D> {
    int i_;
    public:
    D(int i) : i_(i) {}
    friend std::ostream& operator<<(std::ostream& out, const D& d) { out << d.i_; return out; }
};

template <typename T> void report() {
    std::cout << "Count: " << T::count; for (const T* p = T::head; p; p = p->next) std::cout << " " << *p; std::cout << std::endl;
}

int main() {
    report<C>();
    C* c4 = nullptr;
    {
        C c1(1);
        report<C>();
        c4 = new C(4);
        C c2(2);
        D d1(10);
        report<C>();
        report<D>();
    }
    report<C>();
    C c3(3);
    report<C>();
    delete c4;
    report<C>();
    report<D>();
}

