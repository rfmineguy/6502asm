#include "parser_instruction.h"
#include "parser_operand.h"
#include "parser_errors.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

const char* ins_parse_ora(const char* cursor, instruction* ins_out, error_parse_op* error) {
  assert(error && "error must not be null");

  const char* new_cursor;
  const char* orig = cursor;
  assert(strncmp(cursor, "ora", 3) == 0);
  *error = 0;

  cursor += 3;
  while (*cursor == ' ') cursor++;

  // (indirect, X)
  long val;
  if ((new_cursor = parse_op_indirectx(cursor, &val, error)) != cursor) {
    *ins_out = (instruction){.length = 2, .opcode = 0x01, .byte1 = val & 0xff};
    return new_cursor;
  }
  if (*error == ERROR_PARSE_OP_EXPECTED_LPAREN) goto skip_op_indx;
  if (*error == ERROR_PARSE_OP_EXPECTED_COMMA) goto skip_op_indx;
  if (*error != 0) return orig;

skip_op_indx:
  // (indirect), Y
  if ((new_cursor = parse_op_indirecty(cursor, &val, error)) != cursor) {
    *ins_out = (instruction){.length = 2, .opcode = 0x11, .byte1 = val & 0xff};
    return new_cursor;
  }
  if (*error == ERROR_PARSE_OP_EXPECTED_LPAREN) goto skip_op_indy;
  if (*error != 0) return orig;

skip_op_indy:
  // immediate
  if ((new_cursor = parse_op_immediate(cursor, &val, error)) != cursor) {
    *ins_out = (instruction){.length = 2, .opcode = 0x09, .byte1 = val & 0xff};
    return new_cursor;
  }
  if (*error == ERROR_PARSE_OP_EXPECTED_HASH) goto skip_op_imm;
  if (*error != 0) return orig;

skip_op_imm:
  // zero page
  if ((new_cursor = parse_op_zp(cursor, &val, error)) != cursor) {
    while (*new_cursor == ' ') new_cursor++;
    if (*new_cursor == ',') goto skip_op_zp;
    *ins_out = (instruction){.length = 2, .opcode = 0x05, .byte1 = val & 0xff};
    return new_cursor;
  }
  while (*new_cursor == ' ') new_cursor++;
  if (*error == ERROR_PARSE_OP_NUMBER_OUT_OF_RANGE) goto skip_op_zp;
  if (*error != 0)  return orig;
skip_op_zp:

  // zero page, X
  if ((new_cursor = parse_op_zpx(cursor, &val, error)) != cursor) {
    *ins_out = (instruction){.length = 2, .opcode = 0x15, .byte1 = val & 0xff};
    return new_cursor;
  }
  while (*new_cursor == ' ') new_cursor++;
  if (*error == ERROR_PARSE_OP_NUMBER_OUT_OF_RANGE) goto skip_op_absx;
  if (error != 0) return orig;

skip_op_absx:
  // absolute, X
  if ((new_cursor = parse_op_absolutex(cursor, &val, error)) != cursor) {
    *ins_out = (instruction){.length = 3, .opcode = 0x1D, .byte1 = val & 0xff, .byte2 = (val >> 8) & 0xff};
    return new_cursor;
  }
  if (*error == ERROR_PARSE_OP_EXPECTED_COMMA) goto skip_op_abs;
  if (*error == ERROR_PARSE_OP_EXPECTED_X) goto skip_op_absy;
  if (error != 0) return orig;

skip_op_absy:
  // absolute, Y
  if ((new_cursor = parse_op_absolutey(cursor, &val, error)) != cursor) {
    *ins_out = (instruction){.length = 3, .opcode = 0x19, .byte1 = val & 0xff, .byte2 = (val >> 8) & 0xff};
    return new_cursor;
  }
  if (error != 0) return orig;

skip_op_abs:
  // absolute
  if ((new_cursor = parse_op_absolute(cursor, &val, error)) != cursor) {
    *ins_out = (instruction){.length = 3, .opcode = 0x0D, .byte1 = val & 0xff, .byte2 = (val >> 8) & 0xff};
    return new_cursor;
  }
  if (error != 0) return orig;
  return orig;
}

const char* ins_parse_instruction(const char* cursor, instruction* ins_out, error_parse_op* error) {
  assert(error && "error must not be null");
  const char* orig = cursor;
  const char* new_cursor;
  if (strncmp(cursor, "ora", 3) == 0) {
    if ((new_cursor = ins_parse_ora(cursor, ins_out, error)) == cursor) {
      return orig;
    }
    return new_cursor;
  }
  printf("%3s\n", cursor);
  assert(0 && "instruction not implemented");
  return orig;
}
