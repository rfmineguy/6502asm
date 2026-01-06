#ifndef OPERAND_PARSER_H
#define OPERAND_PARSER_H
#include "parser_errors.h"

// accumulator
const char* parse_op_accumulator(const char* cursor, error_parse* error);

// #immediate
const char* parse_op_immediate(const char* cursor, long* val, error_parse* error);

// zp
const char* parse_op_zp(const char* cursor, long* val, error_parse* error);

// zp,X
const char* parse_op_zpx(const char* cursor, long* val, error_parse* error);

// absolute
const char* parse_op_absolute(const char* cursor, long* val, error_parse* error);

// absolute, X
const char* parse_op_absolutex(const char* cursor, long* val, error_parse* error);

// absolute, Y
const char* parse_op_absolutey(const char* cursor, long* val, error_parse* error);

// (indirect, X)
const char* parse_op_indirectx(const char* cursor, long* val, error_parse* error);

// (indirect), Y
const char* parse_op_indirecty(const char* cursor, long* val, error_parse* error);

#endif
