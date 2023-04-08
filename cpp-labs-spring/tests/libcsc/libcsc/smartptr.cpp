#include <libcsc/smartptr/SharedPtr.hpp>
#include <libcsc/smartptr/UniquePtr.hpp>

#include <gtest/gtest.h>

TEST(SmartPtr, UniquePtr) {
    auto ptr = csc::smartptr::UniquePtr<std::string>(new std::string("hello"));
    EXPECT_EQ(*ptr, "hello");
    EXPECT_EQ(*(ptr.get()), "hello");
    EXPECT_STREQ(ptr->c_str(), "hello");
    ptr->append(" world");
    EXPECT_EQ(*ptr, "hello world");
    ptr = csc::smartptr::UniquePtr<std::string>(
        std::move(new std::string("moving ￣ .￣")));
    EXPECT_EQ(*(ptr.get()), "moving ￣ .￣");
    auto moving_ptr = csc::smartptr::UniquePtr<std::string>(
        new std::string("more moving!!! u.u"));
    ptr = std::move(moving_ptr);
    EXPECT_EQ(*ptr, "more moving!!! u.u");
    EXPECT_EQ(moving_ptr.get(), nullptr);
}

TEST(SmartPtr, SharedPtr) {
    auto ptr = csc::smartptr::SharedPtr<std::string>(new std::string("goes"));
    EXPECT_EQ(ptr.use_count(), 1);
    csc::smartptr::SharedPtr<std::string> another_ptr(ptr);
    EXPECT_EQ(another_ptr.use_count(), 2);
    EXPECT_EQ(ptr.use_count(), 2);
    EXPECT_EQ(*ptr, "goes");
    EXPECT_EQ(*another_ptr, "goes");
    EXPECT_EQ(*(ptr.get()), "goes");
    EXPECT_EQ(*(another_ptr.get()), "goes");
    EXPECT_STREQ(ptr->c_str(), "goes");
    EXPECT_STREQ(another_ptr->c_str(), "goes");
    another_ptr->append(" nowhere");
    EXPECT_EQ(*ptr, "goes nowhere");
    EXPECT_EQ(*another_ptr, "goes nowhere");
    another_ptr =
        csc::smartptr::SharedPtr<std::string>(new std::string("fragile"));
    EXPECT_EQ(ptr.use_count(), 1);
    EXPECT_EQ(another_ptr.use_count(), 1);
    EXPECT_EQ(*ptr, "goes nowhere");
    EXPECT_EQ(*another_ptr, "fragile");
    ptr.reset();
    EXPECT_EQ(ptr.get(), nullptr);
    EXPECT_EQ(ptr.use_count(), 0);
}