#include "dump.hpp"

#include <vector>

void test_c_string() {
  char buf[8] = {};

  std::string str = "hello";
  std::memcpy(buf, str.c_str(), str.size());

  fmt::print("c-string\n");
  fmt::print("{} | {}\n\n", dump(buf), buf);
}

void test_pascal_string() {
  char buf[8] = {0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f};

  // type for len?
  // - fixed: u8, u16, u32, u64
  // - varint
  std::string str = "hello";
  const std::uint8_t len = str.size();
  std::memcpy(buf, &len, sizeof(len));
  std::memcpy(buf + sizeof(len), str.c_str(), str.size());

  // pascal string is not null-terminated
  const std::uint8_t read_len = *buf;
  std::string_view read_str(buf + sizeof(read_len), read_len);

  fmt::print("pascal string\n");
  fmt::print("{} | {}\n", dump(buf), read_str);
}

int main() {
  test_c_string();
  test_pascal_string();
}
