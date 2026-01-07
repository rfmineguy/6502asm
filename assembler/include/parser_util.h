#ifndef PARSER_UTIL_H
#define PARSER_UTIL_H
#include <stdbool.h>
#include <stdint.h>
#include "parser_errors.h"

typedef enum {
  TM_NONE,
  TM_HIGH,
  TM_LOW,
} truncate_mode;

typedef enum {
  ET_LABEL,
  ET_LABEL_REL,
  ET_LITERAL,
} expr_type;

typedef struct {
  expr_type type;
  truncate_mode trunc_mode;
  union {
    struct {
      const char* s;
      int len;
    } label;
    struct {
      int offset;
    } label_rel;
    long val;
  } data;
} expr;

/*
 * Desc:   begin parsing a number a the cursor pointer
 * Format:
 *    $4252   - base 16 hexadecimal denoted by $
 *    7164    - base 10 decimal denoted by lack of $
 * Return:
 *    Returns cursor if no number was able to be parsed
 *    Returns the character after the number if a number was parsed
 */
const char* util_parse_number(const char*, long*, error_parse*);

/*
 * Desc:    begin parsing an expression at the cursor
 * Format:
 *   $4252
 *   7425
 *   <$4252
 *   >$4252
 *   <hello
 *   >hello
 */
const char* util_parse_expr(const char*, expr*, error_parse*);

/*
 * Desc:    count how many consecutive alpha characters there are starting at
 *            cursor
 */
int         util_alphalen(const char* cursor);

/*
 * Desc:    check if the sequence pointed to by s is a newline
 * Formats:
 *    \r\n
 *    \r
 *    \n
 * Return:
 *    True if its a newline
 *    False if not
 */
bool        util_isnewline(const char* s, int* len);

/*
 * Desc:    count how many characters match the predicate
 */
int         util_predlen(const char* cursor, int(*predicate)(int c));

#endif
