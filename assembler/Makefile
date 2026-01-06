THIS_MAKEFILE := $(lastword $(MAKEFILE_LIST))
THIS_DIR   := $(dir $(abspath $(THIS_MAKEFILE)))

# Variables
INCLUDE := -I$(THIS_DIR)/include
SRC     := $(THIS_DIR)/src
OUT     ?= out
OBJ_OUT ?= $(OUT)/lib65asm
CFG_GGO := $(THIS_DIR)/config.ggo

SOURCES := $(SRC)/fileutil.c\
           $(SRC)/parser.c\
           $(SRC)/parser_directive.c\
           $(SRC)/parser_instruction.c\
           $(SRC)/parser_instruction_util.c\
           $(SRC)/instruction_tables.c\
           $(SRC)/parser_operand.c\
           $(SRC)/parser_label.c\
           $(SRC)/parser_util.c\
           $(SRC)/parser_errors.c

MAIN    := $(SRC)/main.c\
           $(SRC)/cmdline.c

DRIVER  := $(THIS_DIR)/driver.c

OBJS    := $(patsubst $(SRC)/%.c, $(OBJ_OUT)/%.c.o, $(SOURCES))

CFLAGS  := -ggdb -Wall -Werror -pedantic -std=c11

.PHONY: always clean
.PHONY: build
.PHONY: gengetopt

# Utility
always:
	mkdir -p $(OBJ_OUT)

clean:
	+rm -r $(OUT)

# Compile
build: always gengetopt $(OUT)/lib65asm.a
$(OUT)/lib65asm.a: $(OBJS) | always
	ar rcs $@ $^

$(OBJ_OUT)/%.c.o: $(SRC)/%.c | always
	$(CC) -c $^ -o $@ -ggdb $(INCLUDE) $(CFLAGS)

$(OUT)/main: $(MAIN) | $(OUT)/lib65asm.a
	$(CC) $^ -o $@ -l65asm -L$(OUT) $(INCLUDE) $(CFLAGS)

$(OUT)/driver: $(DRIVER) | $(OUT)/lib65asm.a
	$(CC) $^ -o $@ -l65asm -L$(OUT) $(INCLUDE) $(CFLAGS)

# Gengetopt
gengetopt: $(INCLUDE)/65asm/cmdline.h $(SRC)/cmdline.c
$(INCLUDE)/65asm/cmdline.h $(SRC)/cmdline.c: $(CFG_GGO)
	gengetopt \
		--input=$(CFG_GGO) \
		--header-output-dir=$(SRC) \
		--src-output-dir=$(SRC)
