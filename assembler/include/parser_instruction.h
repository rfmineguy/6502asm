#ifndef PARSER_INSTRUCTION_H
#define PARSER_INSTRUCTION_H
#include "assembler_data.h"

/* ALL FUNCTIONS IN THIS FILE EXPECT THE FOLLOWING
 *    cursor points to the start of the operands
 */


const char* ins_parse_ora(const char* cursor, instruction* ins_out, error_parse_op* error);

const char* ins_parse_instruction(const char* cursor, instruction* ins_out, error_parse_op* error);

#endif
