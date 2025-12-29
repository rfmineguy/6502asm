#include "tests.h"

MunitTest parse_util[] = {
  {"/number",    parse_util_parse_number_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  {"/alphalen",  parse_util_alphalen_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  {"/isnewline", parse_util_isnewline_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  {NULL, NULL, NULL, 0, MUNIT_TEST_OPTION_NONE},
};

MunitTest parse_operand[] = {
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
MunitSuite parse_suite[] = {
  {"/util",      parse_util,      NULL, 1, MUNIT_SUITE_OPTION_NONE },
  {"/operand",   parse_operand,   NULL, 1, MUNIT_SUITE_OPTION_NONE },
  {NULL, NULL, NULL, 0, MUNIT_SUITE_OPTION_NONE},
};

MunitSuite suite = {
  "/parse", NULL, parse_suite, 1, MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char** argv) {
  return munit_suite_main(&suite, 0, argc, argv);
}
