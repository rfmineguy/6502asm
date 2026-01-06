#include "parser_operand.h"
#include "parser_errors.h"
#include "parser_util.h"
#include <limits.h>
#include <stdio.h>
#include <assert.h>

// accumulator
const char* parse_op_accumulator(const char* cursor, error_parse* error) {
  assert(error && "error must be non null");
  const char* orig = cursor;

  while (*cursor == ' ') cursor++;
  if (*cursor != 'A') {
    *error = ERROR_PARSE_EXPECTED_ACCUMULATOR;
    return orig;
  }
  *error = ERROR_PARSE_NONE;
  return cursor + 1;
}

// immediate
const char* parse_op_immediate(const char* cursor, long* val, error_parse* error) {
  assert(val && "val must be non null");
  assert(error && "error must be non null");
  const char* orig = cursor;

  while (*cursor == ' ') cursor++;
  if (*cursor != '#') {
    *error = ERROR_PARSE_EXPECTED_HASH;
    return orig;
  }
  cursor++;

  const char* new_cursor;
  if ((new_cursor = util_parse_number(cursor, val, error)) == cursor) {
    fprintf(stderr, "Expected number, got %c\n", *cursor);
    return orig;
  }

  if (*val < 0 || *val > UCHAR_MAX) {
    fprintf(stderr, "Immediate value too big\n");
    *error = ERROR_PARSE_NUMBER_OUT_OF_RANGE;
    return orig;
  }
  *error = ERROR_PARSE_NONE;
  return new_cursor;
}

// zp
const char* parse_op_zp(const char* cursor, long* val, error_parse* error) {
  assert(val && "val must be non null");
  assert(error && "error must be non null");
  const char* orig = cursor;

  while (*cursor == ' ') cursor++;

  const char* new_cursor;
  if ((new_cursor = util_parse_number(cursor, val, error)) == cursor) {
    return orig;
  }
  cursor = new_cursor;

  if (*val < 0 || *val > UCHAR_MAX) {
    *error = ERROR_PARSE_NUMBER_OUT_OF_RANGE;
    return orig;
  }
  *error = ERROR_PARSE_NONE;
  return cursor;
}

// zp,X
const char* parse_op_zpx(const char* cursor, long* val, error_parse* error) {
  assert(val && "val must be non null");
  assert(error && "error must be non null");
  const char* orig = cursor;

  while (*cursor == ' ') cursor++;
  
  const char* new_cursor;
  if ((new_cursor = util_parse_number(cursor, val, error)) == cursor) {
    return orig;
  }
  cursor = new_cursor;

  if (*val < 0 || *val > UCHAR_MAX) {
    *error = ERROR_PARSE_NUMBER_OUT_OF_RANGE;
    return orig;
  }

  while (*cursor == ' ') cursor++;
  if (*cursor != ',') {
    *error = ERROR_PARSE_EXPECTED_COMMA;
    return orig;
  }
  cursor++;

  while (*cursor == ' ') cursor++;
  if (*cursor != 'X') {
    *error = ERROR_PARSE_EXPECTED_X;
    return orig;
  }
  *error = ERROR_PARSE_NONE;
  cursor++;
  return cursor;
}

// absolute
const char* parse_op_absolute(const char* cursor, long* val, error_parse* error) {
  assert(val && "val must be non null");
  assert(error && "error must be non null");
  const char* orig = cursor;

  while (*cursor == ' ') cursor++;
  
  const char* new_cursor;
  if ((new_cursor = util_parse_number(cursor, val, error)) == cursor) {
    return orig;
  }
  cursor = new_cursor;

  if (*val < 0 || *val > USHRT_MAX) {
    *error = ERROR_PARSE_NUMBER_OUT_OF_RANGE;
    return orig;
  }
  *error = ERROR_PARSE_NONE;
  return new_cursor;
}

