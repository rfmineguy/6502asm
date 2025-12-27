#include "tests.h"

MunitTest parse_test[] = {
  {"/number",   parse_number_test,    NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  {"/alphalen", parser_alphalen_test, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
  {NULL, NULL, NULL, 0, MUNIT_TEST_OPTION_NONE},
};

MunitSuite suite = {
  "/parse", parse_test, NULL, 1, MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char** argv) {
  return munit_suite_main(&suite, 0, argc, argv);
}
