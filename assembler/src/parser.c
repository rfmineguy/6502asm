#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#define ENABLE_PRIVATE_INTERFACE
#include "parser.h"

const char* parse_number(const char* cursor, long* num_out) {
  assert(num_out && "Must call parse_number with non null num_out");
  const char* orig = cursor;
  if (cursor == NULL) return NULL;
  if (*cursor == 0) return orig;
  int base = 10;
  if (*cursor == '$') {
    base = 16;
    cursor++;
  }
  char* end;
  *num_out = strtol(cursor, &end, base);
  if (*num_out == 0) {
    // TODO: These errors should be propogated to some sort of report system
    // if (errno == EINVAL || errno == ERANGE) {
    //   fprintf(stderr, "parse_number: %*s '%s'\n", 9, cursor, strerror(errno));
    // }
    return orig;
  }
  return end;
}

int alphalen(const char* cursor) {
  int len = 0;
  while (isalpha(*(cursor++))) len++;
  return len;
}

bool isnewline(const char* s, int* len) {
  assert(len && "Must call isnewline with non null len");
  if (*s == '\r' && *(s + 1) == '\n') {
    *len = 2;
    return true;
  }
  if (*s == '\n' || *s == '\r') {
    *len = 1;
    return true;
  }
  return false;
}
