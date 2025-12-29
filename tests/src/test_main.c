#include "tests.h"

MunitTest parse_util[] = {
  {"/number",    parse_util_parse_number_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  {"/alphalen",  parse_util_alphalen_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  {NULL, NULL, NULL, 0, MUNIT_TEST_OPTION_NONE},
};

MunitSuite parse_suite[] = {
  {"/util",      parse_util,      NULL, 1, MUNIT_SUITE_OPTION_NONE },
  {NULL, NULL, NULL, 0, MUNIT_SUITE_OPTION_NONE},
};

MunitSuite suite = {
  "/parse", NULL, parse_suite, 1, MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char** argv) {
  return munit_suite_main(&suite, 0, argc, argv);
}