// absolute, X
const char* parse_op_absolutex(const char* cursor, long* val, error_parse* error) {
  assert(val && "val must be non null");
  assert(error && "error must be non null");
  const char* orig = cursor;

  while (*cursor == ' ') cursor++;
  const char* new_cursor;
  if ((new_cursor = util_parse_number(cursor, val, error)) == cursor) {
    return orig;
  }
  cursor = new_cursor;
  if (*val < 0 || *val > USHRT_MAX) {
    *error = ERROR_PARSE_NUMBER_OUT_OF_RANGE;
    return orig;
  }
  cursor = new_cursor;
  while (*cursor == ' ') cursor++;
  if (*cursor != ',') {
    *error = ERROR_PARSE_EXPECTED_COMMA;
    return orig;
  }
  cursor++;
  while (*cursor == ' ') cursor++;
  if (*cursor != 'X') {
    *error = ERROR_PARSE_EXPECTED_X;
    return orig;
  }
  *error = ERROR_PARSE_NONE;
  cursor++;
  return cursor;
}

// absolute, Y
const char* parse_op_absolutey(const char* cursor, long* val, error_parse* error) {
  assert(val && "val must be non null");
  assert(error && "error must be non null");
  const char* orig = cursor;

  while (*cursor == ' ') cursor++;
  const char* new_cursor;
  if ((new_cursor = util_parse_number(cursor, val, error)) == cursor) {
    return orig;
  }
  cursor = new_cursor;
  if (*val < 0 || *val > USHRT_MAX) {
    *error = ERROR_PARSE_NUMBER_OUT_OF_RANGE;
    return orig;
  }
  cursor = new_cursor;
  while (*cursor == ' ') cursor++;
  if (*cursor != ',') {
    *error = ERROR_PARSE_EXPECTED_COMMA;
    return orig;
  }
  cursor++;
  while (*cursor == ' ') cursor++;
  if (*cursor != 'Y') {
    *error = ERROR_PARSE_EXPECTED_Y;
    return orig;
  }
  *error = ERROR_PARSE_NONE;
  cursor++;
  return cursor;
}

// (indirect, X)
const char* parse_op_indirectx(const char* cursor, long* val, error_parse* error) {
  assert(val && "val must be non null");
  assert(error && "error must be non null");
  const char* orig = cursor;

  while (*cursor == ' ') cursor++;
  if (*cursor != '(') {
    *error = ERROR_PARSE_EXPECTED_LPAREN;
    return orig;
  }
  cursor++;
  const char* new_cursor;
  if ((new_cursor = util_parse_number(cursor, val, error)) == cursor) {
    return orig;
  }
  cursor = new_cursor;

  if (*val < 0 || *val > UCHAR_MAX) {
    *error = ERROR_PARSE_NUMBER_OUT_OF_RANGE;
    return orig;
  }

  while (*cursor == ' ') cursor++;
  if (*cursor != ',') {
    *error = ERROR_PARSE_EXPECTED_COMMA;
    return orig;
  }
  cursor++;

  while (*cursor == ' ') cursor++;
  if (*cursor != 'X') {
    *error = ERROR_PARSE_EXPECTED_X;
    return orig;
  }
  cursor++;
  if (*cursor != ')') {
    *error = ERROR_PARSE_EXPECTED_RPAREN;
    return orig;
  }
  cursor++;
  *error = ERROR_PARSE_NONE;

  return cursor;
}

// (indirect), Y
const char* parse_op_indirecty(const char* cursor, long* val, error_parse* error) {
  assert(val && "val must be non null");
  assert(error && "error must be non null");
  const char* orig = cursor;

  while (*cursor == ' ') cursor++;
  if (*cursor != '(') {
    *error = ERROR_PARSE_EXPECTED_LPAREN;
    return orig;
  }
  cursor++;
  const char* new_cursor;
  if ((new_cursor = util_parse_number(cursor, val, error)) == cursor) {
    return orig;
  }
  cursor = new_cursor;

  if (*val < 0 || *val > UCHAR_MAX) {
    *error = ERROR_PARSE_NUMBER_OUT_OF_RANGE;
    return orig;
  }

  while (*cursor == ' ') cursor++;
  if (*cursor !=  ')') {
    *error = ERROR_PARSE_EXPECTED_RPAREN;
    return orig;
  }
  cursor++;

  while (*cursor == ' ') cursor++;
  if (*cursor != ',') {
    *error = ERROR_PARSE_EXPECTED_COMMA;
    return orig;
  }
  cursor++;

  while (*cursor == ' ') cursor++;
  if (*cursor != 'Y') {
    *error = ERROR_PARSE_EXPECTED_Y;
    return orig;
  }

  *error = ERROR_PARSE_NONE;
  cursor ++;
  return cursor;
}
