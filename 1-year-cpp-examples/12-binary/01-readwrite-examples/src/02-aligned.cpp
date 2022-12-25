#include <fmt/printf.h>

#include <cstdint>

struct Point {
  std::int32_t x_ = 0;
  std::int32_t y_ = 0;
};

void test_aligned_read() {
  // clang-format off
  char buf[24] = {
    0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
    0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  // clang-format on

  // zero-copy deserialization
  const std::uint32_t count = *reinterpret_cast<std::uint32_t*>(buf);
  Point* pts = reinterpret_cast<Point*>(buf + sizeof(count));

  for (std::uint32_t i = 0; i < count; ++i) {
    fmt::print("Point({} {}) ", pts[i].x_, pts[i].y_);
  }
  fmt::print("\n");
}

int main() {
  test_aligned_read();
}
