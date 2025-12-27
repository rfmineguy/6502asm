#include "tests.h"
#define ENABLE_PRIVATE_INTERFACE
#include "parser.h"
#include <stdbool.h>
#include <stdio.h>

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

typedef struct {
  const char* input;
  struct {
    int addr;
    const char* remaining;
  } expected;
} org_struct;

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

MunitResult parser_alphalen_test(const MunitParameter *params, void *fixture) {
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
    int len = alphalen(test.input);
    munit_assert_long(len, ==, tests[i].expected.len);
  }

  return MUNIT_OK;
}

MunitResult parser_isnewline_test(const MunitParameter *params, void *fixture) {
  int len;
  munit_assert_true(isnewline("\n", &len));
  munit_assert_int(len, ==, 1);
  munit_assert_true(isnewline("\r", &len));
  munit_assert_int(len, ==, 1);
  munit_assert_true(isnewline("\r\n", &len));
  munit_assert_int(len, ==, 2);

  char buff[5];
  for (char c = 'a'; c <= 'z'; c++) {
    buff[0] = c;
    buff[1] = 0;
    munit_assert_false(isnewline(buff, &len));
  }
  return MUNIT_OK;
}

MunitResult parser_parse_org_test(const MunitParameter *params, void *fixture) {
  org_struct tests[] = {
    { "org $0800\nad",     { 0x800,  "\nad" } },
    { "org $1800\nlda #x", { 0x1800, "\nlda #x" } },
    { "org $40\nlda #x",   { 0x40,   "\nlda #x" } },
    { "org $5341\nlda #x", { 0x5341, "\nlda #x" } },
    { NULL, 0 }
  };

  for (int i = 0; tests[i].input != NULL; i++) {
    org_struct test = tests[i];
    const char* code = test.input;
    long addr;
    const char* c = parse_org(code, &addr);
    munit_assert_int(addr, ==, test.expected.addr);
    munit_assert_char(*c, ==, '\n');
    munit_assert_string_equal(c, test.expected.remaining);
  }

  return MUNIT_OK;
}
