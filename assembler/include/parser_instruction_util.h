#ifndef PARSER_INSTRUCTION_UTIL_H
#define PARSER_INSTRUCTION_UTIL_H
#include "parser_instruction.h"
typedef enum {
  ACC = 0,
  IMM,
  ZP,
  ZPX,
  ABS,
  ABSX,
  ABSY,
  INDX,
  INDY,
  // IMP and REL are mutually exclusive addressing modes
  //   an instruction can't have both
  IMP,
  REL,
  AM_LAST_
} addr_mode;

const char* ins_util_parse_operand(const char* cursor, instruction* ins_out, instruction table[AM_LAST_], error_parse*);

#endif
