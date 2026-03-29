# ============================================================
# Makefile  –  TS2C Compiler build rules
# TypeScript → C translator
# ============================================================
# Usage:
#   make          – build the compiler (ts2c executable)
#   make run      – build and run with the sample test file
#   make clean    – remove generated files
# ============================================================

CC      = gcc
CFLAGS  = -Wall -Wextra -g
LDFLAGS = -lm

# Tool binaries (adjust paths if Flex/Bison are not on PATH)
FLEX    = flex
BISON   = bison

# Source files
LEX_SRC    = ts_lexer.l
PARSER_SRC = parser.y
AST_SRC    = ast.c
CODEGEN_SRC= codegen.c
OPT_SRC    = optimizer.c

# Generated files
LEX_C      = lex.yy.c
PARSER_C   = parser.tab.c
PARSER_H   = parser.tab.h

# Object files
OBJS = $(PARSER_C:.c=.o) $(LEX_C:.c=.o) $(AST_SRC:.c=.o) \
       $(CODEGEN_SRC:.c=.o) $(OPT_SRC:.c=.o)

TARGET = ts2c

# ── Default target ────────────────────────────────────────
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# ── Bison (parser + header) ───────────────────────────────
$(PARSER_C) $(PARSER_H): $(PARSER_SRC)
	$(BISON) -d -o $(PARSER_C) $(PARSER_SRC)

# ── Flex ──────────────────────────────────────────────────
$(LEX_C): $(LEX_SRC) $(PARSER_H)
	$(FLEX) -o $(LEX_C) $(LEX_SRC)

# ── C compilation ─────────────────────────────────────────
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# ── Run with sample input ─────────────────────────────────
run: $(TARGET)
	./$(TARGET) test_input.ts output.c
	@echo ""
	@echo "=== Generated C file (output.c) ==="
	@cat output.c

# ── Clean ─────────────────────────────────────────────────
clean:
	rm -f $(OBJS) $(TARGET) $(LEX_C) $(PARSER_C) $(PARSER_H) \
	      output.c *.o

.PHONY: all run clean
