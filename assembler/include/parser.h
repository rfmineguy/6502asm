#ifndef PARSER_H
#define PARSER_H

typedef struct {
} instruction;

typedef struct {
} parse_ctx;

const char* parse_number(const char* cursor, long* num_out);
int         asciilen(const char* cursor);

const char* parse_line(const char*, instruction*);

#endif
