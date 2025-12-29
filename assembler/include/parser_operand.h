#ifndef OPERAND_PARSER_H
#define OPERAND_PARSER_H

// #immediate
const char* parse_op_immediate(const char* cursor, long* val);

// zp
const char* parse_op_zp(const char* cursor, long* val);

// zp,X
const char* parse_op_zpx(const char* cursor, long* val);

// absolute
const char* parse_op_absolute(const char* cursor, long* val);

// absolute, X
const char* parse_op_absolutex(const char* cursor, long* val);

// absolute, Y
const char* parse_op_absolutey(const char* cursor, long* val);

// (indirect, X)
const char* parse_op_indirectx(const char* cursor, long* val);

// (indirect), Y
const char* parse_op_indirecty(const char* cursor, long* val);

#endif
