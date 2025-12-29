#include "tests.h"
#include "parser_directive.h"
#include "assembler_data.h"
#include <stdbool.h>

typedef struct {
  const char* input;
  struct {
    bool ok;
    directive dir;
  } expected;
} parse_operand_struct;

MunitResult parse_directive_org_test(const MunitParameter *params, void *fixture) {
  parse_ctx ctx_ = {};
  parse_ctx* ctx = &ctx_;
  parse_operand_struct tests[] = {
    { "org $5000",  { true, (directive){.type = dt_org, .data.org.addr = 0x5000}} },
    { "org $50",    { true, (directive){.type = dt_org, .data.org.addr = 0x50}} },
    { "org $5",     { true, (directive){.type = dt_org, .data.org.addr = 0x5}} },
    { "org $3000",  { true, (directive){.type = dt_org, .data.org.addr = 0x3000}} },
    { "org $30",    { true, (directive){.type = dt_org, .data.org.addr = 0x30}} },
    { "org $3",     { true, (directive){.type = dt_org, .data.org.addr = 0x3}} },
    { "org $256d4", { false, {} } },
    { "org $10000", { false, {} } },
    { NULL },
  };

  for (int i = 0; tests[i].input; i++) {
    parse_operand_struct test = tests[i];
    directive val;
    const char* new_cursor = dir_parse_directive(test.input, &val);
    if (test.expected.ok) {
      munit_assert_ptr_not_equal(tests[i].input, new_cursor);
      munit_assert_int(val.type, ==, dt_org);
      munit_assert_int(val.data.org.addr, ==, tests[i].expected.dir.data.org.addr);
    }
    else
      munit_assert_ptr_equal(tests[i].input, new_cursor);
  }

  return MUNIT_OK;
}
