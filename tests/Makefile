ROOT_MAKEFILE := $(CURDIR)
THIS_MAKEFILE := $(lastword $(MAKEFILE_LIST))
THIS_DIR   := $(dir $(abspath $(THIS_MAKEFILE)))

INCLUDE ?= -I$(THIS_DIR)/../assembler/include
OUT ?= out
TESTS_OUTDIR := $(OUT)
TESTFILE_DIR := $(OUT)/testfiles

TESTFILES    := $(TESTFILE_DIR)/test1.txt\
                $(TESTFILE_DIR)/test2.txt\
                $(TESTFILE_DIR)/test3.txt\

TESTS_SOURCES := $(THIS_DIR)/src/parser_directive.c\
                 $(THIS_DIR)/src/parser_operand.c\
                 $(THIS_DIR)/src/parser_util.c\
                 $(THIS_DIR)/src/parser_instruction_ora.c\
                 $(THIS_DIR)/src/fileutil.c\
                 $(THIS_DIR)/src/munit.c\
                 $(THIS_DIR)/src/test_main.c

tests_always:
	mkdir -p $(OUT)
	$(file test.txt,$(test1txt))

$(OUT)/test: $(TESTS_SOURCES) | tests_always $(OUT)/lib65asm.a $(TESTFILES)
	$(CC) -o $@ $^ $(INCLUDE) -L$(OUT) -l65asm -DGIT_ROOT_BASELINE="\"$(ROOT_MAKEFILE)\""

$(TESTFILES):
	source tests/genfiles.sh && gentestfiles $(TESTFILE_DIR)
