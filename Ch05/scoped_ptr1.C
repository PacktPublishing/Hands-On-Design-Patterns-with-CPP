#include <stdlib.h>

#include <new>

#include "gtest/gtest.h"

template <typename T>
class scoped_ptr {
    public:
    explicit scoped_ptr(T* p) : p_(p) {}
    ~scoped_ptr() { delete p_; }
    T* operator->() { return p_; }
    const T* operator->() const { return p_; }
    private:
    T* p_;
};

struct object_counter {
    static int count;
    object_counter() { ++count; }
    ~object_counter() { --count; }
};
int object_counter::count = 0;

TEST(Scoped_ptr, Construct) {
    object_counter::count = 0;
    {
        scoped_ptr<object_counter> p(new object_counter);
        EXPECT_EQ(1, object_counter::count);
    }
    EXPECT_EQ(0, object_counter::count);
}
