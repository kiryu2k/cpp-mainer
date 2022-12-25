// https://gcc.gnu.org/onlinedocs/gcc/Zero-Length.html
//
// Массивы нулевой длины - нестандартное расширение. При компиляции с опцией
// -pedantic:
// warning: ISO C++ forbids zero-size array ‘items_’ [-Wpedantic]
//     8 |   char items_[0];
//       |               ^

#include <cstdint>
#include <iostream>

struct ArrayViewNonStandard {
  std::uint32_t size_;
  char items_[0];
};

int main() {
  char membuf[] = {4, 0, 0, 0, 'a', 'b', 'c', 'd'};

  auto* av = reinterpret_cast<ArrayViewNonStandard*>(membuf);

  std::cout << "Size: " << av->size_ << '\n';
  for (std::uint32_t i = 0; i < av->size_; ++i) {
    std::cout << av->items_[i] << ' ';
  }
}
