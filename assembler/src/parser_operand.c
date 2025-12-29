#include "parser_operand.h"
#include "parser_util.h"
#include <limits.h>
#include <stdio.h>

// immediate
const char* parse_op_immediate(const char* cursor, long* val) {
  const char* orig = cursor;

  while (*cursor == ' ') cursor++;
  if (*cursor != '#') {
    return orig;
  }
  cursor++;

  const char* new_cursor;
  if ((new_cursor = util_parse_number(cursor, val)) == cursor) {
    fprintf(stderr, "Expected number, got %c\n", *cursor);
    return orig;
  }
  cursor = new_cursor;

  if (*val < 0 || *val > UCHAR_MAX) {
    fprintf(stderr, "Immediate value too big\n");
    return orig;
  }
  return new_cursor;
}

// zp
const char* parse_op_zp(const char* cursor, long* val) {
  const char* orig = cursor;

  while (*cursor == ' ') cursor++;

  const char* new_cursor;
  if ((new_cursor = util_parse_number(cursor, val)) == cursor) {
    return orig;
  }
  cursor = new_cursor;

  if (*val < 0 || *val > UCHAR_MAX) {
    return orig;
  }
  return cursor;
}

// zp,X
const char* parse_op_zpx(const char* cursor, long* val) {
  const char* orig = cursor;

  while (*cursor == ' ') cursor++;
  
  const char* new_cursor;
  if ((new_cursor = util_parse_number(cursor, val)) == cursor) {
    return orig;
  }
  cursor = new_cursor;

  if (*val < 0 || *val > UCHAR_MAX) {
    return orig;
  }

  while (*cursor == ' ') cursor++;
  if (*cursor != ',') {
    return orig;
  }
  cursor++;

  while (*cursor == ' ') cursor++;
  if (*cursor != 'X') {
    return orig;
  }
  cursor++;
  return cursor;
}

// absolute
const char* parse_op_absolute(const char* cursor, long* val) {
  const char* orig = cursor;

  while (*cursor == ' ') cursor++;
  
  const char* new_cursor;
  if ((new_cursor = util_parse_number(cursor, val)) == cursor) {
    return orig;
  }
  cursor = new_cursor;

  if (*val < 0 || *val > USHRT_MAX) {
    return orig;
  }

  return new_cursor;
}
