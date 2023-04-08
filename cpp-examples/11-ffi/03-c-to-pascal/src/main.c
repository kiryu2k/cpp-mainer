#include <stdint.h>
#include <stdio.h>

int32_t MATH_$$_ADD$LONGINT$LONGINT$$LONGINT(int32_t, int32_t);
int32_t math_add(int32_t, int32_t);

int32_t MYSTRING_$$_MYLEN$SHORTSTRING$$LONGINT(const char*);
int32_t mylen(const char*);

int main() {
  const int32_t a = 40;
  const int32_t b = 2;
  const int32_t x = MATH_$$_ADD$LONGINT$LONGINT$$LONGINT(a, b);
  const int32_t y = math_add(a, b);
  printf("%d + %d = %d\n", a, b, x);
  printf("%d + %d = %d\n", a, b, y);

  const char* str = "hello";
  int32_t len = MYSTRING_$$_MYLEN$SHORTSTRING$$LONGINT(str);
  printf("len(%s) = %d\n", str, len);

  len = mylen(str);
  printf("len(%s) = %d\n", str, len);
}
