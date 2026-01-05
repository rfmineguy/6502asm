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
#include "parser_label.h"
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
    case dt_byte:
      {
        printf("Directive {type: byte, bytes_count: %d, bytes: [", dir.data.byte.bytes_count);
        for (int i = 0; i < dir.data.byte.bytes_count; i++) {
          printf("%x", dir.data.byte.bytes[i]);
          if (i != dir.data.byte.bytes_count - 1) printf(", ");
        }
        printf("]}\n");
        // assert(0 && "dt_byte print not implemented");
      }
  }
}

typedef enum state {
  s_default,
  s_directive,
  s_unnamed_label,
  s_instruction,
  s_skip_line,
  s_label,
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
      else if (*cursor == ';')        state = s_skip_line;
      else if (util_alphalen(cursor) == 3) state = s_instruction;
      else if (isalpha(*cursor))      state = s_label;
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

    if (state == s_unnamed_label) {
      label label;
      error_parse error;
      const char* new_cursor = 0;

      if ((new_cursor = label_parse_unnamed_label(cursor, &label, &error)) != cursor) {
        cursor = new_cursor;
        label.type = lt_unnamed;
        line_info_out->data[line_info_out->data_count++] = li_label(label);
      }
      if (error == ERROR_PARSE_EXPECTED_COLON) state = s_label;
      else if (error != ERROR_PARSE_NONE) {
        state = s_skip_line;
      }
      else state = s_default;
    }

    if (state == s_label) {
      label label;
      error_parse error;
      const char* new_cursor = 0;

      if ((new_cursor = label_parse_named_label(cursor, &label, &error)) != cursor) {
        cursor = new_cursor;
        label.type = lt_named;
        line_info_out->data[line_info_out->data_count++] = li_label(label);
      }
      if (error != 0) {
        state = s_skip_line;
        line_info_out->data[line_info_out->data_count++] = li_error(error);
        continue;
      }
      else state = s_default;
      continue;
    }

    if (state == s_directive) {
      cursor++;
      directive dir;
      error_parse error;
      const char* new_cursor = 0;

      if ((new_cursor = dir_parse_directive(cursor, &dir, &error)) != cursor) {
        cursor = new_cursor;
        line_info_out->data[line_info_out->data_count++] = li_directive(dir);
      }
      if (error != 0) {
        state = s_skip_line;
        line_info_out->data[line_info_out->data_count++] = li_error(error);
        continue;
      }
      state = s_default;
      continue;
    }

    if (state == s_instruction) {
      instruction ins;
      error_parse error;
      const char* new_cursor;
      if ((new_cursor = ins_parse_instruction(cursor, &ins, &error)) != cursor) {
        cursor = new_cursor;
        line_info_out->data[line_info_out->data_count++] = li_instruction(ins);
      }
      if (error != 0) {
        state = s_skip_line;
        line_info_out->data[line_info_out->data_count++] = li_error(error);
        continue;
      }
      state = s_default;
      continue;
    }
  }
  return cursor + newlinelen;
}

void parser_print_label(label label) {
  switch (label.type) {
    case lt_named:
      printf("Label {name: %.*s}\n", label.data.named.length, label.data.named.name);
      break;
    case lt_unnamed:
      printf("Label {<unnamed>}\n");
      break;
  }
}

void parser_print_error(error_parse code) {
  printf("Error {\n");
  printf("\tcode: %d\n", code);
  printf("\tmsg: %s\n", parser_errors_str(code));
  printf("}\n");
}

void parser_print_line_info_element(line_info_element el) {
  switch (el.type) {
    case lt_none:
      printf("LineInfo: none\n");
      break;
    case lt_instruction:
      parser_print_instruction(el.data.instruction);
      break;
    case lt_directive:
      parser_print_directive(el.data.directive);
      break;
    case lt_label:
      parser_print_label(el.data.label);
      break;
    case lt_error:
      parser_print_error(el.data.error);
      break;
  }
}

void parser_print_lineinfo(line_info info) {
  for (int i = 0; i < info.data_count; i++) {
    parser_print_line_info_element(info.data[i]);
  }
}
