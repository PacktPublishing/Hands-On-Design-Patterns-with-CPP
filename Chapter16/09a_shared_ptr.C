// Version 09 with enable_if to control conversion policies
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
    DeleteSmallHeap(const DeleteSmallHeap&) = default;
    DeleteSmallHeap& operator=(const DeleteSmallHeap&) = default;
    DeleteSmallHeap(DeleteSmallHeap&& other)
        : heap_(other.heap_) 
    {
        other.heap_ = nullptr;
    }
    void operator()(T* p) const {
        p->~T();
        heap_->deallocate(p);
    }
    private:
    SmallHeap* heap_;
};

class NoMoveNoCopy {
    protected:
    NoMoveNoCopy() = default;
    NoMoveNoCopy(NoMoveNoCopy&&) = delete;
    NoMoveNoCopy(const NoMoveNoCopy&) = delete;
    constexpr bool must_delete() const { return true; }
};

class MoveNoCopy {
    protected:
    MoveNoCopy() = default;
    MoveNoCopy(MoveNoCopy&&) = default;
    MoveNoCopy(const MoveNoCopy&) = delete;
    constexpr bool must_delete() const { return true; }
};

class CopyRefCounted {
    protected:
    CopyRefCounted() : count_(new size_t(1)) {}
    CopyRefCounted(const CopyRefCounted& other)
        : count_(other.count_)
    {
        ++(*count_);
    }
    ~CopyRefCounted() {
        --(*count_);
        if (*count_ == 0) {
            delete count_;
        }
    }
    bool must_delete() const { return *count_ == 1; }
    private:
    size_t* count_;
};

struct NoRaw {
    static constexpr bool implicit_conv = false;
    static constexpr bool explicit_conv = false;
};

struct ExplicitRaw {
    static constexpr bool implicit_conv = false;
    static constexpr bool explicit_conv = true;
};

struct ImplicitRaw {
    static constexpr bool implicit_conv = true;
    static constexpr bool explicit_conv = false;
};

template <typename T,
          typename DeletionPolicy = DeleteByOperator<T>,
          typename CopyMovePolicy = NoMoveNoCopy,
          typename ConversionPolicy = ExplicitRaw
         >
class SmartPtr : private DeletionPolicy,
                 public CopyMovePolicy
{
    public:
    explicit SmartPtr(T* p = nullptr,
                      DeletionPolicy&& deletion_policy = DeletionPolicy()
        ) : DeletionPolicy(std::move(deletion_policy)),
            p_(p)
    {}
    SmartPtr(SmartPtr&& other)
        : DeletionPolicy(std::move(other)),
          CopyMovePolicy(std::move(other)),
          p_(other.p_)
    {
        other.release();
    }
    SmartPtr(const SmartPtr& other)
        : DeletionPolicy(other),
          CopyMovePolicy(other),
          p_(other.p_)
    {
    }
    ~SmartPtr() { 
        if (CopyMovePolicy::must_delete()) DeletionPolicy::operator()(p_);
    }
    // Explicit and implicit operators cannot be overloaded even if some are disabled:
    template <typename U = T, typename V = std::enable_if_t<ConversionPolicy::implicit_conv, U>>
    operator U*() { return p_; }
    template <typename U = T, typename V = std::enable_if_t<ConversionPolicy::implicit_conv, U>>
    operator const U*() const { return p_; }
    template <typename U = T, typename V = std::enable_if_t<ConversionPolicy::explicit_conv, U>>
    explicit operator U*() { return p_; }
    template <typename U = T, typename V = std::enable_if_t<ConversionPolicy::explicit_conv, U>>
    explicit operator const U*() const { return p_; }
    void release() { p_ = NULL; }
    T* operator->() { return p_; }
    const T* operator->() const { return p_; }
    T& operator*() { return *p_; }
    const T& operator*() const { return *p_; }
    private:
    T* p_;
};

int main() {
    {
        SmartPtr<int> p(new int(42));
        std::cout << *p << std::endl;
        int* p1(p);
        //int* p2 = p; // Does not compile
        std::cout << *p1 << std::endl;
    }

    {
        SmartPtr<int, DeleteByOperator<int>, NoMoveNoCopy, ImplicitRaw> p(new int(42));
        std::cout << *p << std::endl;
        int* p1(p);
        int* p2 = p; // Does not compile
        std::cout << *p1 << " " << *p2 << std::endl;
    }

    {
        SmartPtr<int, DeleteByOperator<int>, NoMoveNoCopy, NoRaw> p(new int(42));
        std::cout << *p << std::endl;
        //int* p1(p); // Does not compile
        //int* p2 = p; // Does not compile
    }
}

