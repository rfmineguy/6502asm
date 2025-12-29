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
