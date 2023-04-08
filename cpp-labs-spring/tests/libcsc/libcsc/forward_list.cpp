#include <libcsc/forwardlist/forward_list.hpp>

#include <gtest/gtest.h>

template <typename T> bool has_loop(csc::forwardlist::ForwardList<T> &list) {
    auto turtle_iter = list.begin();
    auto rabbit_iter = list.begin();
    while (turtle_iter != nullptr && rabbit_iter != nullptr &&
           rabbit_iter->next_ != nullptr) {
        ++turtle_iter;
        ++rabbit_iter;
        ++rabbit_iter;
        if (turtle_iter == rabbit_iter) {
            return true;
        }
    }
    return false;
}

template <typename T> void make_loop(csc::forwardlist::ForwardList<T> &list) {
    auto begin_iterator = list.begin();
    auto end_iterator = list.end();
    if (begin_iterator != nullptr && begin_iterator->next_ != nullptr) {
        end_iterator->next_ = begin_iterator->next_;
    }
}

TEST(ForwardList, EvenCountElementsList) {
    csc::forwardlist::ForwardList<std::string_view> list;
    list.push_front("hello");
    list.push_front("ugh");
    list.push_front("endless");
    list.push_front("loooooooop");
    EXPECT_EQ(has_loop(list), false);
    make_loop(list);
    EXPECT_EQ(has_loop(list), true);
}

TEST(ForwardList, OddCountElementsList) {
    csc::forwardlist::ForwardList<std::string_view> list;
    list.push_front("idk");
    list.push_front("what");
    list.push_front("to call");
    list.push_front("these");
    list.push_front("nodes");
    EXPECT_EQ(has_loop(list), false);
    make_loop(list);
    EXPECT_EQ(has_loop(list), true);
}

TEST(ForwardList, EmptyList) {
    csc::forwardlist::ForwardList<std::string_view> list;
    EXPECT_EQ(has_loop(list), false);
    EXPECT_EQ(list.begin(), nullptr);
    make_loop(list);
    EXPECT_EQ(has_loop(list), false);
}

TEST(ForwardList, OneElementList) {
    csc::forwardlist::ForwardList<std::string_view> list;
    list.push_front("alone node");
    EXPECT_EQ(has_loop(list), false);
    make_loop(list);
    EXPECT_EQ(has_loop(list), false);
}