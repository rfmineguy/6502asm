#include "tests.h"
#include "parser_util.h"
#include <ctype.h>      /* isdigit */

typedef struct {
  bool end;
  const char* input;
  struct {
    bool ok;
    int number;
    const char* remaining;
    error_parse error;
  } expected;
} parse_number_struct;

typedef struct {
  const char* input;
  struct {
    bool ok;
    expr expr;
    error_parse error;
  } expected;
} parse_expr_struct;

typedef struct {
  const char* input;
  struct {
    int len;
  } expected;
} alphalen_struct;

typedef struct {
  const char* input;
  int(*predicate)(int);
  struct {
    int len;
  } expected;
} predlen_struct;

MunitResult parse_util_parse_number_test(const MunitParameter *params, void *fixture) {
  parse_number_struct tests[] = {
    {false, "52746daf",     { true , 52746,   "daf"          , ERROR_PARSE_NONE } },
    {false, "01afd",        { true , 1,       "afd"          , ERROR_PARSE_NONE } },
    {false, "00005afd",     { true , 5,       "afd"          , ERROR_PARSE_NONE } },
    {false, "0afd",         { true , 0,       "afd"          , ERROR_PARSE_NONE } },
    {false, "000afd",       { true , 0,       "afd"          , ERROR_PARSE_NONE } },
    {false, "213afajfa fl", { true , 213,     "afajfa fl"    , ERROR_PARSE_NONE } },
    {false, "$4214asfdshy", { true , 0x4214a, "sfdshy"       , ERROR_PARSE_NONE } },
    {false, "asfdshy",      { false, 0,       "asfdshy"      , ERROR_PARSE_EXPECTED_NUMBER } },
    {false, "#$1372asfdshy",{ false, 0,       "#$1372asfdshy", ERROR_PARSE_EXPECTED_NUMBER } },
    {false, "",             { false, 0,       ""             , ERROR_PARSE_EXPECTED_NUMBER } },
    {false, "$",            { false, 0,       ""             , ERROR_PARSE_EXPECTED_NUMBER } },
    {false, "$0",           { true , 0,       ""             , ERROR_PARSE_NONE } },
    {false, 0,              { false, 0,       0              , ERROR_PARSE_NONE } },
    {true,  NULL, 0}
  };

  for (int i = 0; !tests[i].end; i++) {
    parse_number_struct test = tests[i];
    const char* s = test.input;
    munit_logf(MUNIT_LOG_INFO, "input: %s", s);

    long num;
    error_parse error;
    s = util_parse_number(s, &num, &error);
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

MunitResult parse_util_parse_expr_test(const MunitParameter *params, void *fixture) {
  parse_expr_struct tests[] = {
    {"52746daf",     { true , (expr) {.type = ET_LITERAL, .trunc_mode = TM_NONE, .data.val = 52746               }, ERROR_PARSE_NONE } },
    {"01afd",        { true , (expr) {.type = ET_LITERAL, .trunc_mode = TM_NONE, .data.val =1,                   }, ERROR_PARSE_NONE } },
    {"00005afd",     { true , (expr) {.type = ET_LITERAL, .trunc_mode = TM_NONE, .data.val =5,                   }, ERROR_PARSE_NONE } },
    {"0afd",         { true , (expr) {.type = ET_LITERAL, .trunc_mode = TM_NONE, .data.val =0,                   }, ERROR_PARSE_NONE } },
    {"000afd",       { true , (expr) {.type = ET_LITERAL, .trunc_mode = TM_NONE, .data.val =0,                   }, ERROR_PARSE_NONE } },
    {"213afajfa fl", { true , (expr) {.type = ET_LITERAL, .trunc_mode = TM_NONE, .data.val =213,                 }, ERROR_PARSE_NONE } },
    {"$4214asfdshy", { true , (expr) {.type = ET_LITERAL, .trunc_mode = TM_NONE, .data.val =0x4214a              }, ERROR_PARSE_NONE } },
    {"asfdshy",      { true , (expr) {.type = ET_LABEL  , .trunc_mode = TM_NONE, .data.label={"asfdshy", .len=7} }, ERROR_PARSE_NONE } },
    {"hello",        { true , (expr) {.type = ET_LABEL  , .trunc_mode = TM_NONE, .data.label={"hello",   .len=5} }, ERROR_PARSE_NONE } },
    {"#$1372asfdshy",{ false, (expr) {0}                                                                          , ERROR_PARSE_EXPECTED_NUMBER } },
    {"",             { false, (expr) {0}                                                                          , ERROR_PARSE_EXPECTED_NUMBER } },
    {"$",            { false, (expr) {0}                                                                          , ERROR_PARSE_EXPECTED_NUMBER } },
    {"$0",           { true , (expr) {.type = ET_LITERAL, .trunc_mode = TM_NONE, .data.val = 0                   }, ERROR_PARSE_NONE } },
    /* truncation should happen at emit time, even for literals */
    {"<$4530",       { true , (expr) {.type = ET_LITERAL, .trunc_mode = TM_LOW,  .data.val = 0x4530              }, ERROR_PARSE_NONE } },
    {">$4530",       { true , (expr) {.type = ET_LITERAL, .trunc_mode = TM_HIGH, .data.val = 0x4530              }, ERROR_PARSE_NONE } },
    {"<$45",         { false, (expr) {0}                                                                          , ERROR_PARSE_NUMBER_OUT_OF_RANGE } },
    {">$30",         { false, (expr) {0}                                                                          , ERROR_PARSE_NUMBER_OUT_OF_RANGE } },
    {">hello",       { true , (expr) {.type = ET_LABEL  , .trunc_mode = TM_HIGH, .data.label={"hello", .len=5}   }, ERROR_PARSE_NONE } },
    {"<hello",       { true , (expr) {.type = ET_LABEL  , .trunc_mode = TM_LOW,  .data.label={"hello", .len=5}   }, ERROR_PARSE_NONE } },
    /* truncating nothing */
    {"<",            { false, (expr) {0}                                                                          , ERROR_PARSE_EXPECTED_NUMBER } },
    {0,              { false, (expr) {0}                                                                          , ERROR_PARSE_NUMBER_OUT_OF_RANGE } },
    {NULL}
  };

  for (int i = 0; tests[i].input; i++) {
    parse_expr_struct test = tests[i];
    const char* s = test.input;
    munit_logf(MUNIT_LOG_INFO, "input: %s", s);

    expr expr;
    error_parse error;
    s = util_parse_expr(s, &expr, &error);
    munit_logf(MUNIT_LOG_INFO, "s: %s", s);
    if (test.expected.ok) {
      munit_assert_int(error, ==, test.expected.error);
      munit_assert_ptr_not_null(s);
      munit_assert_ptr_not_equal(s, test.input);
      munit_assert_long(expr.trunc_mode, ==, tests[i].expected.expr.trunc_mode);
      munit_assert_long(expr.type      , ==, tests[i].expected.expr.type);
      switch (expr.type) {
        case ET_LABEL:
          munit_assert_long(expr.data.label.len, ==, tests[i].expected.expr.data.label.len);
          munit_assert_memory_equal(expr.data.label.len, expr.data.label.s, tests[i].expected.expr.data.label.s);
          break;
        case ET_LABEL_REL:
          break;
        case ET_LITERAL:
          munit_assert_long(expr.data.val, ==, tests[i].expected.expr.data.val);
          break;
      }
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

static int pred_label(int ch) {
  return ch == '_' || isalpha(ch);
}

MunitResult parse_util_predlen_test(const MunitParameter *params, void *fixture) {
  predlen_struct tests[] = {
    { "5348614", isdigit,    { 7 } },
    { "53",      isdigit,    { 2 } },
    { "hello",   isalpha,    { 5 } },
    { "hel245ad",isalnum,    { 8 } },
    { "hel_ad",  pred_label, { 6 } },
    NULL
  };

  for (int i = 0; tests[i].input != NULL; i++) {
    predlen_struct test = tests[i];
    munit_assert_int(test.expected.len, ==, util_predlen(test.input, test.predicate));
  }
  return MUNIT_OK;
}
