#include "tests.h"
#include "parser_util.h"

typedef struct {
  bool end;
  const char* input;
  struct {
    bool ok;
    int number;
    const char* remaining;
  } expected;
} parse_number_struct;

typedef struct {
  const char* input;
  struct {
    int len;
  } expected;
} alphalen_struct;

MunitResult parse_util_parse_number_test(const MunitParameter *params, void *fixture) {
  parse_number_struct tests[] = {
    {false, "52746daf",     { true, 52746, "daf" } },
    {false, "01afd",        { true, 1, "afd" } },
    {false, "00005afd",     { true, 5, "afd" } },
    {false, "0afd",         { true, 0, "afd" } },
    {false, "000afd",       { true, 0, "afd" } },
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
    // munit_logf(MUNIT_LOG_INFO, "input: %s", s);

    long num;
    s = util_parse_number(s, &num);
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

MunitResult parse_util_alphalen_test(const MunitParameter *params, void *fixture) {
  alphalen_struct tests[] = {
    { "hello",   { 5 } },
    { "general", { 7 } },
    { "akjfskd,jadkmfhamhfsikjaduj", { 7 } },
    { "akjfskdjadkmf83hamhfsikjaduj", { 13 } },
    { "2kjfskdjadkmf", { 0 } },
    { "akjfskdjadkmf#3hamhfsikjaduj", { 13 } },
    { "2kjfskdjadkmf", { 0 } },
    { NULL, 0 }
  };

  for (int i = 0; tests[i].input != NULL; i++) {
    alphalen_struct test = tests[i];
    int len = util_alphalen(test.input);
    munit_assert_long(len, ==, tests[i].expected.len);
  }

  return MUNIT_OK;
}

MunitResult parse_util_isnewline_test(const MunitParameter *params, void *fixture) {
  int len;
  munit_assert_true(util_isnewline("\n", &len));
  munit_assert_int(len, ==, 1);
  munit_assert_true(util_isnewline("\r", &len));
  munit_assert_int(len, ==, 1);
  munit_assert_true(util_isnewline("\r\n", &len));
  munit_assert_int(len, ==, 2);

  char buff[5];
  for (char c = 'a'; c <= 'z'; c++) {
    buff[0] = c;
    buff[1] = 0;
    munit_assert_false(util_isnewline(buff, &len));
  }
  return MUNIT_OK;
}
