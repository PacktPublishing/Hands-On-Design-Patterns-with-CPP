// Basic smart pointer with automatic deletion.
#include <cstdlib>
#include <iostream>

template <typename T>
class SmartPtr {
    public:
    explicit SmartPtr(T* p = nullptr)
        : p_(p) {}
    ~SmartPtr() { 
        delete p_;
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
    SmartPtr<int> p(new int(42));
    std::cout << *p << std::endl;
}
