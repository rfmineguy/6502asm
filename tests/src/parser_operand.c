#include "tests.h"
#include "parser_operand.h"
#include <stdbool.h>

typedef struct {
  const char* input;
  struct {
    bool ok;
    long val;
  } expected;
} parse_operand_struct;

MunitResult parse_operand_immediate_test(const MunitParameter *params, void *fixture) {
  parse_operand_struct tests[] = {
    { "#0",     { true,  0  } },
    { "#000",   { true,  0  } },
    { "#00010", { true,  10 } },
    { "#00020", { true,  20 } },
    { "#00030", { true,  30 } },
    { "#00040", { true,  40 } },
    { "#00050", { true,  50 } },
    { "#00060", { true,  60 } },
    { "#00070", { true,  70 } },
    { "#00080", { true,  80 } },
    { "#256",   { false, -1 } },
    { NULL }
  };

  for (int i = 0; tests[i].input; i++) {
    parse_operand_struct test = tests[i];
    long val;
    const char* new_cursor = parse_op_immediate(test.input, &val);
    if (test.expected.ok) {
      munit_assert_ptr_not_equal(tests[i].input, new_cursor);
      munit_assert_int(val, ==, tests[i].expected.val);
    }
    else
      munit_assert_ptr_equal(tests[i].input, new_cursor);
  }
  return MUNIT_OK;
}

