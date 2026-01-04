#ifndef PARSER_DIRECTIVE_H
#define PARSER_DIRECTIVE_H
#include "assembler_data.h"

/*
 * Desc:    parse an ORG directive
 * Format:
 *    org $0800
 * Return:
 *    Returns cursor if no number was able to be parsed
 *    Returns the character after the org address number if the org was parsed
 */
const char* dir_parse_org(const char* cursor, uint16_t* addr_out, error_parse* error);


const char* dir_parse_directive(const char* cursor, directive* dir_out, error_parse* error);

#endif
