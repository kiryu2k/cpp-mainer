#pragma once

#include <utility>

namespace csc::smartptr {

template <typename T> class UniquePtr {
private:
    T *ptr_ = nullptr;

public:
    UniquePtr(T *const ptr) : ptr_(ptr) {}

    ~UniquePtr() { delete ptr_; }

    UniquePtr(UniquePtr &&rhs) noexcept : ptr_(rhs.ptr_) { rhs.ptr_ = nullptr; }

    UniquePtr &operator=(UniquePtr &&rhs) noexcept {
        if (this == &rhs) {
            return *this;
        }
        if (ptr_ != nullptr) {
            delete ptr_;
        }
        ptr_ = std::exchange(rhs.ptr_, nullptr);
        return *this;
    }

    UniquePtr(const UniquePtr &) = delete;

    UniquePtr operator=(const UniquePtr &) = delete;

    T &operator*() const { return *ptr_; }

    T *operator->() const { return ptr_; }

    T *get() const { return ptr_; }

    void reset(T *const p = nullptr) {
        if (ptr_ != nullptr) {
            delete ptr_;
        }
        ptr_ = p;
    }
};

} // namespace csc::smartptr
