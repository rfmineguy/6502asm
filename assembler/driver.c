#include <stdio.h>
#include "../include/parser.h"

int main() {
  const char* s = "asfdshy";
  long num;
  s = parse_number(s, &num);

  printf("s = %s\n", s);
  printf("num = %lu\n", num);
}
