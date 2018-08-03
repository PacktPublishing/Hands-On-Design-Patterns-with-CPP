#include <stdlib.h>
#include <memory>

struct deleter {
    template <typename T> void operator()(T* p) { delete p; }
};


deleter d;

std::shared_ptr<int> get_shared_ptr() {
    return std::shared_ptr<int>(new int, d);
}
template <typename T>
struct deleter1 {
    void operator()(T* p) { delete p; }
};

template <typename T>
class smartptr_te {
    struct deleter_base {
        virtual void apply(void*) = 0;
        virtual ~deleter_base() {}
    };
    template <typename Deleter> struct deleter : public deleter_base {
        deleter(Deleter d) : d_(d) {}
        virtual void apply(void* p) { d_(static_cast<T*>(p)); }
        Deleter d_;
    };
    public:
    template <typename Deleter> smartptr_te(T* p, Deleter d) : p_(p), d_(new deleter<Deleter>(d)) {}
    ~smartptr_te() { d_->apply(p_); delete d_; }
    T* operator->() { return p_; }
    const T* operator->() const { return p_; }
    private:
    T* p_;
    deleter_base* d_;
};

template <typename T>
class smartptr_te_lb {
    struct deleter_base {
        virtual void apply(void*) = 0;
        virtual ~deleter_base() {}
    };
    template <typename Deleter> struct deleter : public deleter_base {
        deleter(Deleter d) : d_(d) {}
        virtual void apply(void* p) { d_(static_cast<T*>(p)); }
        Deleter d_;
    };
    public:
    template <typename Deleter> smartptr_te_lb(T* p, Deleter d) :
        p_(p),
        d_(sizeof(Deleter) > sizeof(buf_) ? new deleter<Deleter>(d) : new (buf_) deleter<Deleter>(d)) 
    {}
    ~smartptr_te_lb() {
        d_->apply(p_);
        if (static_cast<void*>(d_) == static_cast<void*>(buf_)) {
            d_->~deleter_base();
        } else {
            delete d_;
        }
    }
    T* operator->() { return p_; }
    const T* operator->() const { return p_; }
    private:
    T* p_;
    deleter_base* d_;
    char buf_[16];
};

smartptr_te<int> get_smartptr_te() {
    return smartptr_te<int>(new int, d);
}

smartptr_te_lb<int> get_smartptr_te_lb() {
    return smartptr_te_lb<int>(new int, d);
}
