#include "tests.h"

MunitTest parse_util[] = {
  {"/number",    parse_util_parse_number_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  {"/alphalen",  parse_util_alphalen_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  {"/isnewline", parse_util_isnewline_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  {"/predlen",   parse_util_predlen_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  {NULL, NULL, NULL, 0, MUNIT_TEST_OPTION_NONE},
};

MunitTest parse_operand[] = {
  {"/acc",       parse_operand_accumulator_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  {"/immediate", parse_operand_immediate_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  {"/zp",        parse_operand_zp_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  {"/zpx",       parse_operand_zpx_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  {"/absolute",  parse_operand_absolute_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  {"/absolutex", parse_operand_absolutex_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  {"/absolutey", parse_operand_absolutey_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  {"/indirectx", parse_operand_indirectx_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  {"/indirecty", parse_operand_indirecty_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  {NULL, NULL, NULL, 0, MUNIT_TEST_OPTION_NONE},
};

MunitTest parse_directive[] = {
  {"/org",       parse_directive_org_test,  NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  {"/byte",      parse_directive_byte_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  {NULL, NULL, NULL, 0, MUNIT_TEST_OPTION_NONE},
};

MunitTest parse_instruction_ora[] = {
  {"/imm",      parse_instruction_ora_immediate_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  {"/zpx",      parse_instruction_ora_zpx_test      , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  {"/zp",       parse_instruction_ora_zp_test       , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  {"/abs",      parse_instruction_ora_absolute_test , NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  {"/absx",     parse_instruction_ora_absolutex_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  {"/absy",     parse_instruction_ora_absolutey_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  {"/indx",     parse_instruction_ora_indirectx_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  {"/indy",     parse_instruction_ora_indirecty_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  {NULL, NULL, NULL, 0, MUNIT_TEST_OPTION_NONE},
};

MunitTest parse_label[] = {
  {"/named",    parse_label_named_test,   NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {"/unnamed",  parse_label_unnamed_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  {NULL, NULL, NULL, 0, MUNIT_TEST_OPTION_NONE},
};

MunitSuite parse_instruction[] = {
  {"/ora",       parse_instruction_ora, NULL, 1, MUNIT_SUITE_OPTION_NONE },
  {NULL, NULL, NULL, 0, MUNIT_SUITE_OPTION_NONE},
};

MunitSuite parse_suite[] = {
  {"/util",        parse_util,      NULL, 1, MUNIT_SUITE_OPTION_NONE },
  {"/operand",     parse_operand,   NULL, 1, MUNIT_SUITE_OPTION_NONE },
  {"/directive",   parse_directive, NULL, 1, MUNIT_SUITE_OPTION_NONE },
  {"/instruction", NULL, parse_instruction, 1, MUNIT_SUITE_OPTION_NONE },
  {"/label",       parse_label, NULL, 1, MUNIT_SUITE_OPTION_NONE },
  {NULL, NULL, NULL, 0, MUNIT_SUITE_OPTION_NONE},
};

MunitTest fileutil[] = {
  {"/read",          fileutil_read_test         , NULL, NULL, MUNIT_TEST_OPTION_NONE },
  {NULL, NULL, NULL, 0, MUNIT_TEST_OPTION_NONE},
};

MunitSuite suite[] = {
  { "/fileutil", fileutil, NULL, 1, MUNIT_SUITE_OPTION_NONE },
  { "/parse", NULL, parse_suite, 1, MUNIT_SUITE_OPTION_NONE },
  {NULL, NULL, NULL, 0, MUNIT_SUITE_OPTION_NONE},
};

MunitSuite main_suite = {
  "/main", NULL, suite, 1, MUNIT_SUITE_OPTION_NONE,
};

int main(int argc, char** argv) {
  return munit_suite_main(&main_suite, 0, argc, argv);
}
