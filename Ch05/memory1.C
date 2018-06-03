#include <stdlib.h>

#include "gtest/gtest.h"

struct object_counter {
    static int count;
    static int all_count;
    object_counter() { ++count; ++all_count; }
    ~object_counter() { --count; }
};
int object_counter::count = 0;
int object_counter::all_count = 0;

TEST(Memory, AcquireRelease) {
    object_counter::all_count = object_counter::count = 0;
    object_counter* p = new object_counter;
    EXPECT_EQ(1, object_counter::count);
    EXPECT_EQ(1, object_counter::all_count);
    delete p;
    EXPECT_EQ(0, object_counter::count);
    EXPECT_EQ(1, object_counter::all_count);
}

TEST(Memory, Leak1) {
    object_counter::all_count = object_counter::count = 0;
    object_counter* p = new object_counter;
    EXPECT_EQ(1, object_counter::count);
    EXPECT_EQ(1, object_counter::all_count);
    (void)p;
    //delete p;
    EXPECT_EQ(0, object_counter::count);
    EXPECT_EQ(1, object_counter::all_count);
}

TEST(Memory, Leak2) {
    object_counter::all_count = object_counter::count = 0;
    do {        // One-time scope for early exit
        object_counter* p = new object_counter;
        break;  // Early exit
        delete p;
    } while (false);
    EXPECT_EQ(0, object_counter::count);
    EXPECT_EQ(1, object_counter::all_count);
}

struct my_exception {
};

TEST(Memory, Leak3) {
    object_counter::all_count = object_counter::count = 0;
    try {
        object_counter* p = new object_counter;
        throw my_exception();
        delete p;
    } catch ( my_exception& e ) {
    }
    EXPECT_EQ(0, object_counter::count);
    EXPECT_EQ(1, object_counter::all_count);
}
