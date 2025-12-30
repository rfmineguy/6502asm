THIS_MAKEFILE := $(lastword $(MAKEFILE_LIST))
THIS_DIR   := $(dir $(abspath $(THIS_MAKEFILE)))

INCLUDE ?= $(THIS_DIR)/../assembler/include
OUT ?= out
TESTS_OUTDIR := $(OUT)

TESTS_SOURCES := $(THIS_DIR)/src/parser_directive.c\
                 $(THIS_DIR)/src/parser_operand.c\
                 $(THIS_DIR)/src/parser_util.c\
                 $(THIS_DIR)/src/parser_instruction_ora.c\
                 $(THIS_DIR)/src/munit.c\
                 $(THIS_DIR)/src/test_main.c

tests_always:
	mkdir -p $(OUT)

$(OUT)/test: $(TESTS_SOURCES) | tests_always $(OUT)/lib65asm.a
	$(CC) -o $@ $^ -I$(INCLUDE) -L$(OUT) -l65asm
