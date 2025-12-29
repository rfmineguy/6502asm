#ifndef PARSER_UTIL_H
#define PARSER_UTIL_H
#include <stdbool.h>

/*
 * Desc:   begin parsing a number a the cursor pointer
 * Format:
 *    $4252   - base 16 hexadecimal denoted by $
 *    7164    - base 10 decimal denoted by lack of $
 * Return:
 *    Returns cursor if no number was able to be parsed
 *    Returns the character after the number if a number was parsed
 */
const char* util_parse_number(const char*, long*);

#endif
