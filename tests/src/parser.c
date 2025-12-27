#include "tests.h"
#define ENABLE_PRIVATE_INTERFACE
#include "parser.h"
#include <stdbool.h>

typedef struct {
  bool end;
  const char* input;
  struct {
    bool ok;
    int number;
    const char* remaining;
  } expected;
} parse_number_struct;

MunitResult parse_number_test(const MunitParameter *params, void *fixture) {
  parse_number_struct tests[] = {
    {false, "52746daf",     { true, 52746, "daf" } },
    {false, "213afajfa fl", { true, 213, "afajfa fl" } },
    {false, "$4214asfdshy", { true, 0x4214a, "sfdshy" } },
    {false, "asfdshy",      { false, 0, "asfdshy" } },
    {false, "#$1372asfdshy",{ false, 0, "#$1372asfdshy" } },
    {false, "",             { false, 0, "" } },
    {false, 0,              { false, 0, 0 } },
    {true,  NULL, 0}
  };

  for (int i = 0; !tests[i].end; i++) {
    parse_number_struct test = tests[i];
    const char* s = test.input;

    long num;
    s = parse_number(s, &num);
    if (test.expected.ok) {
      munit_assert_ptr_not_null(s);
      munit_assert_ptr_not_equal(s, test.input);
      munit_assert_long(num, ==, tests[i].expected.number);
      munit_assert_char(*s, ==, *tests[i].expected.remaining);
      munit_assert_string_equal(s, tests[i].expected.remaining);
    }
    else
      munit_assert_ptr_equal(s, test.input);
  }

  return MUNIT_OK;
}
