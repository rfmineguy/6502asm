#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#define ENABLE_PRIVATE_INTERFACE
#include "parser.h"
#include "parser_util.h"
#include "parser_directive.h"
#include "parser_operand.h"
#include "parser_instruction.h"
#include "parser_errors.h"

void parser_print_instruction(instruction ins) {
  assert(ins.length >= 1 && ins.length <= 3 && "Length 1,2,3 required");
  if (ins.length == 1) {
    printf("Instruction(1) {opcode: %02x}\n", ins.bytes[0]);
  }
  else if (ins.length == 2) {
    printf("Instruction(2) {opcode: %02x, operand: %x}\n", ins.bytes[0], ins.bytes[1]);
  }
  else {
    uint16_t operand = *((uint16_t*)(ins.bytes + 1));
    printf("Instruction(3) {opcode: %02x, operand: %x}\n", ins.bytes[0], operand);
  }
}

void parser_print_directive(directive dir) {
  switch (dir.type) {
    case dt_org:
      {
        printf("Directive {type: org, addr: %x}\n", dir.data.org.addr);
        break;
      }
  }
}

typedef enum state {
  s_default,
  s_directive,
  s_unnamed_label,
  s_instruction,
  s_skip_line,
  s_end,
} state;

const char* parse_line(const char* cursor, parse_ctx* ctx, line_info* line_info_out) {
  assert(line_info_out && "ins_out must not be null");
  assert(ctx           && "ctx must not be null");
  if (*cursor == 0) return 0;

  state state = s_default;
  int newlinelen = 0;
  *line_info_out = (line_info){0};
  while (*cursor) {
    if (state == s_default) {
      while (*cursor && *cursor == ' ') cursor++;
      if (util_isnewline(cursor, &newlinelen)) {
        ctx->column_number = 0;
        ctx->line_number++;
        break;
      }
      if (*cursor == '.')             state = s_directive;
      else if (*cursor == ':')        state = s_unnamed_label;
      else if (util_alphalen(cursor) == 3) state = s_instruction;
      else if (*cursor == ';')        state = s_skip_line;
      else if (*cursor == ' ')        { cursor++; continue; }
    }

    if (state == s_end) break;

    if (state == s_skip_line) {
      while(*cursor) {
        int len;
        if (util_isnewline(cursor, &len)) {
          cursor += len;
          ctx->column_number = 0;
          ctx->line_number++;
          break;
        }
        else cursor ++;
      }
      state = s_end;
      continue;
    }

    if (state == s_directive) {
      cursor++;
      directive dir;
      const char* new_cursor = 0;

      if ((new_cursor = dir_parse_directive(cursor, &dir)) != cursor) {
        cursor = new_cursor;
        *line_info_out = li_directive(dir);
      }
      state = s_default;
      continue;
    }

    if (state == s_instruction) {
      instruction ins;
      error_parse_op error;
      const char* new_cursor;
      if ((new_cursor = ins_parse_instruction(cursor, &ins, &error)) != cursor) {
        cursor = new_cursor;
        *line_info_out = li_instruction(ins);
      }
      if (error != 0) {
        state = s_skip_line;
        *line_info_out = li_error(error);
        continue;
      }
      state = s_default;
      continue;
    }
  }
  return cursor + newlinelen;
}

void parser_print_label(const char* label) {
  printf("Label {name: %s}\n", label);
}

void parser_print_error(error_parse_op code) {
  printf("Error {\n");
  printf("\tcode: %d\n", code);
  printf("\tmsg: %s\n", parser_errors_str(code));
  printf("}\n");
}

void parser_print_lineinfo(line_info info) {
  switch (info.type) {
    case lt_none:
      printf("LineInfo: none\n");
      break;
    case lt_instruction:
      parser_print_instruction(info.data.instruction);
      break;
    case lt_directive:
      parser_print_directive(info.data.directive);
      break;
    case lt_label:
      parser_print_label(info.data.label);
      break;
    case lt_error:
      parser_print_error(info.data.error);
      break;
  }
}
