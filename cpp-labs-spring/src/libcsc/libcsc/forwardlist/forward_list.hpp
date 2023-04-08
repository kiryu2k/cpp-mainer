#include <memory>

namespace csc::forwardlist {

template <typename T> class ForwardList {
private:
    class Node {
    public:
        std::unique_ptr<T> value_;
        Node *next_;

        Node() : value_{nullptr}, next_{nullptr} {}

        Node(const T &value)
            : value_{std::make_unique<T>(value)}, next_{nullptr} {}
    };

    Node *head_;
    std::size_t size_ = 0;

    template <typename U> class ForwardIterator {
    private:
        using value_type = U;
        using reference = value_type &;
        using pointer = value_type *;
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;

        pointer ptr_;

    public:
        ForwardIterator() : ptr_{nullptr} {}

        ForwardIterator(pointer ptr) : ptr_{ptr} {}

        ForwardIterator(const ForwardIterator &iterator)
            : ptr_{iterator.ptr_} {}

        const reference operator*() const { return *ptr_; }

        pointer operator->() { return ptr_; }

        ForwardIterator &operator++() {
            ptr_ = ptr_->next_;
            return *this;
        }

        ForwardIterator operator++(int) {
            auto tmp = *this;
            ++(*this);
            return tmp;
        }

        friend bool
        operator==(const ForwardIterator &lhs, const ForwardIterator &rhs) {
            return lhs.ptr_ == rhs.ptr_;
        }

        friend bool
        operator!=(const ForwardIterator &lhs, const ForwardIterator &rhs) {
            return lhs.ptr_ != rhs.ptr_;
        }
    };

public:
    ForwardList() : head_{nullptr} {}

    ~ForwardList() {
        for (; size_ > 0; --size_) {
            auto *node = head_;
            head_ = head_->next_;
            delete node;
        }
    }

    /* implementing destructor moment.. */
    ForwardList(const ForwardList<T> &list) = delete;

    ForwardList(ForwardList<T> &&list) = delete;

    ForwardList<T> &operator=(const ForwardList<T> &rhs) = delete;

    ForwardList<T> &operator=(ForwardList<T> &&rhs) = delete;
    /* rule of five moment.. */

    void push_front(const T &value) {
        auto *node = new Node(value);
        node->next_ = head_;
        head_ = node;
        ++size_;
    }

    ForwardIterator<Node> begin() { return ForwardIterator<Node>(head_); }

    ForwardIterator<Node> end() {
        auto iterator = begin();
        while (iterator != nullptr && iterator->next_ != nullptr) {
            ++iterator;
        }
        return iterator;
    }
};

} // namespace csc::forwardlist