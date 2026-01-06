#include "munit.h"

MunitResult fileutil_read_test(const MunitParameter params[], void* fixture);

MunitResult parse_util_parse_number_test(const MunitParameter params[], void* fixture);
MunitResult parse_util_alphalen_test(const MunitParameter *params, void *fixture);
MunitResult parse_util_isnewline_test(const MunitParameter *params, void *fixture);
MunitResult parse_util_predlen_test(const MunitParameter *params, void *fixture);

MunitResult parse_operand_accumulator_test(const MunitParameter *params, void *fixture);
MunitResult parse_operand_immediate_test  (const MunitParameter *params, void *fixture);
MunitResult parse_operand_zp_test         (const MunitParameter *params, void *fixture);
MunitResult parse_operand_zpx_test        (const MunitParameter *params, void *fixture);
MunitResult parse_operand_absolute_test   (const MunitParameter *params, void *fixture);
MunitResult parse_operand_absolutex_test  (const MunitParameter *params, void *fixture);
MunitResult parse_operand_absolutey_test  (const MunitParameter *params, void *fixture);
MunitResult parse_operand_indirectx_test  (const MunitParameter *params, void *fixture);
MunitResult parse_operand_indirecty_test  (const MunitParameter *params, void *fixture);

MunitResult parse_instruction_ora_immediate_test(const MunitParameter *params, void *fixture);
MunitResult parse_instruction_ora_zpx_test      (const MunitParameter *params, void *fixture);
MunitResult parse_instruction_ora_zp_test       (const MunitParameter *params, void *fixture);
MunitResult parse_instruction_ora_absolutex_test(const MunitParameter *params, void *fixture);
MunitResult parse_instruction_ora_absolutey_test(const MunitParameter *params, void *fixture);
MunitResult parse_instruction_ora_indirectx_test(const MunitParameter *params, void *fixture);
MunitResult parse_instruction_ora_indirecty_test(const MunitParameter *params, void *fixture);
MunitResult parse_instruction_ora_absolute_test (const MunitParameter *params, void *fixture);

MunitResult parse_directive_org_test(const MunitParameter *params, void *fixture);
MunitResult parse_directive_byte_test(const MunitParameter *params, void *fixture);

MunitResult parse_label_named_test(const MunitParameter *params, void *fixture);
MunitResult parse_label_unnamed_test(const MunitParameter *params, void *fixture);

MunitResult parser_test(const MunitParameter *params, void *fixture);
