#include "tests.h"
#include "parser_operand.h"
#include "parser_errors.h"
#include <stdbool.h>

typedef struct {
  const char* input;
  struct {
    bool ok;
    long val;
    int error;
  } expected;
} parse_operand_struct;

MunitResult parse_operand_accumulator_test(const MunitParameter *params, void *fixture) {
  parse_operand_struct tests[] = {
    { "A",      { true,  0 , ERROR_PARSE_NONE } },
    { "A, 3",   { true,  0 , ERROR_PARSE_NONE } },
    { "B",      { false, 0 , ERROR_PARSE_EXPECTED_ACCUMULATOR } },
    { "#4",     { false, 0 , ERROR_PARSE_EXPECTED_ACCUMULATOR } },
    { NULL }
  };

  for (int i = 0; tests[i].input; i++) {
    parse_operand_struct test = tests[i];
    munit_logf(MUNIT_LOG_INFO, "in: %s", test.input);
    long val;
    error_parse error;
    const char* new_cursor = parse_op_accumulator(test.input, &error);
    munit_assert_int(error, ==, test.expected.error);
    if (test.expected.ok) {
      munit_assert_ptr_not_equal(tests[i].input, new_cursor);
    }
    else
      munit_assert_ptr_equal(tests[i].input, new_cursor);
  }
  return MUNIT_OK;
}

