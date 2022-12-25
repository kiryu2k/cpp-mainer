#include "dump.hpp"

#include <cstring>

void test_write_int() {
  char buf[8] = {};
  const std::int32_t value = 4098;
  std::memcpy(buf, &value, sizeof(value));

  fmt::print("write_int\n");
  fmt::print("{} | {}\n\n", dump(buf), value);
}

void test_write_ints() {
  char buf[8] = {};
  const std::int32_t a = 4098, b = 42;
  std::memcpy(buf, &a, sizeof(a));
  std::memcpy(buf + sizeof(a), &b, sizeof(b));

  fmt::print("write_ints\n");
  fmt::print("{} | {} {}\n\n", dump(buf), a, b);
}

struct Point {
  std::int32_t x_ = 0;
  std::int32_t y_ = 0;
};

void test_write_struct() {
  char buf[8] = {};
  const Point p = {16, 20};
  std::memcpy(buf, &p, sizeof(p));

  fmt::print("write_struct\n");
  fmt::print("{} | point({}, {})\n\n", dump(buf), p.x_, p.y_);
}

void test_write_struct_by_fields() {
  char buf[8] = {};
  const Point p = {16, 20};
  std::memcpy(buf, &p.x_, sizeof(std::int32_t));
  std::memcpy(buf + sizeof(std::int32_t), &p.y_, sizeof(std::int32_t));

  fmt::print("write_struct_by_fields\n");
  fmt::print("{} | point({}, {})\n\n", dump(buf), p.x_, p.y_);
}

struct IntsWithPadding {
  std::uint8_t a_;
  std::int32_t b_;
};
static_assert(sizeof(IntsWithPadding) == 8);

void test_write_struct_with_alignment() {
  char buf[8] = {};
  const IntsWithPadding obj = {127, 5};

  std::memcpy(buf, &obj, sizeof(obj));
  fmt::print("write_struct_with_alignment\n");
  fmt::print("{} | {}, {}\n\n", dump(buf), obj.a_, obj.b_);
}

#pragma pack(push, 1)
struct PackedInts {
  std::uint8_t a_;
  std::int32_t b_;
};
#pragma pack(pop)
static_assert(sizeof(PackedInts) == 5);

void test_write_packed_struct() {
  char buf[8] = {};
  const PackedInts obj = {127, 5};

  std::memcpy(buf, &obj, sizeof(obj));
  fmt::print("write_packed_struct\n");
  fmt::print("{} | {}, {}\n\n", dump(buf), obj.a_, obj.b_);
}

int main() {
  test_write_int();
  test_write_ints();
  test_write_struct();
  test_write_struct_by_fields();
  test_write_struct_with_alignment();
  test_write_packed_struct();
}
