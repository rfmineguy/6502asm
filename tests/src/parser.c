#include "tests.h"
#include "parser.h"
#include <stdbool.h>

typedef struct {
  const char* input;
  struct {
    bool ok;
    int number;
    const char* remaining;
  } expected;
} parse_number_struct;

MunitResult parse_number_test(const MunitParameter *params, void *fixture) {
  parse_number_struct tests[] = {
    {"52746daf",     { true, 52746, "daf" } },
    {"213afajfa fl", { true, 213, "afajfa fl" } },
    {"$4214asfdshy", { true, 0x4214a, "sfdshy" } },
    {"asfdshy",      { false, 0, "asfdshy" } },
    {"#$1372asfdshy",{ false, 0, "#$1372asfdshy" } },
    {NULL, 0}
  };

  int i = 0;
  for (int i = 0; tests[i].input != NULL; i++) {
    parse_number_struct test = tests[i];
    const char* s = test.input;

    long num;
    s = parse_number(s, &num);
    if (test.expected.ok)
      munit_assert_ptr_not_equal(s, test.input);
    else
      munit_assert_ptr_equal(s, test.input);
    munit_assert_long(num, ==, tests[i].expected.number);
    munit_assert_char(*s, ==, *tests[i].expected.remaining);
    munit_assert_string_equal(s, tests[i].expected.remaining);
  }

  return MUNIT_OK;
}
