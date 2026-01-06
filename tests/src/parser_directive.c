#include "tests.h"
#include "parser_directive.h"
#include "assembler_data.h"
#include "parser_directive_support.h"
#include <stdbool.h>

typedef struct {
  const char* input;
  struct {
    bool ok;
    directive dir;
    error_parse error;
  } expected;
} parse_operand_struct;

MunitResult parse_directive_org_test(const MunitParameter *params, void *fixture) {
  parse_ctx ctx_ = {};
  parse_ctx* ctx = &ctx_;
  parse_operand_struct tests[] = {
    { "org $5000",  { true, (directive){.type = dt_org, .data.org.addr = 0x5000}, ERROR_PARSE_NONE }},
    { "org $50",    { true, (directive){.type = dt_org, .data.org.addr = 0x50}  , ERROR_PARSE_NONE }},
    { "org $5",     { true, (directive){.type = dt_org, .data.org.addr = 0x5}   , ERROR_PARSE_NONE }},
    { "org $3000",  { true, (directive){.type = dt_org, .data.org.addr = 0x3000}, ERROR_PARSE_NONE }},
    { "org $30",    { true, (directive){.type = dt_org, .data.org.addr = 0x30}  , ERROR_PARSE_NONE }},
    { "org $3",     { true, (directive){.type = dt_org, .data.org.addr = 0x3}   , ERROR_PARSE_NONE }},
    { "org $256d4", { false, {/* irrelevant due to error */}                    , ERROR_PARSE_NUMBER_OUT_OF_RANGE }},
    { "org $10000", { false, {/* irrelevant due to error */}                    , ERROR_PARSE_NUMBER_OUT_OF_RANGE }},
    { "org",        { false, {/* irrelevant due to error */}                    , ERROR_PARSE_EXPECTED_NUMBER }},
    { NULL },
  };

  for (int i = 0; tests[i].input; i++) {
    parse_operand_struct test = tests[i];
    munit_logf(MUNIT_LOG_INFO, "%s", test.input);
    directive val;
    error_parse error;
    const char* new_cursor = dir_parse_directive(test.input, &val, &error);
    munit_assert_int(error, ==, test.expected.error);
    if (test.expected.ok) {
      munit_assert_ptr_not_equal(tests[i].input, new_cursor);
      munit_assert_int(val.type, ==, tests[i].expected.dir.type);
      munit_assert_int(val.data.org.addr, ==, tests[i].expected.dir.data.org.addr);
    }
    else
      munit_assert_ptr_equal(tests[i].input, new_cursor);
  }

  return MUNIT_OK;
}

MunitResult parse_directive_byte_test(const MunitParameter *params, void *fixture) {
  parse_ctx ctx_ = {};
  parse_ctx* ctx = &ctx_;
  parse_operand_struct tests[] = {
    { "byte $50",           { true,  (directive){.type = dt_byte, .data.byte = {.bytes = { 0x50              }, .bytes_count = 1   }}, ERROR_PARSE_NONE }},
    { "byte $50,$25",       { true,  (directive){.type = dt_byte, .data.byte = {.bytes = { 0x50, 0x25        }, .bytes_count = 2   }}, ERROR_PARSE_NONE }},
    { "byte $50,$25",       { true,  (directive){.type = dt_byte, .data.byte = {.bytes = { 0x50, 0x25        }, .bytes_count = 2   }}, ERROR_PARSE_NONE }},
    { "byte #50,$25",       { true,  (directive){.type = dt_byte, .data.byte = {.bytes = { 50,   0x25        }, .bytes_count = 2   }}, ERROR_PARSE_NONE }},
    { "byte $50,$25 ; comm",{ true,  (directive){.type = dt_byte, .data.byte = {.bytes = { 0x50, 0x25        }, .bytes_count = 2   }}, ERROR_PARSE_NONE }},
    { "byte " BYTE_SEQ_100, { true,  (directive){.type = dt_byte, .data.byte = {.bytes = EXPECTED_BYTE_SEQ_100, .bytes_count = 100 }}, ERROR_PARSE_NONE }},
    { "byte #50,$125",      { false, (directive){ /* 0x125 is out of range of a byte                                         */     }, ERROR_PARSE_NUMBER_OUT_OF_RANGE }},
    { "byte " BYTE_SEQ_101, { false, (directive){ /* 101 bytes in a byte directive cannot happen, split into multiple .bytes */     }, ERROR_PARSE_BYTE_DIR_TOO_LONG }},
    { NULL },
  };

  for (int i = 0; tests[i].input; i++) {
    parse_operand_struct test = tests[i];
    directive val;
    error_parse error;
    munit_logf(MUNIT_LOG_INFO, "%s", test.input);
    const char* new_cursor = dir_parse_directive(test.input, &val, &error);
    munit_assert_int(error, ==, test.expected.error);
    if (test.expected.ok) {
      munit_assert_ptr_not_equal(tests[i].input, new_cursor);
      munit_assert_int(val.type, ==, tests[i].expected.dir.type);
      munit_assert_int(val.data.byte.bytes_count, ==, tests[i].expected.dir.data.byte.bytes_count);
      munit_assert_memory_equal(tests[i].expected.dir.data.byte.bytes_count, val.data.byte.bytes, tests[i].expected.dir.data.byte.bytes);
    }
    else
      munit_assert_ptr_equal(tests[i].input, new_cursor);
  }

  return MUNIT_OK;
}
