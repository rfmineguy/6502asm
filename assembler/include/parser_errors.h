#ifndef PARSER_ERRORS
#define PARSER_ERRORS

// reserve 0 - 200 for parse op errors
typedef enum {
  ERROR_PARSE_NONE                = 0,
  ERROR_PARSE_EXPECTED_NUMBER     = 1,
  ERROR_PARSE_NUMBER_OUT_OF_RANGE = 2,
  ERROR_PARSE_EXPECTED_COMMA      = 3,
  ERROR_PARSE_EXPECTED_X          = 4,
  ERROR_PARSE_EXPECTED_Y          = 5,
  ERROR_PARSE_EXPECTED_LPAREN     = 6,
  ERROR_PARSE_EXPECTED_RPAREN     = 7,
  ERROR_PARSE_EXPECTED_HASH       = 8,

  ERROR_PARSE_BYTE_DIR_TOO_LONG   = 100,
} error_parse;

const char* parser_errors_str(error_parse code);

#endif
