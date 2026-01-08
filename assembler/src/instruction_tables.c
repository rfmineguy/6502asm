#include "instruction_tables.h"

/*
 * tables that help the parser understand what is legal, and
 * what instruction to generate
 *
 * length = 0 indicates an illegal addressing mode
 */
instruction ins_table_ora[AM_LAST_] = {
  [ACC ] = {.length = 0 },
  [INDX] = {.length = 2, .opcode = 0x01 },
  [INDY] = {.length = 2, .opcode = 0x11 },
  [IMM ] = {.length = 2, .opcode = 0x09 },
  [ZP  ] = {.length = 2, .opcode = 0x05 },
  [ZPX ] = {.length = 2, .opcode = 0x15 },
  [ABSX] = {.length = 3, .opcode = 0x1D },
  [ABSY] = {.length = 3, .opcode = 0x19 },
  [ABS ] = {.length = 3, .opcode = 0x0D },
  [IMP ] = {.length = 0 },
  [REL ] = {.length = 0 },
};

instruction ins_table_pha[AM_LAST_] = {
  [ACC ] = {.length = 0 },
  [IMM ] = {.length = 0 },
  [ZP  ] = {.length = 0 },
  [ZPX ] = {.length = 0 },
  [ABS ] = {.length = 0 },
  [ABSX] = {.length = 0 },
  [ABSY] = {.length = 0 },
  [INDX] = {.length = 0 },
  [INDY] = {.length = 0 },
  [IMP ] = {.length = 1, .opcode = 0x48 },
  [REL ] = {.length = 0 },
};

instruction ins_table_asl[AM_LAST_] = {
  [ACC ] = {.length = 1, .opcode = 0x0A },
  [INDX] = {.length = 0 },
  [INDY] = {.length = 0 },
  [IMM ] = {.length = 0 },
  [ZP  ] = {.length = 2, .opcode = 0x06 },
  [ZPX ] = {.length = 2, .opcode = 0x16 },
  [ABSX] = {.length = 3, .opcode = 0x1e },
  [ABSY] = {.length = 0 },
  [ABS ] = {.length = 3, .opcode = 0x0e },
  [IMP ] = {.length = 0 },
  [REL ] = {.length = 0 },
};

instruction ins_table_bcc[AM_LAST_] = {
  [ACC ] = {.length = 0 },
  [INDX] = {.length = 0 },
  [INDY] = {.length = 0 },
  [IMM ] = {.length = 0 },
  [ZP  ] = {.length = 0 },
  [ZPX ] = {.length = 0 },
  [ABSX] = {.length = 0 },
  [ABSY] = {.length = 0 },
  [ABS ] = {.length = 0 },
  [IMP ] = {.length = 0 },
  [REL ] = {.length = 2, .opcode = 0x90}
};
