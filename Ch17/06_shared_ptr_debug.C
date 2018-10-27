// Version 05 with debug policy.
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

struct NoDebug {
    template <typename P, typename T> static void construct(const P* ptr, const T* p) {}
    template <typename P, typename T> static void move(const P* ptr, const P* from, const T* p) {}
    template <typename P, typename T> static void copy(const P* ptr, const P* from, const T* p) {}
    template <typename P, typename T> static void destroy(const P* ptr, const T* p, bool must_delete) {}
};

struct Debug {
    template <typename P, typename T> static void construct(const P* ptr, const T* p) {
        std::cout << "Constructed SmartPtr at " << ptr << ", object " << static_cast<const void*>(p) << std::endl;
    }
    template <typename P, typename T> static void move(const P* ptr, const P* from, const T* p) {
        std::cout << "Moved SmartPtr at " << ptr << " from " << from << ", object " << static_cast<const void*>(p) << std::endl;
    }
    template <typename P, typename T> static void copy(const P* ptr, const P* from, const T* p) {
        std::cout << "Copied SmartPtr at " << ptr << " from " << from << ", object " << static_cast<const void*>(p) << std::endl;
    }
    template <typename P, typename T> static void destroy(const P* ptr, const T* p, bool must_delete) {
        std::cout << "Destroyed SmartPtr at " << ptr << ", object " << static_cast<const void*>(p) << (must_delete ? " is" : " is not") << " deleted" << std::endl;
    }
};

template <typename T,
          typename DeletionPolicy = DeleteByOperator<T>,
          typename CopyMovePolicy = NoMoveNoCopy,
          typename DebugPolicy = NoDebug
         >
class SmartPtr : private DeletionPolicy,
                 public CopyMovePolicy
{
    public:
    explicit SmartPtr(T* p = nullptr,
                      DeletionPolicy&& deletion_policy = DeletionPolicy()
        ) : DeletionPolicy(std::move(deletion_policy)),
            p_(p)
    {
        DebugPolicy::construct(this, p);
    }
    SmartPtr(SmartPtr&& other)
        : DeletionPolicy(std::move(other)),
          CopyMovePolicy(std::move(other)),
          p_(other.p_)
    {
        DebugPolicy::move(this, &other, p_);
        other.release();
    }
    SmartPtr(const SmartPtr& other)
        : DeletionPolicy(other),
          CopyMovePolicy(other),
          p_(other.p_)
    {
        DebugPolicy::copy(this, &other, p_);
    }
    ~SmartPtr() { 
        DebugPolicy::destroy(this, p_, CopyMovePolicy::must_delete());
        if (CopyMovePolicy::must_delete()) DeletionPolicy::operator()(p_);
    }
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
        SmartPtr<int, DeleteByOperator<int>, NoMoveNoCopy, Debug> p(new int(42));
        //SmartPtr<int> p1(std::move(p));  // Does not compile
        std::cout << *p << std::endl;
    }

    {
        SmallHeap h;
        SmartPtr<int, DeleteSmallHeap<int>, NoMoveNoCopy, Debug> p{new(&h) int(42), DeleteSmallHeap<int>(h)};
        std::cout << *p << std::endl;
    }

    {
        SmartPtr<int, DeleteByOperator<int>, CopyRefCounted, Debug> p{new int(42)};
        std::cout << *p << " " << std::endl;
        auto p1(p);
        std::cout << *p << " " << std::endl;
        std::cout << *p1 << " " << std::endl;
        {
            auto p2(p);
            std::cout << *p2 << " " << std::endl;
        }
        std::cout << *p1 << " " << std::endl;
        std::cout << *p << " " << std::endl;
    }

    {
        SmartPtr<int, DeleteByOperator<int>, MoveNoCopy, Debug> p{new int(42)};
        std::cout << *p << std::endl;
        auto p1(std::move(p));
        //auto p2(p);   // Does not compile
    }
}

