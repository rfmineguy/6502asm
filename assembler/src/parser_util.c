#include "parser_util.h"
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

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
  errno = 0;
  *num_out = strtol(cursor, &end, base);
  if (end == cursor) {
    if (errno == EINVAL || errno == ERANGE) {
      fprintf(stderr, "Failed to convert to number (reason: %s)\n", strerror(errno));
      fprintf(stderr, "%-5s\n", cursor);
      return orig;
    }
  }
  errno = ERROR_PARSE_NONE;
  return end;
}

const char* util_parse_expr(const char* cursor, expr* exp_out, error_parse* error) {
  assert(exp_out && "Must call parse_expr with non null exp_out");
  assert(error && "Must call parse_expr with non null error");
  const char* orig = cursor;

  *error = ERROR_PARSE_NONE;
  if (cursor == NULL) {
      *error = ERROR_PARSE_NULL_CURSOR;
      return NULL;
  }
  if (*cursor == 0) {
      *error = ERROR_PARSE_EXPECTED_NUMBER;
      return orig;
  }

  int base = 10, len;
  truncate_mode trunc_mode = TM_NONE;
  if      (*cursor == '>') { trunc_mode = TM_HIGH; cursor++; }
  else if (*cursor == '<') { trunc_mode = TM_LOW;  cursor++; }
  if (*cursor == '$') { base = 16;            cursor++; }
  if ((len = util_alphalen(cursor)) > 0) {
    // label next of length 'len'
    exp_out->type = ET_LABEL;
    exp_out->trunc_mode = trunc_mode;
    exp_out->data.label.s = cursor;
    exp_out->data.label.len = len;
    return cursor + len;
  }
  else {
    if (base == 10 && !isdigit(*cursor)) {
      *error = ERROR_PARSE_EXPECTED_NUMBER;
      return orig;
    }
    if (base == 16 && !ishex(*cursor)) {
      *error = ERROR_PARSE_EXPECTED_NUMBER;
      return orig;
    }
    exp_out->type = ET_LITERAL;
    exp_out->trunc_mode = trunc_mode;
    errno = 0;
    char* end;

    // parse out the number
    exp_out->data.val = strtol(cursor, &end, base);
    if (end == cursor) {
      if (errno == EINVAL || errno == ERANGE) {
        fprintf(stderr, "Failed to convert to number (reason: %s)\n", strerror(errno));
        fprintf(stderr, "%-5s\n", cursor);
        return orig;
      }
    }
    // truncating single bytes is not possible
    if (exp_out->trunc_mode != TM_NONE && exp_out->data.val <= UCHAR_MAX) {
      *error = ERROR_PARSE_NUMBER_OUT_OF_RANGE;
      return orig;
    }
    return end;
  }
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
