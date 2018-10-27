// Version 04 with enable_if for move constructor and operator->()
#include <cstdlib>
#include <cassert>
#include <utility>
#include <iostream>
#include <type_traits>

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
    static constexpr bool can_move = false;
};

struct MoveAllowed {
    static constexpr bool can_move = true;
};

struct WithArrow {
    static constexpr bool have_arrow = true;
};

struct WithoutArrow {
    static constexpr bool have_arrow = false;
};

template <typename T,
          typename DeletionPolicy = DeleteByOperator<T>,
          typename MovePolicy = MoveForbidden,
          typename ArrowPolicy = WithArrow
         >
class SmartPtr : private DeletionPolicy
{
    public:
    explicit SmartPtr(T* p = nullptr,
                      DeletionPolicy&& deletion_policy = DeletionPolicy()
        ) : DeletionPolicy(std::move(deletion_policy)),
            p_(p)
    {}
    template <typename U, typename V = std::enable_if_t<MovePolicy::can_move && std::is_same<U, SmartPtr>::value, U>>
    SmartPtr(U&& other)
        : DeletionPolicy(std::move(other)),
          p_(other.p_)
    {
        other.release();
    }
    ~SmartPtr() { 
        DeletionPolicy::operator()(p_);
    }
    void release() { p_ = NULL; }
    // SFINAE works on template functions only
    //std::enable_if_t<ArrowPolicy::have_arrow, T*> operator->() { return p_; } // Does not work
    template <typename U = T> std::enable_if_t<ArrowPolicy::have_arrow, U*> operator->() { return p_; }
    template <typename U = T> std::enable_if_t<ArrowPolicy::have_arrow, const U*> operator->() const { return p_; }
    T& operator*() { return *p_; }
    const T& operator*() const { return *p_; }
    private:
    T* p_;
    SmartPtr(const SmartPtr&) = delete;
    SmartPtr& operator=(const SmartPtr&) = delete;
};

struct S0 {};
struct S1 {
    int i;
    explicit S1(int i) : i(i) {}
};

int main() {
    {
        SmartPtr<S1> p(new S1(42));
        std::cout << p->i << std::endl;
        //auto p1(std::move(p)); // Does not compile
    }

    {
        SmartPtr<S1, DeleteByOperator<S1>, MoveAllowed, WithoutArrow> p{new S1(42)};
        //std::cout << p->i << std::endl; // Does not compile
        auto p1(std::move(p));
    }
}

