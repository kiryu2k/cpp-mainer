#include <fmt/format.h>

#include <iostream>
#include <map>
#include <string>

// Проблема 1: копирование потенциально тяжелой структуры
void traverse_bad(std::map<std::string, bool> text_to_flag) {
  std::cout << fmt::format(
      "traverse_bad: map.size() = {}\n", text_to_flag.size());
  for (const std::string text : {"two", "three", "four"}) {
    // Проблема 2: оператор[] создает элементы.
    if (text_to_flag[text]) {
      // some processing
    }
  }
  std::cout << fmt::format(
      "traverse_bad: map.size() = {}\n", text_to_flag.size());
}

// Ok: структура передается по константной ссылке
// - Нет лишних копий
// - Есть гарантия: функция не меняет объект
void traverse_good(const std::map<std::string, bool>& text_to_flag) {
  std::cout << fmt::format(
      "traverse_good: map.size() = {}\n", text_to_flag.size());

  for (const std::string text : {"two", "three", "four"}) {
    // У оператора[] нет константной версии, ошибка компиляции
    // if (text_to_flag[text])

    if (auto it = text_to_flag.find(text);
        it != text_to_flag.end() && it->second) {
      // some processing
    }
  }
  std::cout << fmt::format(
      "traverse_good: map.size() = {}\n", text_to_flag.size());
}

int main() {
  std::map<std::string, bool> text_to_flag;

  for (const std::string text : {"one", "two", "three"}) {
    text_to_flag[text] = true;
  }

  traverse_bad(text_to_flag);
  traverse_good(text_to_flag);
}
