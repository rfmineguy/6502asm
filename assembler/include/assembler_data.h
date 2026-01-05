#ifndef ASSEMBLER_DATA_H
#define ASSEMBLER_DATA_H
#include <stdint.h>
#include "parser_errors.h"

#define _bytes(...) {__VA_ARGS__}

#define li_directive(directive_)\
  (line_info_element){\
    .type = lt_directive,\
    .line_number = ctx->line_number,\
    .column_number = ctx->column_number,\
    .filename = "",\
    .data.directive = directive_,\
  }

#define li_instruction(instruction_)\
  (line_info_element){\
    .type = lt_instruction,\
    .line_number = ctx->line_number,\
    .column_number = ctx->column_number,\
    .filename = "",\
    .data.instruction = instruction_,\
  }

#define li_label(label_)\
  (line_info_element){\
    .type = lt_label,\
    .line_number = ctx->line_number,\
    .column_number = ctx->column_number,\
    .filename = "",\
    .data.label = label_,\
  }

#define li_error(error_)\
  (line_info_element){\
    .type = lt_error,\
    .line_number = ctx->line_number,\
    .column_number = ctx->column_number,\
    .filename = "",\
    .data.error = error_,\
  }

#define ins(len, ...)\
  (instruction) {\
    .length = len,\
    .bytes = { __VA_ARGS__ }\
  }

typedef struct {
  uint8_t length;
  union {
    uint8_t bytes[3];
    struct {
      uint8_t opcode;
      uint8_t byte1;
      uint8_t byte2;
    };
  };
} instruction;

typedef enum {
  dt_org,
} directive_type;

typedef struct {
  directive_type type;
  union {
    struct {
      uint16_t addr;
    } org;
  } data;
} directive;

typedef enum {
  lt_named,
  lt_unnamed,
} label_type;

typedef struct {
  label_type type;
  union {
    struct {
      const char* name;
      int length;
    } named;
    /* no data for the unnamed label */
  } data;
} label;

typedef enum {
  lt_none,
  lt_instruction,
  lt_directive,
  lt_label,
  lt_error
} line_info_type;

typedef struct {
  int line_number, column_number;
  const char* filename;
  line_info_type type;
  union {
    instruction instruction;
    directive directive;
    label label;
    error_parse error;
  } data;
} line_info_element;

typedef struct {
  line_info_element data[10];
  int data_count;
} line_info;

typedef struct {
  int line_number, column_number;
} parse_ctx;

#endif
