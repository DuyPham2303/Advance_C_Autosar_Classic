# Makefile (macOS, clang + CommonCrypto headers)
CC      := clang
CFLAGS  := -std=c11 -O2 -Wall -Wextra -Iinclude
LDFLAGS :=
SRC     := $(wildcard src/*.c)

OUTDIR  := bin
BIN     := $(OUTDIR)/secoc_sim

.PHONY: all clean

all: $(BIN)

# Tạo bin/ trước khi link (order-only prerequisite với dấu '|')
$(BIN): $(SRC) | $(OUTDIR)
	$(CC) $(CFLAGS) $(SRC) -o $@ $(LDFLAGS)

$(OUTDIR):
	@mkdir -p $@

clean:
	rm -f $(BIN)
