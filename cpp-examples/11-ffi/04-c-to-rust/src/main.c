#include <stdint.h>
#include <stdio.h>

struct Ints {
  int8_t a;
  int16_t b;
  int8_t c;
};

void fill_rust(struct Ints*);
void fill_c(struct Ints*);

int main() {
  struct Ints ints_rust;
  fill_rust(&ints_rust);
  printf(
      "Rust layout:\n"
      "a = %d (%#04x)\n"
      "b = %d (%#04x)\n"
      "c = %d (%#04x)\n\n",
      ints_rust.a, ints_rust.a, ints_rust.b, ints_rust.b, ints_rust.c,
      ints_rust.c);

  struct Ints ints_c;
  fill_c(&ints_c);
  printf(
      "C layout:\n"
      "a = %d (%#04x)\n"
      "b = %d (%#04x)\n"
      "c = %d (%#04x)\n",
      ints_c.a, ints_c.a, ints_c.b, ints_c.b, ints_c.c, ints_c.c);
}
