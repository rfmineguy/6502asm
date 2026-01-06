#include "parser_util.h"
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

static int ishex(char c) {
    c = tolower(c);
    return isdigit(c) || (c >= 'a' && c <= 'f');
}

const char* util_parse_number(const char* cursor, long* num_out, error_parse* error) {
  assert(num_out && "Must call parse_number with non null num_out");
  assert(error && "Must call parse_number with non null error");
  const char* orig = cursor;
  if (cursor == NULL) {
      *error = ERROR_PARSE_NULL_CURSOR;
      return NULL;
  }
  if (*cursor == 0) {
      *error = ERROR_PARSE_EXPECTED_NUMBER;
      return orig;
  }
  int base = 10;
  if (*cursor == '$') {
    base = 16;
    cursor++;
  }
  char* end;

  if (base == 10 && !isdigit(*cursor)) {
      *error = ERROR_PARSE_EXPECTED_NUMBER;
      return orig;
  }
  if (base == 16 && !ishex(*cursor)) {
      *error = ERROR_PARSE_EXPECTED_NUMBER;
      return orig;
  }

  // NOTE: Is this ok to do? The idea is that it is setting errno to something that strtol
  //  will never set it to, thus allowing me to accurately judge if strtol set errno internally
  *num_out = strtol(cursor, &end, base);
  if (*num_out == 0) {
    if (errno == EINVAL || errno == ERANGE) {
      fprintf(stderr, "Failed to convert to number (reason: %s)\n", strerror(errno));
      fprintf(stderr, "%-5s\n", cursor);
      return orig;
    }
  }
  errno = ERROR_PARSE_NONE;
  return end;
}

int util_alphalen(const char* cursor) {
  int len = 0;
  while (isalpha(*(cursor++))) len++;
  return len;
}

bool util_isnewline(const char* s, int* len) {
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

int util_predlen(const char *cursor, int(*predicate)(int)) {
  assert(predicate && "Must call predlen with non null predicated");
  const char* orig = cursor;
  while (*cursor && predicate(*cursor)) cursor++;
  return cursor - orig;
}
