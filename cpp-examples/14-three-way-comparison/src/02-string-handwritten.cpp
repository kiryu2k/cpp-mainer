#include <gtest/gtest.h>

#include <algorithm>
#include <compare>
#include <cstring>

class String {
 public:
  String(const char* str, std::size_t length)
      : length_(length),
        buf_(new char[length + 1]()) {
    std::copy(str, str + length_, buf_);
  }

  String(const char* str)
      : String(str, std::strlen(str)) {
  }

  String(const String& other)
      : String(other.buf_, other.length_) {
  }

  String(String&& other) noexcept
      : length_(std::exchange(other.length_, 0)),
        buf_(std::exchange(other.buf_, nullptr)) {
  }

  ~String() {
    delete[] buf_;
  }

  String& operator=(const String& other) {
    String tmp(other);
    swap(tmp);
    return *this;
  }

  String& operator=(String&& other) noexcept {
    String tmp(std::move(other));
    swap(tmp);
    return *this;
  }

  bool operator==(const String& other) const noexcept {
    // Shortcut
    if (length_ != other.length_) {
      return false;
    }

    return buf_ == other;
  }

  bool operator==(const char* other) const noexcept {
    const auto* lhs_ptr = buf_;
    const auto* rhs_ptr = other;

    for (; *lhs_ptr; ++lhs_ptr, ++rhs_ptr) {
      if (!(*lhs_ptr == *rhs_ptr))
        return false;
    }
    return true;
  }

  std::strong_ordering operator<=>(const String& other) const noexcept {
    return buf_ <=> other;
  }

  // https://en.cppreference.com/w/cpp/algorithm/lexicographical_compare_three_way
  std::strong_ordering operator<=>(const char* other) const noexcept {
    const auto* lhs_ptr = buf_;
    const auto* rhs_ptr = other;

    bool lhs_exhaust = (*lhs_ptr == '\0');
    bool rhs_exhaust = (*rhs_ptr == '\0');
    for (; !lhs_exhaust && !rhs_exhaust;
         lhs_exhaust = (*++lhs_ptr == '\0'), rhs_exhaust = (*++rhs_ptr == '\0')) {
      if (auto c = *lhs_ptr <=> *rhs_ptr; c != 0) {
        return c;
      }
    }

    return !lhs_exhaust ? std::strong_ordering::greater
        : !rhs_exhaust  ? std::strong_ordering::less
                     : std::strong_ordering::equal;
  }

  std::size_t length() const {
    return length_;
  }

  const char* c_str() const {
    return buf_;
  }

 private:
  void swap(String& other) {
    std::swap(length_, other.length_);
    std::swap(buf_, other.buf_);
  }

  std::size_t length_;
  char* buf_;
};

std::ostream& operator<<(std::ostream& os, const String& str) {
  os << str.c_str();
  return os;
}

TEST(String, Comparison) {
  const String abc("abc");
  const String abcd("abcd");

  // We are using EXPECT_{TRUE, FALSE} instead of
  // EXPECT_{EQ, NE, LT, LE, GT, GE}
  // to explicitly call implemented operators from our code.

  // Shortcut
  EXPECT_FALSE(abc == abcd);

  // Rewrite: a != b -> !(a == b)
  // Shortcut
  EXPECT_TRUE(abc != abcd);

  // Call optimal overload: String::operator==(const char*).
  // No extra allocations.
  EXPECT_TRUE(abc == "abc");

  // Reverse: a == b -> b == a.
  // Call optimal overload: String::operator==(const char*).
  // No extra allocations.
  EXPECT_TRUE("abc" == abc);

  // Formula: a @ b -> (a <=> b) @ 0
  // Rewrite: a < b -> (a <=> b) < 0
  EXPECT_TRUE(abc < abcd);
  EXPECT_TRUE((abc <=> abcd) < 0);
  EXPECT_TRUE((abc <=> abcd) == std::strong_ordering::less);
}

TEST(String, RuleOfFive) {
  // Copy ctor
  {
    String a("a");
    String b(a);
    EXPECT_EQ(a, b);
  }

  // Move ctor
  {
    String a("a");
    String b(std::move(a));
    EXPECT_EQ(b, "a");
    EXPECT_EQ(0, a.length());
    EXPECT_EQ(nullptr, a.c_str());
  }

  // Copy assignment
  {
    String a("a");
    String b("b");
    a = b;
    EXPECT_EQ(a, b);
    EXPECT_EQ(a, "b");
  }

  // Move assignment
  {
    String a("a");
    String b("b");
    b = std::move(a);
    EXPECT_EQ(b, "a");
    EXPECT_EQ(0, a.length());
    EXPECT_EQ(nullptr, a.c_str());
  }
}
