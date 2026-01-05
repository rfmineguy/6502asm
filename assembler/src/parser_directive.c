#include "parser_directive.h"
#include "parser_util.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

// cursor expected to be on address
// .org $4536
const char* dir_parse_org(const char* cursor, uint16_t* addr_out, error_parse* error) {
  assert(addr_out && "Must call parse_org with non null addr_out");
  const char* orig = cursor;
  // skip spaces
  while (cursor && *cursor == ' ') cursor++;
  // parse number
  long val;
  const char* new_cursor = util_parse_number(cursor, &val);
  if (new_cursor == cursor) {
    fprintf(stderr, "Failed to read number in org\n");
    *error = ERROR_PARSE_EXPECTED_NUMBER;
    return orig;
  }
  if (val < 0 || val > SHRT_MAX) {
    fprintf(stderr, "org: addr out of range\n");
    *error = ERROR_PARSE_NUMBER_OUT_OF_RANGE;
    return orig;
  }
  *addr_out = val & 0xffff;
  *error = ERROR_PARSE_NONE;
  return new_cursor;
}

// cursor expected to be on first number (if it is present)
// .byte 42, 51, $53, 21, 95
const char* dir_parse_byte(const char* cursor, uint8_t bytes_out[MAX_BYTES_IN_BYTE_DIR], int* bytes_count_out, error_parse* error) {
  assert(bytes_out && "bytes_out must not be null");
  assert(bytes_count_out && "bytes_count_out must not be null");
  const char* orig = cursor;
  const char* new_cursor;
  *bytes_count_out = 0;

  // skip spaces
  while (cursor && *cursor == ' ') cursor++;

  // parse number(s)
  // [0-9]+{, [0-9]+}
  do {
    long val;
    // bool is_decimal = false;
    if (*cursor == '#') {
      // is_decimal = true;
      cursor++;
    }
    new_cursor = util_parse_number(cursor, &val);
    if (new_cursor == cursor) {
      *error = ERROR_PARSE_EXPECTED_NUMBER;
      return orig;
    }
    if (val < 0 || val > UCHAR_MAX) {
      *error = ERROR_PARSE_NUMBER_OUT_OF_RANGE;
      return orig;
    }
    if (*bytes_count_out >= MAX_BYTES_IN_BYTE_DIR) {
      *error = ERROR_PARSE_BYTE_DIR_TOO_LONG;
      return orig;
    }
    bytes_out[(*bytes_count_out)++] = (uint8_t)val;
    cursor = new_cursor;
    // skip spaces
    while (cursor && *cursor == ' ') cursor++;
    if (*cursor != ',') break;
    cursor++;

    // skip spaces
    while (cursor && *cursor == ' ') cursor++;

  } while (1);

  *error = ERROR_PARSE_NONE;
  return new_cursor;
}
const char* dir_parse_directive(const char* cursor, directive* dir_out, error_parse* error) {
  assert(dir_out && "Must call parse_directive with non null dir_out");
  const char* orig = cursor;
  const char* new_cursor;
  if (strncmp(cursor, "org", 3) == 0) {
    dir_out->type = dt_org;
    cursor += 3;
    while (*cursor == ' ') cursor++;
    if ((new_cursor = dir_parse_org(cursor, &dir_out->data.org.addr, error)) == cursor) {
      return orig;
    }
    return new_cursor;
  }
  if (strncmp(cursor, "byte", 4) == 0) {
    dir_out->type = dt_byte;
    cursor += 4;
    while (*cursor == ' ') cursor++;
    if ((new_cursor = dir_parse_byte(cursor, dir_out->data.byte.bytes, &dir_out->data.byte.bytes_count, error)) == cursor) {
      return orig;
    }
    return new_cursor;
  }
  assert(0 && "directive not implemented");
  return orig;
}
