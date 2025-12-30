#include "parser_errors.h"

const char* parser_errors_str(error_parse_op code) {
  switch (code) {
    case ERROR_PARSE_OP_NONE:
      return "None";
    case ERROR_PARSE_OP_EXPECTED_NUMBER:
      return "Expected Number";
    case ERROR_PARSE_OP_NUMBER_OUT_OF_RANGE:
      return "Number out of range";
    case ERROR_PARSE_OP_EXPECTED_COMMA:
      return "Expected comma";
    case ERROR_PARSE_OP_EXPECTED_X:
      return "Expected 'X'";
    case ERROR_PARSE_OP_EXPECTED_Y:
      return "Expected 'Y'";
    case ERROR_PARSE_OP_EXPECTED_LPAREN:
      return "Expected '('";
    case ERROR_PARSE_OP_EXPECTED_RPAREN:
      return "Expected ')'";
    case ERROR_PARSE_OP_EXPECTED_HASH:
      return "Expected '#'";
  }
}
