#include "parser_errors.h"
#include <assert.h>

const char* parser_errors_str(error_parse code) {
  switch (code) {
    case ERROR_PARSE_NONE:
      return "None";
    case ERROR_PARSE_EXPECTED_NUMBER:
      return "Expected Number";
    case ERROR_PARSE_NUMBER_OUT_OF_RANGE:
      return "Number out of range";
    case ERROR_PARSE_EXPECTED_COMMA:
      return "Expected comma";
    case ERROR_PARSE_EXPECTED_X:
      return "Expected 'X'";
    case ERROR_PARSE_EXPECTED_Y:
      return "Expected 'Y'";
    case ERROR_PARSE_EXPECTED_LPAREN:
      return "Expected '('";
    case ERROR_PARSE_EXPECTED_RPAREN:
      return "Expected ')'";
    case ERROR_PARSE_EXPECTED_HASH:
      return "Expected '#'";
    case ERROR_PARSE_BYTE_DIR_TOO_LONG:
      return "Byte sequence too long";
    case ERROR_PARSE_EXPECTED_NAME:
      return "Expected name";
    case ERROR_PARSE_UNEXPECTED_NAME:
      return "Unexpected name";
    case ERROR_PARSE_EXPECTED_COLON:
      return "Expected ':'";
  }
  assert(0 && "Unreachable");
}
