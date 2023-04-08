#include <fmt/format.h>

#include <iostream>

#define MAX(a, b) (((a) < (b)) ? (b) : (a))

template <typename T>
T max(T a, T b) {
  return (a < b) ? b : a;
}

int main() {
  {
    int a = 1;
    int b = 2;
    const int m = MAX(a++, b++);
    // Проблема: побочные эффекты.
    // Макрос - текстовая подстановка, инкремент b выполняется дважды.
    std::cout << fmt::format("MAX:   m = {}, a = {}, b = {}\n", m, a, b);
  }
  {
    int a = 1;
    int b = 2;
    const int m = ::max(a++, b++);
    // При вызове функции аргументы вычисляются один раз.
    std::cout << fmt::format("::max: m = {}, a = {}, b = {}\n", m, a, b);
  }
}
