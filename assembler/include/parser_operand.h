#ifndef OPERAND_PARSER_H
#define OPERAND_PARSER_H
#include "parser_errors.h"

// #immediate
const char* parse_op_immediate(const char* cursor, long* val, error_parse_op* error);

// zp
const char* parse_op_zp(const char* cursor, long* val, error_parse_op* error);

// zp,X
const char* parse_op_zpx(const char* cursor, long* val, error_parse_op* error);

// absolute
const char* parse_op_absolute(const char* cursor, long* val, error_parse_op* error);

// absolute, X
const char* parse_op_absolutex(const char* cursor, long* val, error_parse_op* error);

// absolute, Y
const char* parse_op_absolutey(const char* cursor, long* val, error_parse_op* error);

// (indirect, X)
const char* parse_op_indirectx(const char* cursor, long* val, error_parse_op* error);

// (indirect), Y
const char* parse_op_indirecty(const char* cursor, long* val, error_parse_op* error);

#endif
