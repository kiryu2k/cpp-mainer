#include "dump.hpp"

#include <fmt/printf.h>

#include <cstdint>

void test_write_unaligned() {
  char buf[8] = {0, 1, 2, 3, 4, 5, 6, 7};

  auto* unaligned_ptr = reinterpret_cast<std::int32_t*>(buf + 1);
  *unaligned_ptr = 42;  // boom

  fmt::print("{}\n", dump(buf));
}

int main() {
  test_write_unaligned();
}
