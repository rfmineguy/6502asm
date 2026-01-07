#include "tests.h"
#include "parser_errors.h"
#include "instruction_tables.h"
#include <stdbool.h>

typedef struct {
  const char* input;
  struct {
    bool ok;
    instruction ins;
    error_parse error;
  } expected;
} parse_ins_struct;

MunitResult parse_instruction_asl_accumulator_test(const MunitParameter *params, void *fixture) {
  parse_ins_struct tests[] = {
    { "asl A",      { true,  (instruction) {.length = 1, .opcode = 0x0A   }, ERROR_PARSE_NONE } },
    { "asl B",      { false, { /* asl B doesn't exist */                  }, ERROR_PARSE_EXPECTED_NUMBER } },
    { "asl Cdaf",   { false, { /* asl Cdaf doesn't exist */               }, ERROR_PARSE_EXPECTED_NUMBER } },
    { NULL }
  };

  for (int i = 0; tests[i].input; i++) {
    parse_ins_struct test = tests[i];
    munit_logf(MUNIT_LOG_INFO, "%s", test.input);
    instruction ins;
    error_parse error;
    munit_assert_memory_equal(3, test.input, "asl");
    test.input += 3;
    const char* new_cursor = ins_util_parse_operand(test.input, &ins, ins_table_asl, &error);
    munit_logf(MUNIT_LOG_INFO, "newc: '%s', '%s'", new_cursor, test.input);
    munit_assert_int(error, ==, test.expected.error);
    if (test.expected.ok) {
      munit_assert_ptr_not_equal(test.input, new_cursor);
      munit_assert_int(ins.length, ==, test.expected.ins.length);
      munit_assert_int(ins.opcode, ==, test.expected.ins.opcode);
      munit_assert_int(ins.byte1,  ==, test.expected.ins.byte1);
      munit_assert_int(ins.byte2,  ==, test.expected.ins.byte2);
    }
    else
      munit_assert_ptr_equal(test.input, new_cursor);
  }
  return MUNIT_OK;
}

MunitResult parse_instruction_asl_zp_test(const MunitParameter *params, void *fixture) {
  parse_ins_struct tests[] = {
    { "asl $0f",    { true,  (instruction) {.length = 2, .opcode = 0x06, .byte1=0x0f   }, ERROR_PARSE_NONE } },
    { "asl $34",    { true,  (instruction) {.length = 2, .opcode = 0x06, .byte1=0x34   }, ERROR_PARSE_NONE } },
    { "asl $ff",    { true,  (instruction) {.length = 2, .opcode = 0x06, .byte1=0xff   }, ERROR_PARSE_NONE } },
    { "asl ff",     { false, { /* number must lead with something */                   }, ERROR_PARSE_EXPECTED_NUMBER } },
    { NULL }
  };

  for (int i = 0; tests[i].input; i++) {
    parse_ins_struct test = tests[i];
    munit_logf(MUNIT_LOG_INFO, "%s", test.input);
    instruction ins;
    error_parse error;
    munit_assert_memory_equal(3, test.input, "asl");
    test.input += 3;
    const char* new_cursor = ins_util_parse_operand(test.input, &ins, ins_table_asl, &error);
    munit_logf(MUNIT_LOG_INFO, "newc: '%s', '%s'", new_cursor, test.input);
    munit_assert_int(error, ==, test.expected.error);
    if (test.expected.ok) {
      munit_assert_ptr_not_equal(test.input, new_cursor);
      munit_assert_int(ins.length, ==, test.expected.ins.length);
      munit_assert_int(ins.opcode, ==, test.expected.ins.opcode);
      munit_assert_int(ins.byte1,  ==, test.expected.ins.byte1);
      munit_assert_int(ins.byte2,  ==, test.expected.ins.byte2);
    }
    else
      munit_assert_ptr_equal(test.input, new_cursor);
  }
  return MUNIT_OK;
}

