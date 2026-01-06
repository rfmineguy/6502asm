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

MunitResult parse_instruction_ora_immediate_test(const MunitParameter *params, void *fixture) {
  parse_ins_struct tests[] = {
    { "ora #53",    { true,  (instruction) {.length = 2, .opcode = 0x09, .byte1 = 53   }, ERROR_PARSE_NONE } },
    { "ora #$53",   { true,  (instruction) {.length = 2, .opcode = 0x09, .byte1 = 0x53 }, ERROR_PARSE_NONE } },
    { "ora #5342",  { false, (instruction) { /* doesnt matter */ }                      , ERROR_PARSE_NUMBER_OUT_OF_RANGE } },
    { "ora #254",   { true,  (instruction) {.length = 2, .opcode = 0x09, .byte1 = 254  }, ERROR_PARSE_NONE } },
    { "ora #$3113", { false, (instruction) { /* doesnt matter */ }                      , ERROR_PARSE_NUMBER_OUT_OF_RANGE } },
    { "ora #$",     { false, (instruction) { /* doesnt matter */ }                      , ERROR_PARSE_EXPECTED_NUMBER     } },
    { NULL }
  };

  for (int i = 0; tests[i].input; i++) {
    parse_ins_struct test = tests[i];
    munit_logf(MUNIT_LOG_INFO, "%s", test.input);
    instruction ins;
    error_parse error;
    munit_assert_memory_equal(3, test.input, "ora");
    test.input += 3;
    const char* new_cursor = ins_util_parse_operand(test.input, &ins, ins_table_ora, &error);
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

MunitResult parse_instruction_ora_zpx_test(const MunitParameter *params, void *fixture) {
  parse_ins_struct tests[] = {
    { "ora $53, X",    { true,  (instruction) {.length = 2, .opcode = 0x15, .byte1 = 0x53 }, ERROR_PARSE_NONE } },
    { "ora $ff, X",    { true,  (instruction) {.length = 2, .opcode = 0x15, .byte1 = 0xff }, ERROR_PARSE_NONE } },
    { "ora $42, Y",    { false, (instruction) { /* this is a zpy instruction        */    }, ERROR_PARSE_EXPECTED_X } },
    { "ora $, X",      { false, (instruction) { /* this is missing a number         */    }, ERROR_PARSE_EXPECTED_NUMBER } },

    /* this is actually a zeropage ins due to not having a comma and being a valid zeropage instruction
     * this tests whether the zero page parser can recognize whether it needs to try zeropage, x next */
    { "ora $42 X",     { true,  (instruction) {.length = 2, .opcode = 0x05, .byte1 = 0x42  }, ERROR_PARSE_NONE } },
    { NULL }
  };

  for (int i = 0; tests[i].input; i++) {
    parse_ins_struct test = tests[i];
    munit_logf(MUNIT_LOG_INFO, "%s", test.input);
    instruction ins;
    error_parse error;
    munit_assert_memory_equal(3, test.input, "ora");
    test.input += 3;
    const char* new_cursor = ins_util_parse_operand(test.input, &ins, ins_table_ora, &error);
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

MunitResult parse_instruction_ora_zp_test(const MunitParameter *params, void *fixture) {
  parse_ins_struct tests[] = {
    { "ora $53",    { true,  (instruction) {.length = 2, .opcode = 0x05, .byte1 = 0x53 }, ERROR_PARSE_NONE } },
    { "ora $ff",    { true,  (instruction) {.length = 2, .opcode = 0x05, .byte1 = 0xff }, ERROR_PARSE_NONE } },
    { "ora $",      { false, (instruction) { /* this is missing a number         */    }, ERROR_PARSE_EXPECTED_NUMBER } },
    { "ora $42",    { true,  (instruction) {.length = 2, .opcode = 0x05, .byte1 = 0x42 }, ERROR_PARSE_NONE } },
    { "ora ",       { false, (instruction) { /* this is missing a number         */    }, ERROR_PARSE_EXPECTED_NUMBER } },

    /* this is actually a zeropage,x ins due to having a comma and being a valid zeropage,x instruction
     * this tests whether the zero page parser can recognize whether it needs to try zeropage, x next */
    { "ora $42, X", { true,  (instruction) {.length = 2, .opcode = 0x15, .byte1 = 0x42 }, ERROR_PARSE_NONE } },
    { NULL }
  };

  for (int i = 0; tests[i].input; i++) {
    parse_ins_struct test = tests[i];
    munit_logf(MUNIT_LOG_INFO, "'%s'", test.input);
    instruction ins;
    error_parse error;
    munit_assert_memory_equal(3, test.input, "ora");
    test.input += 3;
    const char* new_cursor = ins_util_parse_operand(test.input, &ins, ins_table_ora, &error);
    munit_logf(MUNIT_LOG_INFO, "newc: '%s'", new_cursor);
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

MunitResult parse_instruction_ora_absolute_test (const MunitParameter *params, void *fixture) {
  parse_ins_struct tests[] = {
    { "ora $531",   { true,  (instruction) {.length = 3, .opcode = 0x0d, .byte1 = 0x31, .byte2 = 0x05 }, ERROR_PARSE_NONE } },
    { "ora $ff13",  { true,  (instruction) {.length = 3, .opcode = 0x0d, .byte1 = 0x13, .byte2 = 0xff }, ERROR_PARSE_NONE } },
    { "ora $ffff",  { true,  (instruction) {.length = 3, .opcode = 0x0d, .byte1 = 0xff, .byte2 = 0xff }, ERROR_PARSE_NONE } },
    { "ora $ff13ab",{ false, (instruction) { /* this has address which is too large */                }, ERROR_PARSE_NUMBER_OUT_OF_RANGE } },
    { "ora $",      { false, (instruction) { /* this is missing a number         */                   }, ERROR_PARSE_EXPECTED_NUMBER } },
    { "ora ",       { false, (instruction) { /* this is missing a number         */                   }, ERROR_PARSE_EXPECTED_NUMBER } },

    /* this is actually a zeropage,x ins due to having a comma and being a valid zeropage,x instruction
     * this tests whether the zero page parser can recognize whether it needs to try zeropage, x next */
    { "ora $42, X", { true,  (instruction) {.length = 2, .opcode = 0x15, .byte1 = 0x42 }, ERROR_PARSE_NONE } },
    { NULL }
  };

  for (int i = 0; tests[i].input; i++) {
    parse_ins_struct test = tests[i];
    munit_logf(MUNIT_LOG_INFO, "%s", test.input);
    instruction ins;
    error_parse error;
    munit_assert_memory_equal(3, test.input, "ora");
    test.input += 3;
    const char* new_cursor = ins_util_parse_operand(test.input, &ins, ins_table_ora, &error);
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

MunitResult parse_instruction_ora_absolutex_test(const MunitParameter *params, void *fixture) {
  parse_ins_struct tests[] = {
    { "ora $531, X",   { true,  (instruction) {.length = 3, .opcode = 0x1d, .byte1 = 0x31, .byte2=0x05 }, ERROR_PARSE_NONE } },
    { "ora $534, X",   { true,  (instruction) {.length = 3, .opcode = 0x1d, .byte1 = 0x34, .byte2=0x05 }, ERROR_PARSE_NONE } },
    { "ora $3145, X",  { true,  (instruction) {.length = 3, .opcode = 0x1d, .byte1 = 0x45, .byte2=0x31 }, ERROR_PARSE_NONE } },
    { "ora #$3113, X", { false, (instruction) { /* doesnt matter */                                    }, ERROR_PARSE_NUMBER_OUT_OF_RANGE } },
    { "ora $, x",      { false, (instruction) { /* doesnt matter */                                    }, ERROR_PARSE_EXPECTED_NUMBER } },
    { NULL }
  };

  for (int i = 0; tests[i].input; i++) {
    parse_ins_struct test = tests[i];
    munit_logf(MUNIT_LOG_INFO, "%s", test.input);
    instruction ins;
    error_parse error;
    munit_assert_memory_equal(3, test.input, "ora");
    test.input += 3;
    const char* new_cursor = ins_util_parse_operand(test.input, &ins, ins_table_ora, &error);
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

MunitResult parse_instruction_ora_absolutey_test(const MunitParameter *params, void *fixture) {
  parse_ins_struct tests[] = {
    { "ora $531, Y",   { true,  (instruction) {.length = 3, .opcode = 0x19, .byte1 = 0x31, .byte2=0x05 }, ERROR_PARSE_NONE } },
    { "ora $534, Y",   { true,  (instruction) {.length = 3, .opcode = 0x19, .byte1 = 0x34, .byte2=0x05 }, ERROR_PARSE_NONE } },
    { "ora $3145, Y",  { true,  (instruction) {.length = 3, .opcode = 0x19, .byte1 = 0x45, .byte2=0x31 }, ERROR_PARSE_NONE } },
    { "ora #$3113, Y", { false, (instruction) { /* doesnt matter */                                    }, ERROR_PARSE_NUMBER_OUT_OF_RANGE } },
    { "ora $, y",      { false, (instruction) { /* doesnt matter */                                    }, ERROR_PARSE_EXPECTED_NUMBER } },
    { NULL }
  };

  for (int i = 0; tests[i].input; i++) {
    parse_ins_struct test = tests[i];
    munit_logf(MUNIT_LOG_INFO, "%s", test.input);
    instruction ins;
    error_parse error;
    munit_assert_memory_equal(3, test.input, "ora");
    test.input += 3;
    const char* new_cursor = ins_util_parse_operand(test.input, &ins, ins_table_ora, &error);
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

MunitResult parse_instruction_ora_indirectx_test(const MunitParameter *params, void *fixture) {
  parse_ins_struct tests[] = {
    { "ora ($31, X)",    { true,  (instruction) {.length = 2, .opcode = 0x01, .byte1 = 0x31 }, ERROR_PARSE_NONE } },
    { "ora ($34, X)",    { true,  (instruction) {.length = 2, .opcode = 0x01, .byte1 = 0x34 }, ERROR_PARSE_NONE } },
    { "ora ($45, X)",    { true,  (instruction) {.length = 2, .opcode = 0x01, .byte1 = 0x45 }, ERROR_PARSE_NONE } },
    { "ora ($3113, X)",  { false, (instruction) { /* doesnt matter */                       }, ERROR_PARSE_NUMBER_OUT_OF_RANGE } },
    { "ora (#$3113, X)", { false, (instruction) { /* doesnt matter */                       }, ERROR_PARSE_EXPECTED_NUMBER } },
    { "ora ($, X)",      { false, (instruction) { /* doesnt matter */                       }, ERROR_PARSE_EXPECTED_NUMBER } },
    { NULL }
  };

  for (int i = 0; tests[i].input; i++) {
    parse_ins_struct test = tests[i];
    munit_logf(MUNIT_LOG_INFO, "%s", test.input);
    instruction ins;
    error_parse error;
    munit_assert_memory_equal(3, test.input, "ora");
    test.input += 3;
    const char* new_cursor = ins_util_parse_operand(test.input, &ins, ins_table_ora, &error);
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

MunitResult parse_instruction_ora_indirecty_test(const MunitParameter *params, void *fixture) {
  parse_ins_struct tests[] = {
    { "ora ($31), Y",    { true,  (instruction) {.length = 2, .opcode = 0x11, .byte1 = 0x31 }, ERROR_PARSE_NONE } },
    { "ora ($34), Y",    { true,  (instruction) {.length = 2, .opcode = 0x11, .byte1 = 0x34 }, ERROR_PARSE_NONE } },
    { "ora ($45), Y",    { true,  (instruction) {.length = 2, .opcode = 0x11, .byte1 = 0x45 }, ERROR_PARSE_NONE } },
    { "ora ($45, Y)",    { false, (instruction) { /* doesnt matter */                       }, ERROR_PARSE_EXPECTED_X } },
    { "ora ($3113), Y",  { false, (instruction) { /* doesnt matter */                       }, ERROR_PARSE_NUMBER_OUT_OF_RANGE } },
    { "ora (#$3113), Y", { false, (instruction) { /* doesnt matter */                       }, ERROR_PARSE_EXPECTED_NUMBER } },
    { "ora ($), Y",      { false, (instruction) { /* doesnt matter */                       }, ERROR_PARSE_EXPECTED_NUMBER } },
    { NULL }
  };

  for (int i = 0; tests[i].input; i++) {
    parse_ins_struct test = tests[i];
    munit_logf(MUNIT_LOG_INFO, "%s", test.input);
    instruction ins;
    error_parse error;
    munit_assert_memory_equal(3, test.input, "ora");
    test.input += 3;
    const char* new_cursor = ins_util_parse_operand(test.input, &ins, ins_table_ora, &error);
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
