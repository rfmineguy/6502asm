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
const char* parse_number(const char* cursor, long* num_out);
const char* parse_org(const char* cursor, long* addr_out);
const char* parse_next_line(const char* cursor);

int         alphalen(const char* cursor);
bool        isnewline(const char* s, int* len);
#endif

#endif
