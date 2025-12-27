#ifndef PARSER_H
#define PARSER_H
#include <stdbool.h>

typedef struct {
} instruction;

typedef struct {
} parse_ctx;

// Public Interface
const char* parse_line(const char*, instruction*);

// Private Interface
#ifdef ENABLE_PRIVATE_INTERFACE
/*
 * Desc:   begin parsing a number a the cursor pointer
 * Format:
 *    $4252   - base 16 hexadecimal denoted by $
 *    7164    - base 10 decimal denoted by lack of $
 * Return:
 *    Returns cursor if no number was able to be parsed
 *    Returns the character after the number if a number was parsed
 */
const char* parse_number(const char* cursor, long* num_out);

/*
 * Desc:    parse an ORG directive
 * Format:
 *    org $0800
 * Return:
 *    Returns cursor if no number was able to be parsed
 *    Returns the character after the org address number if the org was parsed
 */
const char* parse_org(const char* cursor, long* addr_out);

/*
 * Desc:    count how many consecutive alpha characters there are starting at
 *            cursor
 */
int         alphalen(const char* cursor);

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
bool        isnewline(const char* s, int* len);
#endif

#endif