MunitResult parse_operand_zp_test(const MunitParameter *params, void *fixture) {
  parse_operand_struct tests[] = {
    { "$0",      { true,  0x00 } },
    { "$000000", { true,  0x00 } },
    { "$00010",  { true,  0x10 } },
    { "$00020",  { true,  0x20 } },
    { "$00030",  { true,  0x30 } },
    { "$00040",  { true,  0x40 } },
    { "$00050",  { true,  0x50 } },
    { "$00060",  { true,  0x60 } },
    { "$00070",  { true,  0x70 } },
    { "$00080",  { true,  0x80 } },
    { "$000256", { false, -1   } },
    { "$100256", { false, -1   } },
    { NULL }
  };

  for (int i = 0; tests[i].input; i++) {
    parse_operand_struct test = tests[i];
    long val;
    const char* new_cursor = parse_op_zp(test.input, &val);
    if (test.expected.ok) {
      munit_assert_ptr_not_equal(tests[i].input, new_cursor);
      munit_assert_int(val, ==, tests[i].expected.val);
    }
    else
      munit_assert_ptr_equal(tests[i].input, new_cursor);
  }
  return MUNIT_OK;
}
MunitResult parse_operand_zpx_test(const MunitParameter *params, void *fixture) {
  parse_operand_struct tests[] = {
    { "$0, X",       { true,  0x00 } },
    { "$000000, X",  { true,  0x00 } },
    { "$00000, Y",   { false, 0x60 } },
    { "$00010, X",   { true,  0x10 } },
    { "$00010, Y",   { false, 0x60 } },
    { "$00020, X",   { true,  0x20 } },
    { "$00020, Y",   { false, 0x60 } },
    { "$00030, X",   { true,  0x30 } },
    { "$00030, Y",   { false, 0x60 } },
    { "$00040, X",   { true,  0x40 } },
    { "$00040, Y",   { false, 0x60 } },
    { "$00050, X",   { true,  0x50 } },
    { "$00050, Y",   { false, 0x60 } },
    { "$00060, X",   { true,  0x60 } },
    { "$00060, Y",   { false, 0x60 } },
    { "   $00070,X", { true,  0x70 } },
    { "$00080,X",    { true,  0x80 } },
    { "$000256, X",  { false, -1   } },
    { "$100256, X",  { false, -1   } },
    { "$10 0256, X", { false, -1   } },
    { "$10",         { false, -1   } },
    { "$10, Y",      { false, -1   } },
    { NULL }
  };

  for (int i = 0; tests[i].input; i++) {
    parse_operand_struct test = tests[i];
    long val;
    const char* new_cursor = parse_op_zpx(test.input, &val);
    if (test.expected.ok) {
      munit_assert_ptr_not_equal(tests[i].input, new_cursor);
      munit_assert_int(val, ==, tests[i].expected.val);
    }
    else
      munit_assert_ptr_equal(tests[i].input, new_cursor);
  }
  return MUNIT_OK;
}
MunitResult parse_operand_absolute_test(const MunitParameter *params, void *fixture) {
  parse_operand_struct tests[] = {
    { "$0",       { true,  0x00 } },
    { "$000000",  { true,  0x00 } },
    { "$00010",   { true,  0x10 } },
    { "$00020",   { true,  0x20 } },
    { "$00030",   { true,  0x30 } },
    { "$00040",   { true,  0x40 } },
    { "$00050",   { true,  0x50 } },
    { "$00060",   { true,  0x60 } },
    { "   $00070",{ true,  0x70 } },
    { "$00080",   { true,  0x80 } },
    { "$000256",  { true,  0x256} },
    { "$100256",  { false, -1   } },
    { "$10 0256", { true,  0x10 } },
    { NULL }
  };

  for (int i = 0; tests[i].input; i++) {
    munit_logf(MUNIT_LOG_INFO, "%s", tests[i].input);
    parse_operand_struct test = tests[i];
    long val;
    const char* new_cursor = parse_op_absolute(test.input, &val);
    if (test.expected.ok) {
      munit_assert_ptr_not_equal(tests[i].input, new_cursor);
      munit_assert_int(val, ==, tests[i].expected.val);
    }
    else
      munit_assert_ptr_equal(tests[i].input, new_cursor);
  }
  return MUNIT_OK;
}
MunitResult parse_operand_absolutex_test(const MunitParameter *params, void *fixture) {
  parse_operand_struct tests[] = {
    { "$0, X",       { true,  0x00 } },
    { "$000000, X",  { true,  0x00 } },
    { "$00010, X",   { true,  0x10 } },
    { "$00020, X",   { true,  0x20 } },
    { "$00030, X",   { true,  0x30 } },
    { "$00040, X",   { true,  0x40 } },
    { "$00050, X",   { true,  0x50 } },
    { "$00060, X",   { true,  0x60 } },
    { "   $00070, X",{ true,  0x70 } },
    { "$00080, X",   { true,  0x80 } },
    { "$000256, X",  { true,  0x256} },
    { "$100256, X",  { false, -1   } },
    { "$10, X 0256", { true,  0x10 } },
    { "$10, 0256",   { false,  0x10 } },
    { "$10, Y 0256", { false,  0x10 } },
    { "$12 X 0256",  { false,  0x12 } },
    { "$10, 0256",   { false,  0x10 } },
    { NULL }
  };

  for (int i = 0; tests[i].input; i++) {
    munit_logf(MUNIT_LOG_INFO, "%s", tests[i].input);
    parse_operand_struct test = tests[i];
    long val;
    const char* new_cursor = parse_op_absolutex(test.input, &val);
    if (test.expected.ok) {
      munit_assert_ptr_not_equal(tests[i].input, new_cursor);
      munit_assert_int(val, ==, tests[i].expected.val);
    }
    else
      munit_assert_ptr_equal(tests[i].input, new_cursor);
  }
  return MUNIT_OK;
}
MunitResult parse_operand_absolutey_test(const MunitParameter *params, void *fixture) {
  parse_operand_struct tests[] = {
    { "$0, Y",       { true,  0x00 } },
    { "$000000, Y",  { true,  0x00 } },
    { "$00010, Y",   { true,  0x10 } },
    { "$00020, Y",   { true,  0x20 } },
    { "$00030, Y",   { true,  0x30 } },
    { "$00040, Y",   { true,  0x40 } },
    { "$00050, Y",   { true,  0x50 } },
    { "$00060, Y",   { true,  0x60 } },
    { "   $00070, Y",{ true,  0x70 } },
    { "$00080, Y",   { true,  0x80 } },
    { "$000256, Y",  { true,  0x256} },
    { "$100256, Y",  { false, -1   } },
    { "$10, Y 0256", { true,  0x10 } },
    { "$10, 0256",   { false,  0x10 } },
    { "$10, X 0256", { false,  0x10 } },
    { "$12 Y 0256",  { false,  0x12 } },
    { "$10, 0256",   { false,  0x10 } },
    { NULL }
  };

  for (int i = 0; tests[i].input; i++) {
    munit_logf(MUNIT_LOG_INFO, "%s", tests[i].input);
    parse_operand_struct test = tests[i];
    long val;
    const char* new_cursor = parse_op_absolutey(test.input, &val);
    if (test.expected.ok) {
      munit_assert_ptr_not_equal(tests[i].input, new_cursor);
      munit_assert_int(val, ==, tests[i].expected.val);
    }
    else
      munit_assert_ptr_equal(tests[i].input, new_cursor);
  }
  return MUNIT_OK;
}
