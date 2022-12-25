#include <stdio.h>

void repeat(char c, int count) {
  for (int i = 0; i < count; ++i) {
    printf("%c", c);
  }
  printf("\n");
}

int main() {
  // ord('!') == 33
  // ord(')') == 41

  // repeat(')', 33);
  asm("mov $')', %rdi\n"
      "mov $33, %rsi\n"
      "call repeat\n");

  // repeat((char)33, (int)')');
  // repeat('!', 41);
  asm("mov $33, %rdi\n"
      "mov $')', %rsi\n"
      "call repeat\n");
}
