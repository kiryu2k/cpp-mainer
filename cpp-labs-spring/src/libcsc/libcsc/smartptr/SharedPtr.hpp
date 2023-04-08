#pragma once

#include <cstddef>
#include <utility>

namespace csc::smartptr {

template <typename T> class SharedPtr {
private:
    T *ptr_ = nullptr;
    std::size_t *count_ = nullptr;

    void destruct() {
        if (count_ != nullptr) {
            if (--(*count_) == 0) {
                delete ptr_;
                delete count_;
            }
        }
    }

public:
    SharedPtr(T *const ptr) : ptr_{ptr}, count_{new std::size_t(1)} {}

    ~SharedPtr() { destruct(); }

    SharedPtr(const SharedPtr &copy) : ptr_(copy.ptr_), count_(copy.count_) {
        if (count_ != nullptr) {
            ++(*count_);
        }
    }

    SharedPtr &operator=(const SharedPtr &copy) {
        if (this == &copy) {
            return *this;
        }
        destruct();
        ptr_ = copy.ptr_;
        count_ = copy.count_;
        if (count_ != nullptr) {
            ++(*count_);
        }
        return *this;
    }

    SharedPtr(SharedPtr &&rhs) noexcept : ptr_(rhs.ptr_), count_(rhs.count_) {
        rhs.ptr_ = nullptr;
        rhs.count_ = nullptr;
    }

    SharedPtr &operator=(SharedPtr &&rhs) noexcept {
        if (this == &rhs) {
            return *this;
        }
        destruct();
        ptr_ = std::exchange(rhs.ptr_, nullptr);
        count_ = std::exchange(rhs.count_, nullptr);
        return *this;
    }

    T &operator*() const { return *ptr_; }

    T *operator->() const { return ptr_; }

    T *get() const { return ptr_; }

    void reset(T *const p = nullptr) {
        destruct();
        ptr_ = p;
        if (p != nullptr) {
            count_ = new size_t(1);
        } else {
            count_ = nullptr;
        }
    }

    std::size_t use_count() const { return (count_ != nullptr) ? *count_ : 0; }
};

} // namespace csc::smartptr