# Variables
INCLUDE := include
SRC     := src
OUT     ?= out
CFG_GGO ?= config.ggo

SOURCES := $(SRC)/main.c\
					 $(SRC)/fileutil.c\
					 $(SRC)/cmdline.c

.PHONY: always clean
.PHONY: build
.PHONY: gengetopt

# Utility
always:
	mkdir $(OUT)

clean:
	+rm -r $(OUT)

# Compile
build: always gengetopt $(OUT)/65asm
$(OUT)/65asm: $(SOURCES)
	$(CC) $^ -o $@ -I$(INCLUDE)


# Gengetopt
gengetopt: $(INCLUDE)/65asm/cmdline.h $(SRC)/cmdline.c
$(INCLUDE)/65asm/cmdline.h $(SRC)/cmdline.c: $(CFG_GGO)
	gengetopt \
		--input=$(CFG_GGO) \
		--header-output-dir=$(SRC) \
		--src-output-dir=$(SRC)
