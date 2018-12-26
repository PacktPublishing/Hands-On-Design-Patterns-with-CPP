// Version 02 with template template deletion policy.
#include <cstdlib>
#include <cassert>
#include <iostream>

template <typename T>
struct DeleteByOperator {
    void operator()(T* p) const { 
        delete p;
    }
};

template <typename T>
struct DeleteByFree {
    void operator()(T* p) const {
        p->~T();
        free(p);
    }
};

template <typename T>
struct DeleteDestructorOnly {
    void operator()(T* p) const {
        p->~T();
    }
};

class SmallHeap {
    public:
    SmallHeap() {}
    ~SmallHeap() {}
    void* allocate(size_t s) {
        assert(s <= size_);
        return mem_;
    }
    void deallocate(void* p) {
        assert(p == mem_);
    }
    private:
    static constexpr size_t size_ = 1024;
    char mem_[size_];
    SmallHeap(const SmallHeap&) = delete;
    SmallHeap& operator=(const SmallHeap&) = delete;
};
void* operator new(size_t s, SmallHeap* h) { return h->allocate(s); }

template <typename T>
struct DeleteSmallHeap {
    explicit DeleteSmallHeap(SmallHeap& heap)
        : heap_(heap) {}
    void operator()(T* p) const {
        p->~T();
        heap_.deallocate(p);
    }
    private:
    SmallHeap& heap_;
};

template <typename T, template <typename> class DeletionPolicy = DeleteByOperator>
class SmartPtr {
    public:
    explicit SmartPtr(T* p = nullptr,
                      const DeletionPolicy<T>& deletion_policy = DeletionPolicy<T>()
        ) : p_(p),
            deletion_policy_(deletion_policy)
    {}
    ~SmartPtr() { 
        deletion_policy_(p_);
    }
    void release() { p_ = NULL; }
    T* operator->() { return p_; }
    const T* operator->() const { return p_; }
    T& operator*() { return *p_; }
    const T& operator*() const { return *p_; }
    private:
    T* p_;
    DeletionPolicy<T> deletion_policy_;
    SmartPtr(const SmartPtr&) = delete;
    SmartPtr& operator=(const SmartPtr&) = delete;
};

int main() {
    {
        SmartPtr<int> p(new int(42));
        std::cout << *p << std::endl;
    }

    {
        SmallHeap h;
        SmartPtr<int, DeleteSmallHeap> p{new(&h) int(42), DeleteSmallHeap<int>(h)};
        std::cout << *p << std::endl;
    }
}