MunitResult parse_instruction_asl_zpx_test(const MunitParameter *params, void *fixture) {
  parse_ins_struct tests[] = {
    { "asl $0f, X",    { true,  (instruction) {.length = 2, .opcode = 0x16, .byte1 = 0x0f   }, ERROR_PARSE_NONE } },
    { "asl $34, X",    { true,  (instruction) {.length = 2, .opcode = 0x16, .byte1 = 0x34   }, ERROR_PARSE_NONE } },
    { "asl $, X",      { false, { /* asl Cdaf doesn't exist */                              }, ERROR_PARSE_EXPECTED_NUMBER } },
    { NULL }
  };

  for (int i = 0; tests[i].input; i++) {
    parse_ins_struct test = tests[i];
    munit_logf(MUNIT_LOG_INFO, "%s", test.input);
    instruction ins;
    error_parse error;
    munit_assert_memory_equal(3, test.input, "asl");
    test.input += 3;
    const char* new_cursor = ins_util_parse_operand(test.input, &ins, ins_table_asl, &error);
    munit_logf(MUNIT_LOG_INFO, "newc: '%s', '%s'", new_cursor, test.input);
    munit_assert_int(error, ==, test.expected.error);
    if (test.expected.ok) {
      munit_assert_ptr_not_equal(test.input, new_cursor);
      munit_assert_int(ins.length, ==, test.expected.ins.length);
      munit_assert_int(ins.opcode, ==, test.expected.ins.opcode);
      munit_assert_int(ins.byte1,  ==, test.expected.ins.byte1);
      munit_assert_int(ins.byte2,  ==, test.expected.ins.byte2);
    }
    else
      munit_assert_ptr_equal(test.input, new_cursor);
  }
  return MUNIT_OK;
}
MunitResult parse_instruction_asl_abs_test(const MunitParameter *params, void *fixture) {
  parse_ins_struct tests[] = {
    { "asl $314",      { true,  (instruction) {.length = 3, .opcode = 0x0e, .byte1 = 0x14, .byte2 = 0x3   }, ERROR_PARSE_NONE } },
    { "asl $536",      { true,  (instruction) {.length = 3, .opcode = 0x0e, .byte1 = 0x36, .byte2 = 0x5   }, ERROR_PARSE_NONE } },
    { "asl $53641",    { false, { /* asl Cdaf doesn't exist */               }, ERROR_PARSE_NUMBER_OUT_OF_RANGE } },
    { NULL }
  };

  for (int i = 0; tests[i].input; i++) {
    parse_ins_struct test = tests[i];
    munit_logf(MUNIT_LOG_INFO, "%s", test.input);
    instruction ins;
    error_parse error;
    munit_assert_memory_equal(3, test.input, "asl");
    test.input += 3;
    const char* new_cursor = ins_util_parse_operand(test.input, &ins, ins_table_asl, &error);
    munit_logf(MUNIT_LOG_INFO, "newc: '%s', '%s'", new_cursor, test.input);
    munit_assert_int(error, ==, test.expected.error);
    if (test.expected.ok) {
      munit_assert_ptr_not_equal(test.input, new_cursor);
      munit_assert_int(ins.length, ==, test.expected.ins.length);
      munit_assert_int(ins.opcode, ==, test.expected.ins.opcode);
      munit_assert_int(ins.byte1,  ==, test.expected.ins.byte1);
      munit_assert_int(ins.byte2,  ==, test.expected.ins.byte2);
    }
    else
      munit_assert_ptr_equal(test.input, new_cursor);
  }
  return MUNIT_OK;
}
MunitResult parse_instruction_asl_absx_test(const MunitParameter *params, void *fixture) {
  parse_ins_struct tests[] = {
    { "asl $314,X",      { true,  (instruction) {.length = 3, .opcode = 0x1e, .byte1 = 0x14, .byte2 = 0x3   }, ERROR_PARSE_NONE } },
    { "asl $536,X",      { true,  (instruction) {.length = 3, .opcode = 0x1e, .byte1 = 0x36, .byte2 = 0x5   }, ERROR_PARSE_NONE } },
    { "asl $53641,X",    { false, { /* asl Cdaf doesn't exist */               }, ERROR_PARSE_NUMBER_OUT_OF_RANGE } },
    { NULL }
  };

  for (int i = 0; tests[i].input; i++) {
    parse_ins_struct test = tests[i];
    munit_logf(MUNIT_LOG_INFO, "%s", test.input);
    instruction ins;
    error_parse error;
    munit_assert_memory_equal(3, test.input, "asl");
    test.input += 3;
    const char* new_cursor = ins_util_parse_operand(test.input, &ins, ins_table_asl, &error);
    munit_logf(MUNIT_LOG_INFO, "newc: '%s', '%s'", new_cursor, test.input);
    munit_assert_int(error, ==, test.expected.error);
    if (test.expected.ok) {
      munit_assert_ptr_not_equal(test.input, new_cursor);
      munit_assert_int(ins.length, ==, test.expected.ins.length);
      munit_assert_int(ins.opcode, ==, test.expected.ins.opcode);
      munit_assert_int(ins.byte1,  ==, test.expected.ins.byte1);
      munit_assert_int(ins.byte2,  ==, test.expected.ins.byte2);
    }
    else
      munit_assert_ptr_equal(test.input, new_cursor);
  }
  return MUNIT_OK;
}
