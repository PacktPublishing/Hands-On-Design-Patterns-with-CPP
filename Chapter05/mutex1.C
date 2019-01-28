#include <stdlib.h>

#include <mutex>

#include "gtest/gtest.h"

class mutex_guard {
    public:
    explicit mutex_guard(std::mutex& m) : m_(m), must_unlock_(true) { m_.lock(); }
    ~mutex_guard() { if (must_unlock_) m_.unlock(); }
    void reset() { m_.unlock(); must_unlock_ = false; }
    private:
    std::mutex& m_;
    bool must_unlock_;
};


std::mutex m;

TEST(mutex_guard, LockUnlock) {
    {
        mutex_guard lg(m);
        EXPECT_FALSE(m.try_lock());
    }
    EXPECT_TRUE(m.try_lock()); m.unlock();
}

TEST(Scoped_ptr, ThrowNoLeak) {
    try {
        mutex_guard lg(m);
        EXPECT_FALSE(m.try_lock());
        throw 1;
    } catch ( ... ) {
    }
    EXPECT_TRUE(m.try_lock()); m.unlock();
}

TEST(mutex_guard, Reset) {
    {
        mutex_guard lg(m);
        EXPECT_FALSE(m.try_lock());
        lg.reset();
        EXPECT_TRUE(m.try_lock()); m.unlock();
    }
    EXPECT_TRUE(m.try_lock()); m.unlock();
}

