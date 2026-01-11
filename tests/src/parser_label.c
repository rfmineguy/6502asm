#include "tests.h"
#include "parser_label.h"
#include "parser_errors.h"
#include <stdbool.h>

typedef struct {
  const char* input;
  struct {
    bool ok;
    string_view s;
    int error;
  } expected;
} parse_label_named_struct;

typedef struct {
  const char* input;
  struct {
    bool ok;
    int error;
  } expected;
} parse_label_unnamed_struct;

MunitResult parse_label_named_test(const MunitParameter *params, void *fixture) {
#define LONG "this_is_a_long_label_with_underscores"
#define LONG_LEN (sizeof(LONG) - 1) // exclude null term from length here
  parse_label_named_struct tests[] = {
    { "hello:", { true,  "hello", 5,        ERROR_PARSE_NONE } },
    { "hello",  { false, ""     , 0,        ERROR_PARSE_EXPECTED_COLON } },
    { "b:",     { true,  "b"    , 1,        ERROR_PARSE_NONE } },
    { "b",      { false, ""     , 0,        ERROR_PARSE_EXPECTED_COLON } },
    { "a_b_c:", { true,  "a_b_c", 5,        ERROR_PARSE_NONE } },
    { LONG ":", { true,  LONG   , LONG_LEN, ERROR_PARSE_NONE } },
    /* this is an unnamed label, and should fail if parsed as a named label */
    { ":",      { false, ""     , -1      , ERROR_PARSE_NONE } },
    { NULL }
  };

  for (int i = 0; tests[i].input; i++) {
    parse_label_named_struct test = tests[i];
    label val;
    error_parse error;
    munit_logf(MUNIT_LOG_INFO, "'%s', '%*.s'", test.input, (int)test.expected.s.len, test.expected.s.s);
    const char* new_cursor = label_parse_named_label(test.input, &val, &error);
    if (test.expected.ok) {
      munit_assert_int(error, ==, ERROR_PARSE_NONE);
      munit_assert_ptr_not_equal(tests[i].input, new_cursor);
      munit_assert_int(val.type, ==, lt_named);
      munit_assert_int(val.data.named.len, ==, tests[i].expected.s.len);
      munit_assert_memory_equal(val.data.named.len, val.data.named.s, tests[i].expected.s.s);
    }
    else
      munit_assert_ptr_equal(tests[i].input, new_cursor);
  }
  return MUNIT_OK;
}

MunitResult parse_label_unnamed_test(const MunitParameter *params, void *fixture) {
  parse_label_unnamed_struct tests[] = {
    { ":",    { true,  ERROR_PARSE_NONE            }},
    { "faf:", { false, ERROR_PARSE_UNEXPECTED_NAME }},
    { "agda", { false, ERROR_PARSE_UNEXPECTED_NAME }},
    { "",     { false, ERROR_PARSE_EXPECTED_COLON  }},
    { NULL }
  };

  for (int i = 0; tests[i].input; i++) {
    parse_label_unnamed_struct test = tests[i];
    label val;
    error_parse error;
    const char* new_cursor = label_parse_unnamed_label(test.input, &val, &error);
    munit_assert_int(tests[i].expected.error, ==, error);
    if (test.expected.ok) {
      munit_assert_ptr_not_equal(tests[i].input, new_cursor);
      munit_assert_int(val.type, ==, lt_unnamed);
    }
    else
      munit_assert_ptr_equal(tests[i].input, new_cursor);
  }
  return MUNIT_OK;
}
