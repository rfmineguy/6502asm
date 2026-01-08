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

MunitResult parse_instruction_bcc_relative_test(const MunitParameter *params, void *fixture) {
  parse_ins_struct tests[] = {
    { "bcc $53",    { true,  (instruction) {.length = 2, .opcode = 0x90, .byte1 = 0x53 }, ERROR_PARSE_NONE } },
    // { "bcc label",  { true,  (instruction) {.length = 2, .opcode = 0x90, .byte1 = 0x53 }, ERROR_PARSE_NONE } },
    /* { "bcc label",  { true,  (instruction) {.length = 2, .opcode = 0x90   }, ERROR_PARSE_NONE } }, */
    { NULL }
  };

  for (int i = 0; tests[i].input; i++) {
    parse_ins_struct test = tests[i];
    munit_logf(MUNIT_LOG_INFO, "%s", test.input);
    instruction ins;
    error_parse error;
    munit_assert_memory_equal(3, test.input, "bcc");
    test.input += 3;
    const char* new_cursor = ins_util_parse_operand(test.input, &ins, ins_table_bcc, &error);
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
