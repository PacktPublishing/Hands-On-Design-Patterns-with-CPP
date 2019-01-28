// Version 09 with policy rebind
#include <cstdlib>
#include <cassert>
#include <utility>
#include <iostream>

template <typename T>
struct DeleteByOperator {
    void operator()(T* p) const { 
        delete p;
    }
    template <typename U> using rebind_type = DeleteByOperator<U>;
};

template <typename T>
struct DeleteByFree {
    void operator()(T* p) const {
        p->~T();
        free(p);
    }
    template <typename U> using rebind_type = DeleteByFree<U>;
};

template <typename T>
struct DeleteDestructorOnly {
    void operator()(T* p) const {
        p->~T();
    }
    template <typename U> using rebind_type = DeleteDestructorOnly<U>;
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
    template <typename U> using rebind_type = DeleteSmallHeap<U>;
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

template <typename P, typename T>
struct NoRaw {
};

template <typename P, typename T>
struct ExplicitRaw {
    explicit operator T*() { return static_cast<P*>(this)->p_; }
    explicit operator const T*() const { return static_cast<const P*>(this)->p_; }
};

template <typename P, typename T>
struct ImplicitRaw {
    operator T*() { return static_cast<P*>(this)->p_; }
    operator const T*() const { return static_cast<const P*>(this)->p_; }
};

template <typename T,
          typename DeletionPolicy = DeleteByOperator<T>,
          typename CopyMovePolicy = NoMoveNoCopy,
          template <typename, typename> class ConversionPolicy = ExplicitRaw
         >
class SmartPtr : private DeletionPolicy,
                 public CopyMovePolicy,
                 public ConversionPolicy<SmartPtr<T, DeletionPolicy, CopyMovePolicy, ConversionPolicy>, T>
{
    public:
    using value_t = T;
    using deletion_policy_t = DeletionPolicy;
    using copy_move_policy_t = CopyMovePolicy;
    template <typename P, typename T1> using conversion_policy_t = ConversionPolicy<P, T1>;
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
    void release() { p_ = NULL; }
    T* operator->() { return p_; }
    const T* operator->() const { return p_; }
    T& operator*() { return *p_; }
    const T& operator*() const { return *p_; }

    template <typename U> using rebind_type = SmartPtr<U, typename DeletionPolicy::template rebind_type<U>, CopyMovePolicy, ConversionPolicy>;
    private:
    template<typename, typename> friend class ConversionPolicy;
    T* p_;
};

int main() {
    {
        SmartPtr<int, DeleteByOperator<int>, MoveNoCopy, ImplicitRaw> p(new int(42));
        std::cout << *p << std::endl;
        auto p1(std::move(p));
        std::cout << *p1 << std::endl;
        int* p2(p1);
        std::cout << *p2 << std::endl;

        using dptr_t = decltype(p)::rebind_type<double>;
        dptr_t q(new double(4.2));
        std::cout << *q << std::endl;
        auto q1(std::move(q));
        std::cout << *q1 << std::endl;
        double* q2(q1);
        std::cout << *q2 << std::endl;
    }

    {
        SmartPtr<int, DeleteByOperator<int>, MoveNoCopy, ImplicitRaw> p(new int(7));
        using ptr_t = decltype(p);
        SmartPtr<ptr_t::value_t, ptr_t::deletion_policy_t, ptr_t::copy_move_policy_t, ptr_t::conversion_policy_t> q(std::move(p));
        std::cout << *q << std::endl;
    }
}

