#include "parser_directive.h"
#include "parser_util.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

// cursor expected to be on address
const char* dir_parse_org(const char* cursor, uint16_t* addr_out) {
  assert(addr_out && "Must call parse_org with non null addr_out");
  const char* orig = cursor;
  // skip spaces
  while (cursor && *cursor == ' ') cursor++;
  // parse number
  long val;
  const char* new_cursor = util_parse_number(cursor, &val);
  if (new_cursor == cursor) {
    printf("Failed to read number in org\n");
    return orig;
  }
  if (val < 0 || val > SHRT_MAX) {
    fprintf(stderr, "org: addr out of range\n");
    return orig;
  }
  *addr_out = val & 0xffff;
  return new_cursor;
}
