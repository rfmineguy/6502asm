# Variables
INCLUDE := include
SRC     := src
OUT     ?= out
CFG_GGO ?= config.ggo

.PHONY: always clean
.PHONY: gengetopt

# Utility
always:
	mkdir $(OUT)

clean:
	+rm -r $(OUT)

# Gengetopt
gengetopt: $(INCLUDE)/65asm/cmdline.h $(SRC)/cmdline.c
$(INCLUDE)/65asm/cmdline.h $(SRC)/cmdline.c: $(CFG_GGO)
	gengetopt \
		--input=$(CFG_GGO) \
		--header-output-dir=$(SRC) \
		--src-output-dir=$(SRC)
