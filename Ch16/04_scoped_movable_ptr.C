// Version 03 with move policy.
#include <cstdlib>
#include <cassert>
#include <utility>
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
        : heap_(&heap) {}
    void operator()(T* p) const {
        p->~T();
        heap_->deallocate(p);
    }
    DeleteSmallHeap(DeleteSmallHeap&& other)
        : heap_(other.heap_) 
    {
        other.heap_ = nullptr;
    }
    private:
    SmallHeap* heap_;
    DeleteSmallHeap(const DeleteSmallHeap&) = delete;
    DeleteSmallHeap& operator=(const DeleteSmallHeap&) = delete;
};

struct MoveForbidden {
    MoveForbidden() = default;
    MoveForbidden(MoveForbidden&&) = delete;
    MoveForbidden(const MoveForbidden&) = delete;
    MoveForbidden& operator=(MoveForbidden&&) = delete;
    MoveForbidden& operator=(const MoveForbidden&) = delete;
};

struct MoveAllowed {
};

template <typename T,
          typename DeletionPolicy = DeleteByOperator<T>,
          typename MovePolicy = MoveForbidden
         >
class SmartPtr : private DeletionPolicy,
                 private MovePolicy
{
    public:
    explicit SmartPtr(T* p = nullptr,
                      DeletionPolicy&& deletion_policy = DeletionPolicy()
        ) : DeletionPolicy(std::move(deletion_policy)),
            p_(p)
    {}
    SmartPtr(SmartPtr&& other)
        : DeletionPolicy(std::move(other)),
          MovePolicy(std::move(other)),
          p_(other.p_)
    {
        other.release();
    }
    ~SmartPtr() { 
        DeletionPolicy::operator()(p_);
    }
    SmartPtr& operator=(SmartPtr&& other) {
        if (this == &other) return *this;
        DeletionPolicy::operator=(std::move(other));
        MovePolicy::operator=(std::move(other));
        p_ = other.p_;
        other.release();
        return *this;
    }
    void release() { p_ = NULL; }
    T* operator->() { return p_; }
    const T* operator->() const { return p_; }
    T& operator*() { return *p_; }
    const T& operator*() const { return *p_; }
    private:
    T* p_;
    SmartPtr(const SmartPtr&) = delete;
    SmartPtr& operator=(const SmartPtr&) = delete;
};

int main() {
    {
        SmartPtr<int> p(new int(42));
        //SmartPtr<int> p1(std::move(p));  // Does not compile
        std::cout << *p << std::endl;
        SmartPtr<int> q;
        //q = std::move(p); // Does not compile
    }

    {
        SmallHeap h;
        SmartPtr<int,DeleteSmallHeap<int>> p{new(&h) int(42), DeleteSmallHeap<int>(h)};
        std::cout << *p << std::endl;
    }

    {
        // C++17 does not help here since MovePolicy cannot be deduced from the constructor
        SmartPtr<int, DeleteByOperator<int>, MoveAllowed> p{new int(42)};
        std::cout << *p << std::endl;
        auto p1(std::move(p));
        //std::cout << *p << std::endl; // This will crash
        std::cout << *p1 << std::endl;
        SmartPtr<int, DeleteByOperator<int>, MoveAllowed> q;
        q = std::move(p1);
        //std::cout << *p1 << std::endl; // This will crash
        std::cout << *q << std::endl;
    }
}

