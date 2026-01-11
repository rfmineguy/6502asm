#include "parser_label.h"
#include "parser_util.h"
#include <stdio.h>
#include <assert.h>
#include <ctype.h>

static int pred_label(int ch) {
  return ch == '_' || isalpha(ch);
}

const char* label_parse_named_label(const char* cursor, label* label_out, error_parse* error) {
  assert(label_out && "Must call parse_label with non null label_out");
  assert(error     && "Must call parse_label with non null error");
  const char* orig = cursor;

  while (*cursor && *cursor == ' ') cursor++;

  const char* start = cursor;
  int count = util_predlen(cursor, pred_label);
  if (count == 0) {
    *error = ERROR_PARSE_EXPECTED_NAME;
    return orig;
  }

  cursor += count;

  while (*cursor && *cursor == ' ') cursor++;
  if (*cursor != ':') {
    *error = ERROR_PARSE_EXPECTED_COLON;
    return orig;
  }
  cursor++;

  label_out->type = lt_named;
  label_out->data.named.s = start;
  label_out->data.named.len = count;
  *error = ERROR_PARSE_NONE;
  return cursor;
}

const char* label_parse_unnamed_label(const char* cursor, label* label_out, error_parse* error) {
  assert(label_out && "Must call parse_label with non null label_out");
  assert(error     && "Must call parse_label with non null error");
  const char* orig = cursor;

  while (*cursor && *cursor == ' ') cursor++;

  int len;
  if ((len = util_predlen(cursor, pred_label)) > 0) {
    fprintf(stderr, "Unexpected name: '%.*s'\n", len, cursor);
    *error = ERROR_PARSE_UNEXPECTED_NAME;
    return orig;
  }

  if (*cursor != ':') {
    fprintf(stderr, "Expect colon, got '%c'\n", *cursor);
    *error = ERROR_PARSE_EXPECTED_COLON;
    return orig;
  }
  cursor++;

  label_out->type = lt_unnamed;
  *error = ERROR_PARSE_NONE;
  return cursor;
}
