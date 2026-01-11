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
                $(TESTFILE_DIR)/test4.txt\

TESTFILES_C := $(patsubst $(TESTFILE_DIR)/%.txt, $(TESTFILE_DIR)/%.h, $(TESTFILES))

TESTS_SOURCES := $(THIS_DIR)/src/parser_directive.c\
                 $(THIS_DIR)/src/parser_operand.c\
                 $(THIS_DIR)/src/parser_util.c\
                 $(THIS_DIR)/src/parser_instruction_ora.c\
                 $(THIS_DIR)/src/parser_instruction_pha.c\
                 $(THIS_DIR)/src/parser_instruction_asl.c\
                 $(THIS_DIR)/src/parser_instruction_bcc.c\
                 $(THIS_DIR)/src/parser_label.c\
                 $(THIS_DIR)/src/fileutil.c\
                 $(THIS_DIR)/src/munit.c\
                 $(THIS_DIR)/src/test_main.c

tests_always:
	mkdir -p $(OUT)
	mkdir -p $(TESTFILE_DIR)

$(OUT)/test: $(TESTS_SOURCES) | tests_always $(OUT)/lib65asm.a $(TESTFILES_C)
	$(CC) -o $@ $^ $(INCLUDE) -Iout/testfiles -L$(OUT) -l65asm -DTESTFILE_DIR="\"$(TESTFILE_DIR)\""

$(TESTFILES):
	python3 $(THIS_DIR)/genfiles.py --outdir=$(TESTFILE_DIR) -v

#$(TESTFILES_C): $(TESTFILES) | $(TESTFILES)
$(TESTFILE_DIR)/%.h: $(TESTFILE_DIR)/%.txt
	xxd -i -n $(notdir $^) $< > $@