MunitResult parse_operand_immediate_test(const MunitParameter *params, void *fixture) {
  parse_operand_struct tests[] = {
    { "#0",     { true,  0 , ERROR_PARSE_NONE } },
    { "#000",   { true,  0 , ERROR_PARSE_NONE } },
    { "#00010", { true,  10, ERROR_PARSE_NONE } },
    { "#00020", { true,  20, ERROR_PARSE_NONE } },
    { "#00030", { true,  30, ERROR_PARSE_NONE } },
    { "#00040", { true,  40, ERROR_PARSE_NONE } },
    { "#00050", { true,  50, ERROR_PARSE_NONE } },
    { "#00060", { true,  60, ERROR_PARSE_NONE } },
    { "#00070", { true,  70, ERROR_PARSE_NONE } },
    { "#00080", { true,  80, ERROR_PARSE_NONE } },
    { "#256",   { false, -1, ERROR_PARSE_NUMBER_OUT_OF_RANGE } },
    { NULL }
  };

  for (int i = 0; tests[i].input; i++) {
    parse_operand_struct test = tests[i];
    munit_logf(MUNIT_LOG_INFO, "in: %s", test.input);
    long val;
    error_parse error;
    const char* new_cursor = parse_op_immediate(test.input, &val, &error);
    munit_assert_int(error, ==, test.expected.error);
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
    { "$0",      { true,  0x00, ERROR_PARSE_NONE                } },
    { "$000000", { true,  0x00, ERROR_PARSE_NONE                } },
    { "$00010",  { true,  0x10, ERROR_PARSE_NONE                } },
    { "$00020",  { true,  0x20, ERROR_PARSE_NONE                } },
    { "$00030",  { true,  0x30, ERROR_PARSE_NONE                } },
    { "$00040",  { true,  0x40, ERROR_PARSE_NONE                } },
    { "$00050",  { true,  0x50, ERROR_PARSE_NONE                } },
    { "$00060",  { true,  0x60, ERROR_PARSE_NONE                } },
    { "$00070",  { true,  0x70, ERROR_PARSE_NONE                } },
    { "$00080",  { true,  0x80, ERROR_PARSE_NONE                } },
    { "$000256", { false, -1  , ERROR_PARSE_NUMBER_OUT_OF_RANGE } },
    { "$100256", { false, -1  , ERROR_PARSE_NUMBER_OUT_OF_RANGE } },
    { NULL }
  };

  for (int i = 0; tests[i].input; i++) {
    parse_operand_struct test = tests[i];
    munit_logf(MUNIT_LOG_INFO, "%s", test.input);
    long val;
    error_parse error;
    const char* new_cursor = parse_op_zp(test.input, &val, &error);
    munit_assert_int(error, ==, test.expected.error);
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
    { "$0, X",       { true,  0x00, ERROR_PARSE_NONE                } },
    { "$000000, X",  { true,  0x00, ERROR_PARSE_NONE                } },
    { "$00000, Y",   { false, 0x60, ERROR_PARSE_EXPECTED_X          } },
    { "$00010, X",   { true,  0x10, ERROR_PARSE_NONE                } },
    { "$00010, Y",   { false, 0x60, ERROR_PARSE_EXPECTED_X          } },
    { "$00020, X",   { true,  0x20, ERROR_PARSE_NONE                } },
    { "$00020, Y",   { false, 0x60, ERROR_PARSE_EXPECTED_X          } },
    { "$00030, X",   { true,  0x30, ERROR_PARSE_NONE                } },
    { "$00030, Y",   { false, 0x60, ERROR_PARSE_EXPECTED_X          } },
    { "$00040, X",   { true,  0x40, ERROR_PARSE_NONE                } },
    { "$00040, Y",   { false, 0x60, ERROR_PARSE_EXPECTED_X          } },
    { "$00050, X",   { true,  0x50, ERROR_PARSE_NONE                } },
    { "$00050, Y",   { false, 0x60, ERROR_PARSE_EXPECTED_X          } },
    { "$00060, X",   { true,  0x60, ERROR_PARSE_NONE                } },
    { "$00060, Y",   { false, 0x60, ERROR_PARSE_EXPECTED_X          } },
    { "   $00070,X", { true,  0x70, ERROR_PARSE_NONE                } },
    { "$00080,X",    { true,  0x80, ERROR_PARSE_NONE                } },
    { "$000256, X",  { false, -1  , ERROR_PARSE_NUMBER_OUT_OF_RANGE } },
    { "$100256, X",  { false, -1  , ERROR_PARSE_NUMBER_OUT_OF_RANGE } },
    { "$10 0256, X", { false, -1  , ERROR_PARSE_EXPECTED_COMMA      } },
    { "$10",         { false, -1  , ERROR_PARSE_EXPECTED_COMMA      } },
    { "$10, Y",      { false, -1  , ERROR_PARSE_EXPECTED_X          } },
    { NULL }
  };

  for (int i = 0; tests[i].input; i++) {
    parse_operand_struct test = tests[i];
    munit_logf(MUNIT_LOG_INFO, "%s", test.input);
    long val;
    error_parse error;
    const char* new_cursor = parse_op_zpx(test.input, &val, &error);
    munit_assert_int(error, ==, test.expected.error);
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
    { "$0",       { true,  0x00 , ERROR_PARSE_NONE                } },
    { "$000000",  { true,  0x00 , ERROR_PARSE_NONE                } },
    { "$00010",   { true,  0x10 , ERROR_PARSE_NONE                } },
    { "$00020",   { true,  0x20 , ERROR_PARSE_NONE                } },
    { "$00030",   { true,  0x30 , ERROR_PARSE_NONE                } },
    { "$00040",   { true,  0x40 , ERROR_PARSE_NONE                } },
    { "$00050",   { true,  0x50 , ERROR_PARSE_NONE                } },
    { "$00060",   { true,  0x60 , ERROR_PARSE_NONE                } },
    { "   $00070",{ true,  0x70 , ERROR_PARSE_NONE                } },
    { "$00080",   { true,  0x80 , ERROR_PARSE_NONE                } },
    { "$000256",  { true,  0x256, ERROR_PARSE_NONE                } },
    { "$100256",  { false, -1   , ERROR_PARSE_NUMBER_OUT_OF_RANGE } },
    { "$10 0256", { true,  0x10 , ERROR_PARSE_NONE                } },
    { NULL }
  };

  for (int i = 0; tests[i].input; i++) {
    munit_logf(MUNIT_LOG_INFO, "%s", tests[i].input);
    parse_operand_struct test = tests[i];
    long val;
    error_parse error;
    const char* new_cursor = parse_op_absolute(test.input, &val, &error);
    munit_assert_int(error, ==, test.expected.error);
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
    { "$0, X",       { true,  0x00,  ERROR_PARSE_NONE                } },
    { "$000000, X",  { true,  0x00,  ERROR_PARSE_NONE                } },
    { "$00010, X",   { true,  0x10,  ERROR_PARSE_NONE                } },
    { "$00020, X",   { true,  0x20,  ERROR_PARSE_NONE                } },
    { "$00030, X",   { true,  0x30,  ERROR_PARSE_NONE                } },
    { "$00040, X",   { true,  0x40,  ERROR_PARSE_NONE                } },
    { "$00050, X",   { true,  0x50,  ERROR_PARSE_NONE                } },
    { "$00060, X",   { true,  0x60,  ERROR_PARSE_NONE                } },
    { "   $00070, X",{ true,  0x70,  ERROR_PARSE_NONE                } },
    { "$00080, X",   { true,  0x80,  ERROR_PARSE_NONE                } },
    { "$000256, X",  { true,  0x256, ERROR_PARSE_NONE                } },
    { "$100256, X",  { false, -1   , ERROR_PARSE_NUMBER_OUT_OF_RANGE } },
    { "$10, X 0256", { true,  0x10 , ERROR_PARSE_NONE                } },
    { "$10, 0256",   { false,  0x10, ERROR_PARSE_EXPECTED_X          } },
    { "$10, Y 0256", { false,  0x10, ERROR_PARSE_EXPECTED_X          } },
    { "$12 X 0256",  { false,  0x12, ERROR_PARSE_EXPECTED_COMMA      } },
    { "$10, 0256",   { false,  0x10, ERROR_PARSE_EXPECTED_X          } },
    { NULL }
  };

  for (int i = 0; tests[i].input; i++) {
    munit_logf(MUNIT_LOG_INFO, "%s", tests[i].input);
    parse_operand_struct test = tests[i];
    long val;
    error_parse error;
    const char* new_cursor = parse_op_absolutex(test.input, &val, &error);
    munit_assert_int(error, ==, test.expected.error);
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
    { "$0, Y",       { true,  0x00 , ERROR_PARSE_NONE                } },
    { "$000000, Y",  { true,  0x00 , ERROR_PARSE_NONE                } },
    { "$00010, Y",   { true,  0x10 , ERROR_PARSE_NONE                } },
    { "$00020, Y",   { true,  0x20 , ERROR_PARSE_NONE                } },
    { "$00030, Y",   { true,  0x30 , ERROR_PARSE_NONE                } },
    { "$00040, Y",   { true,  0x40 , ERROR_PARSE_NONE                } },
    { "$00050, Y",   { true,  0x50 , ERROR_PARSE_NONE                } },
    { "$00060, Y",   { true,  0x60 , ERROR_PARSE_NONE                } },
    { "   $00070, Y",{ true,  0x70 , ERROR_PARSE_NONE                } },
    { "$00080, Y",   { true,  0x80 , ERROR_PARSE_NONE                } },
    { "$000256, Y",  { true,  0x256, ERROR_PARSE_NONE                } },
    { "$100256, Y",  { false, -1   , ERROR_PARSE_NUMBER_OUT_OF_RANGE } },
    { "$10, Y 0256", { true,  0x10 , ERROR_PARSE_NONE                } },
    { "$10, 0256",   { false, 0x10 , ERROR_PARSE_EXPECTED_Y          } },
    { "$10, X 0256", { false, 0x10 , ERROR_PARSE_EXPECTED_Y          } },
    { "$12 Y 0256",  { false, 0x12 , ERROR_PARSE_EXPECTED_COMMA      } },
    { "$10, 0256",   { false, 0x10 , ERROR_PARSE_EXPECTED_Y          } },
    { NULL }
  };

  for (int i = 0; tests[i].input; i++) {
    munit_logf(MUNIT_LOG_INFO, "%s", tests[i].input);
    parse_operand_struct test = tests[i];
    long val;
    error_parse error;
    const char* new_cursor = parse_op_absolutey(test.input, &val, &error);
    munit_assert_int(error, ==, test.expected.error);
    if (test.expected.ok) {
      munit_assert_ptr_not_equal(tests[i].input, new_cursor);
      munit_assert_int(val, ==, tests[i].expected.val);
    }
    else
      munit_assert_ptr_equal(tests[i].input, new_cursor);
  }
  return MUNIT_OK;
}
MunitResult parse_operand_indirectx_test(const MunitParameter *params, void *fixture) {
  parse_operand_struct tests[] = {
    { "($0, X)",       { true,  0x00 , ERROR_PARSE_NONE                } },
    { "($000000, X)",  { true,  0x00 , ERROR_PARSE_NONE                } },
    { "($00010, X)",   { true,  0x10 , ERROR_PARSE_NONE                } },
    { "($00020, X)",   { true,  0x20 , ERROR_PARSE_NONE                } },
    { "($00030, X)",   { true,  0x30 , ERROR_PARSE_NONE                } },
    { "($00040, X)",   { true,  0x40 , ERROR_PARSE_NONE                } },
    { "($00050, X)",   { true,  0x50 , ERROR_PARSE_NONE                } },
    { "($00060, X)",   { true,  0x60 , ERROR_PARSE_NONE                } },
    { "   ($00070, X)",{ true,  0x70 , ERROR_PARSE_NONE                } },
    { "($00080, X)",   { true,  0x80 , ERROR_PARSE_NONE                } },
    { "($000256, X)",  { false, 0x256, ERROR_PARSE_NUMBER_OUT_OF_RANGE } },
    { "($100256, X)",  { false, -1   , ERROR_PARSE_NUMBER_OUT_OF_RANGE } },
    { "($10, Y)",      { false, 0x10 , ERROR_PARSE_EXPECTED_X          } },
    { "($10, )",       { false, 0x10 , ERROR_PARSE_EXPECTED_X          } },
    { "($10, X)",      { true,  0x10 , ERROR_PARSE_NONE                } },
    { "($12 Y)",       { false, 0x12 , ERROR_PARSE_EXPECTED_COMMA      } },
    { "($10, )",       { false, 0x10 , ERROR_PARSE_EXPECTED_X          } },
    { "($10, X",       { false, 0x10 , ERROR_PARSE_EXPECTED_RPAREN     } },
    { "$10, X)",       { false, 0x10 , ERROR_PARSE_EXPECTED_LPAREN     } },
    { NULL }
  };

  for (int i = 0; tests[i].input; i++) {
    munit_logf(MUNIT_LOG_INFO, "%s", tests[i].input);
    parse_operand_struct test = tests[i];
    long val;
    error_parse error;
    const char* new_cursor = parse_op_indirectx(test.input, &val, &error);
    munit_assert_int(error, ==, test.expected.error);
    if (test.expected.ok) {
      munit_assert_ptr_not_equal(tests[i].input, new_cursor);
      munit_assert_int(val, ==, tests[i].expected.val);
    }
    else
      munit_assert_ptr_equal(tests[i].input, new_cursor);
  }
  return MUNIT_OK;
}
MunitResult parse_operand_indirecty_test(const MunitParameter *params, void *fixture) {
  parse_operand_struct tests[] = {
    { "($0), Y",       { true,  0x00 , ERROR_PARSE_NONE                } },
    { "($000000), Y",  { true,  0x00 , ERROR_PARSE_NONE                } },
    { "($00010), Y",   { true,  0x10 , ERROR_PARSE_NONE                } },
    { "($00020), Y",   { true,  0x20 , ERROR_PARSE_NONE                } },
    { "($00030), Y",   { true,  0x30 , ERROR_PARSE_NONE                } },
    { "($00040), Y",   { true,  0x40 , ERROR_PARSE_NONE                } },
    { "($00050), Y",   { true,  0x50 , ERROR_PARSE_NONE                } },
    { "($00060), Y",   { true,  0x60 , ERROR_PARSE_NONE                } },
    { "   ($00070), Y",{ true,  0x70 , ERROR_PARSE_NONE                } },
    { "($00080), Y",   { true,  0x80 , ERROR_PARSE_NONE                } },
    { "($000256), Y",  { false, 0x256, ERROR_PARSE_NUMBER_OUT_OF_RANGE } },
    { "($100256), Y",  { false, -1   , ERROR_PARSE_NUMBER_OUT_OF_RANGE } },
    { "($10), X",      { false, 0x10 , ERROR_PARSE_EXPECTED_Y          } },
    { "($10), ",       { false, 0x10 , ERROR_PARSE_EXPECTED_Y          } },
    { "($10), Y",      { true,  0x10 , ERROR_PARSE_NONE                } },
    { "($12 X",        { false, 0x12 , ERROR_PARSE_EXPECTED_RPAREN     } },
    { "($10), ",       { false, 0x10 , ERROR_PARSE_EXPECTED_Y          } },
    { "$10, X)",       { false, 0x10 , ERROR_PARSE_EXPECTED_LPAREN     } },
    { NULL }
  };

  for (int i = 0; tests[i].input; i++) {
    munit_logf(MUNIT_LOG_INFO, "%s", tests[i].input);
    parse_operand_struct test = tests[i];
    long val;
    error_parse error;
    const char* new_cursor = parse_op_indirecty(test.input, &val, &error);
    munit_assert_int(error, ==, test.expected.error);
    if (test.expected.ok) {
      munit_assert_ptr_not_equal(tests[i].input, new_cursor);
      munit_assert_int(val, ==, tests[i].expected.val);
    }
    else
      munit_assert_ptr_equal(tests[i].input, new_cursor);
  }
  return MUNIT_OK;
}
