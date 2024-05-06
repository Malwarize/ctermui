# Compiler
CC = gcc
# Compiler flags
CFLAGS = -Wall -Wextra -pedantic -std=c99 
DEBUG = -g
# Directories
SRCDIR = src
INCDIR = include
OBJDIR = src/obj
LIBDIR = build
TESTDIR = test
# Source files
SRCS = $(wildcard $(SRCDIR)/*.c)
# Object files
OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
# Library
LIBNAME = ctermui
LIB = $(LIBDIR)/lib$(LIBNAME).a
# Test files
TESTSRCS = $(wildcard $(TESTDIR)/*.c)
TESTBINS = $(TESTSRCS:$(TESTDIR)/%.c=$(TESTDIR)/%.out)
# Targets
all: $(LIB) $(TESTBINS)

$(TESTDIR)/%.out: $(TESTDIR)/%.c $(LIB)
	@mkdir -p $(TESTDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) -o $@ $< -L$(LIBDIR) -l$(LIBNAME)

run_tests: $(TESTBINS)
	@for testbin in $(TESTBINS); do \
		echo "Running $$testbin"; \
		$$testbin; \
	done

# Rule for creating object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

# Rule for creating the static library
$(LIB): $(OBJS)
	@mkdir -p $(LIBDIR)
	ar rcs $@ $(OBJS)

# ENV run file, take the first argument as the file name
# e.g. make run file=example
run_file: $(LIB)
	$(CC) $(CFLAGS) $(DEBUG) -I$(INCDIR) -o $(file).out $(file) -L$(LIBDIR) -l$(LIBNAME)
	$(file).out

	
# Clean
clean:
	rm -rf $(OBJDIR) $(LIBDIR) $(TESTDIR)/*.out 
.PHONY: all run_tests clean

