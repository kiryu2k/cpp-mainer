#pragma once

#include <functional>
#include <memory>

namespace csc::hashmap {

template <
    typename Key,
    typename T,
    typename Hash = std::hash<Key>,
    typename KeyEqual = std::equal_to<Key>>
class HashMap {
private:
    class ForwardIterator;

    /* HashMap's node implementation */
    class Node {
    private:
        friend HashMap;
        friend ForwardIterator;
        Key key_;
        T value_;
        Node *next_;

    public:
        Node(const Key &key, const T &value)
            : key_{key}, value_{value}, next_{nullptr} {}

        Key key() const { return key_; }

        T value() const { return value_; }

        bool operator==(const Node &other) const {
            if (this == &other) {
                return true;
            }
            return (key_ == other.key_) && (value_ == other.value_);
        }
    };

    /* ForwardIterator implementation */
    class ForwardIterator {
    public:
        using value_type = T;
        using reference = Node &;
        using pointer = Node *;
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;

    private:
        pointer ptr_;
        pointer *table_;
        std::size_t index_;
        std::size_t bucket_count_;

    public:
        ForwardIterator()
            : ptr_{nullptr}, table_{nullptr}, index_{0}, bucket_count_{0} {}

        ForwardIterator(
            pointer ptr,
            pointer *table,
            std::size_t index,
            std::size_t bucket_count)
            : ptr_{ptr}, table_{table}, index_{index}, bucket_count_{
                                                           bucket_count} {}

        reference operator*() { return *ptr_; }

        const reference operator*() const { return *ptr_; }

        pointer operator->() { return ptr_; }

        const pointer operator->() const { return ptr_; }

        ForwardIterator &operator++() {
            if (ptr_ && ptr_->next_) {
                ptr_ = ptr_->next_;
                return *this;
            }
            ptr_ = nullptr;
            for (++index_; index_ < bucket_count_; ++index_) {
                if (table_[index_]) {
                    ptr_ = table_[index_];
                    break;
                }
            }
            return *this;
        }

        ForwardIterator operator++(int) {
            auto old = *this;
            ++(*this);
            return old;
        }

        bool operator==(const ForwardIterator &other) const {
            if (this == &other) {
                return true;
            }
            return ptr_ == other.ptr_;
        }

        bool operator!=(const ForwardIterator &other) const {
            return !(*this == other);
        }
    };

    /* HashMap's fields */
    Node **table_;
    std::size_t bucket_count_;
    Hash calculate_hash_;
    KeyEqual is_equal_;

    void swap(HashMap &other) noexcept {
        std::swap(bucket_count_, other.bucket_count_);
        std::swap(table_, other.table_);
    }

public:
    explicit HashMap(std::size_t bucket_count = 1024)
        : table_(new Node *[bucket_count]), bucket_count_{bucket_count} {
        for (std::size_t i = 0; i < bucket_count_; ++i) {
            table_[i] = nullptr;
        }
    }

    ForwardIterator insert(const Key &key, const T &value) {
        auto iter = find(key);
        if (iter != end()) {
            return iter;
        }
        const auto hash_value = calculate_hash_(key) % bucket_count_;
        auto *node = new Node(key, value);
        node->next_ = table_[hash_value];
        table_[hash_value] = node;
        return ForwardIterator(node, table_, hash_value, bucket_count_);
    }

    ForwardIterator end() const {
        if (bucket_count_ == 0) {
            return ForwardIterator();
        }
        const auto index = bucket_count_ - 1;
        auto iter =
            ForwardIterator(table_[index], table_, index, bucket_count_);
        if (!table_[index]) {
            return iter;
        }
        while (iter->next_) {
            ++iter;
        }
        return ++iter;
    }

    ForwardIterator begin() const {
        for (std::size_t i = 0; i < bucket_count_; ++i) {
            if (table_[i]) {
                return ForwardIterator(table_[i], table_, i, bucket_count_);
            }
        }
        return end();
    }

    ForwardIterator find(const Key &key) const {
        const auto hash_value = calculate_hash_(key) % bucket_count_;
        for (auto *node = table_[hash_value]; node; node = node->next_) {
            if (is_equal_(node->key_, key)) {
                return ForwardIterator(node, table_, hash_value, bucket_count_);
            }
        }
        return end();
    }

    std::size_t bucket_size(std::size_t bucket_index) const {
        std::size_t size = 0;
        if (bucket_index >= bucket_count_) {
            return size;
        }
        for (auto *node = table_[bucket_index]; node; node = node->next_) {
            ++size;
        }
        return size;
    }

    std::size_t bucket_count() const { return bucket_count_; }

    std::size_t size() const { return std::distance(begin(), end()); }

    T &at(const Key &key) {
        auto iter = find(key);
        if (iter == end()) {
            throw std::out_of_range("oops.. key not found =(");
        }
        return iter->value_;
    }

    T &operator[](const Key &key) {
        auto iter = find(key);
        if (iter == end()) {
            T value;
            iter = insert(key, value);
        }
        return iter->value_;
    }

    T &operator[](Key &&key) { return operator[](key); }

    void erase(const Key &key) {
        const auto hash_value = calculate_hash_(key) % bucket_count_;
        Node *previous = nullptr;
        auto *node = table_[hash_value];
        for (; node != nullptr && !is_equal_(node->key_, key);
             node = node->next_) {
            previous = node;
        }
        if (!node) {
            return;
        }
        if (!previous) {
            /* remove first node of list */
            table_[hash_value] = node->next_;
        } else {
            previous->next_ = node->next_;
        }
        delete node;
    }

    void erase(const ForwardIterator &iterator) { erase(iterator->key_); }

    /* Rule of five */
    HashMap(const HashMap<Key, T, Hash, KeyEqual> &other)
        : HashMap(other.bucket_count()) {
        for (auto iter = other.begin(); iter != other.end(); ++iter) {
            insert(iter->key_, iter->value_);
        }
    }

    HashMap(HashMap<Key, T, Hash, KeyEqual> &&other) noexcept
        : table_{other.table_}, bucket_count_{other.bucket_count_} {
        other.table_ = nullptr;
        other.bucket_count_ = 0;
    }

    HashMap &operator=(const HashMap &rhs) {
        if (this != &rhs) {
            HashMap copy(rhs);
            copy.swap(*this);
        }
        return *this;
    }

    HashMap &operator=(HashMap &&rhs) noexcept {
        if (this != &rhs) {
            table_ = nullptr;
            bucket_count_ = 0;
            rhs.swap(*this);
        }
        return *this;
    }

    ~HashMap() {
        for (std::size_t i = 0; i < bucket_count_; ++i) {
            auto *node = table_[i];
            while (node) {
                Node *temp = node;
                node = node->next_;
                delete temp;
            }
        }
        delete[] table_;
    }
};

} // namespace csc::hashmap