#ifndef PARSER_H
#define PARSER_H
#include <stdbool.h>
#include <stdint.h>
#include "assembler_data.h"

// Public Interface
const char* parse_line(const char*, parse_ctx*, line_info*);

void        parser_print_instruction(instruction ins);
void        parser_print_directive(directive dir);
void        parser_print_label(const char* label);

void        parser_print_lineinfo(line_info info);

#endif
