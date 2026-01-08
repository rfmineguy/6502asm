#include "parser_instruction.h"
#include "parser_instruction_util.h"
#include "instruction_tables.h"
#include "parser_errors.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

/* implementation map
 * ins_parse_ora -> assembler/src/parser_instruction_ora.c
 */

const char* ins_parse_instruction(const char* cursor, instruction* ins_out, error_parse* error) {
  assert(error && "error must not be null");
  const char* orig = cursor;
  const char* new_cursor;
  instruction* ins_table = NULL;
  if (strncmp(cursor, "ora", 3) == 0) ins_table = ins_table_ora;
  if (strncmp(cursor, "pha", 3) == 0) ins_table = ins_table_pha;
  if (strncmp(cursor, "asl", 3) == 0) ins_table = ins_table_asl;
  if (strncmp(cursor, "bcc", 3) == 0) ins_table = ins_table_bcc;
  if (ins_table) {
    cursor+=3;
    if ((new_cursor = ins_util_parse_operand(cursor, ins_out, ins_table, error)) == cursor) {
      return orig;
    }
    return new_cursor;
  }
  return orig;

  assert(0 && "instruction not implemented");
  return orig;
}
