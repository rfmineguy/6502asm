#include "parser_instruction_util.h"
#include "parser_operand.h"
#include <assert.h>
#include <string.h>

/*
 * instructions in the table with length = 0 indicate that this is an unsupported addressing mode
 */
const char* ins_util_parse_operand(const char* cursor, instruction* ins_out, instruction table[AM_LAST_], error_parse* error) {
  assert(error && "error must not be null");
  assert(ins_out && "ins_out must not be null");
  assert(table && "table must not be null");
  const char* orig = cursor;

  while (*cursor == ' ') cursor++;

  const char* new_cursor;
  *error = ERROR_PARSE_NONE;

  long val = 0;

  // accumulator
  if (table[ACC].length == 0) {
    *error = ERROR_PARSE_UNSUPPORTED_ADDR_MODE;
    goto skip_op_acc;
  }
  if ((new_cursor = parse_op_accumulator(cursor, error)) != cursor) {
    *ins_out = (instruction){.length = table[ACC].length, .opcode = table[ACC].opcode};
    return new_cursor;
  }
  if (*error == ERROR_PARSE_EXPECTED_ACCUMULATOR) goto skip_op_acc;
  if (*error != ERROR_PARSE_NONE) return orig;
skip_op_acc:
  // order matters for zp
  // (indirect, X)
  if (table[INDX].length == 0) {
    *error = ERROR_PARSE_UNSUPPORTED_ADDR_MODE;
    goto skip_op_indx;
  }
  if ((new_cursor = parse_op_indirectx(cursor, &val, error)) != cursor) {
    *ins_out = (instruction){.length = table[INDX].length, .opcode = table[INDX].opcode, .byte1 = val & 0xff};
    return new_cursor;
  }
  if (*error == ERROR_PARSE_EXPECTED_LPAREN) goto skip_op_indx;
  if (*error == ERROR_PARSE_EXPECTED_COMMA) goto skip_op_indx;
  if (*error != ERROR_PARSE_NONE) return orig;

skip_op_indx:
  // (indirect), Y
  if (table[INDY].length == 0) {
    *error = ERROR_PARSE_UNSUPPORTED_ADDR_MODE;
    goto skip_op_indy;
  }
  if ((new_cursor = parse_op_indirecty(cursor, &val, error)) != cursor) {
    *ins_out = (instruction){.length = table[INDY].length, .opcode = table[INDY].opcode, .byte1 = val & 0xff};
    return new_cursor;
  }
  if (*error == ERROR_PARSE_EXPECTED_LPAREN) goto skip_op_indy;
  if (*error != ERROR_PARSE_NONE) return orig;

skip_op_indy:
  // immediate
  if (table[IMM].length == 0) {
    *error = ERROR_PARSE_UNSUPPORTED_ADDR_MODE;
    goto skip_op_imm;
  }
  if ((new_cursor = parse_op_immediate(cursor, &val, error)) != cursor) {
    *ins_out = (instruction){.length = table[IMM].length, .opcode = table[IMM].opcode, .byte1 = val & 0xff};
    return new_cursor;
  }
  if (*error == ERROR_PARSE_EXPECTED_HASH) goto skip_op_imm;
  if (*error != ERROR_PARSE_NONE) return orig;

skip_op_imm:
  // zero page
  if (table[ZP].length == 0) {
    *error = ERROR_PARSE_UNSUPPORTED_ADDR_MODE;
    goto skip_op_zp;
  }
  if ((new_cursor = parse_op_zp(cursor, &val, error)) != cursor) {
    while (*new_cursor == ' ') new_cursor++;
    if (*new_cursor == ',') goto skip_op_zp;
    *ins_out = (instruction){.length = table[ZP].length, .opcode = table[ZP].opcode, .byte1 = val & 0xff};
    return new_cursor;
  }
  while (*new_cursor == ' ') new_cursor++;
  if (*error == ERROR_PARSE_NUMBER_OUT_OF_RANGE) goto skip_op_zp;
  if (*error == ERROR_PARSE_EXPECTED_NUMBER) goto skip_op_zp;
  if (*error != ERROR_PARSE_NONE)  return orig;
skip_op_zp:

  // zero page, X
  if (table[ZPX].length == 0) {
    *error = ERROR_PARSE_UNSUPPORTED_ADDR_MODE;
    goto skip_op_absx;
  }
  if ((new_cursor = parse_op_zpx(cursor, &val, error)) != cursor) {
    *ins_out = (instruction){.length = table[ZPX].length, .opcode = table[ZPX].opcode, .byte1 = val & 0xff};
    return new_cursor;
  }
  while (*new_cursor == ' ') new_cursor++;
  if (*error == ERROR_PARSE_NUMBER_OUT_OF_RANGE) goto skip_op_absx;
  if (*error == ERROR_PARSE_EXPECTED_COMMA) goto skip_op_absx;
  if (*error == ERROR_PARSE_EXPECTED_NUMBER) goto skip_op_absx;
  if (*error != ERROR_PARSE_NONE) return orig;

skip_op_absx:
  // absolute, X
  if (table[ABSX].length == 0) {
    *error = ERROR_PARSE_UNSUPPORTED_ADDR_MODE;
    goto skip_op_absy;
  }
  if ((new_cursor = parse_op_absolutex(cursor, &val, error)) != cursor) {
    *ins_out = (instruction){.length = table[ABSX].length, .opcode = table[ABSX].opcode, .byte1 = val & 0xff, .byte2 = (val >> 8) & 0xff};
    return new_cursor;
  }
  if (*error == ERROR_PARSE_EXPECTED_COMMA) goto skip_op_abs;
  if (*error == ERROR_PARSE_EXPECTED_X) goto skip_op_absy;
  if (*error == ERROR_PARSE_EXPECTED_NUMBER) goto skip_op_absy;
  if (*error != ERROR_PARSE_NONE) return orig;

skip_op_absy:
  // absolute, Y
  if (table[ABSY].length == 0) {
    *error = ERROR_PARSE_UNSUPPORTED_ADDR_MODE;
    goto skip_op_abs;
  }
  if ((new_cursor = parse_op_absolutey(cursor, &val, error)) != cursor) {
    *ins_out = (instruction){.length = table[ABSY].length, .opcode = table[ABSY].opcode, .byte1 = val & 0xff, .byte2 = (val >> 8) & 0xff};
    return new_cursor;
  }
  if (*error == ERROR_PARSE_EXPECTED_NUMBER) goto skip_op_abs;
  if (*error != ERROR_PARSE_NONE) return orig;

skip_op_abs:
  // absolute
  if (table[ABS].length == 0) {
    *error = ERROR_PARSE_UNSUPPORTED_ADDR_MODE;
    goto skip_op_implied;
  }
  if ((new_cursor = parse_op_absolute(cursor, &val, error)) != cursor) {
    *ins_out = (instruction){.length = table[ABS].length, .opcode = table[ABS].opcode, .byte1 = val & 0xff, .byte2 = (val >> 8) & 0xff};
    return new_cursor;
  }
  if (*error != ERROR_PARSE_NONE) return orig;

skip_op_implied:
  // implied
  if (table[IMP].length == 0) {
    *error = ERROR_PARSE_UNSUPPORTED_ADDR_MODE;
    return orig;
  }
  *ins_out = (instruction){.length = table[IMP].length, .opcode = table[IMP].opcode, .byte1 = val & 0xff, .byte2 = (val >> 8) & 0xff};
  *error = ERROR_PARSE_NONE;

  return orig;
}
