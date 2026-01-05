#ifndef PARSER_LABEL_H
#define PARSER_LABEL_H
#include "assembler_data.h"

const char* label_parse_named_label(const char* cursor, label* label_out, error_parse* error);
const char* label_parse_unnamed_label(const char* cursor, label* label_out, error_parse* error);

#endif
