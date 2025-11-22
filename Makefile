# Makefile - build all .c files in src into build/vm

SRCDIR := src
BUILDDIR := build
TARGET := $(BUILDDIR)/vm

CC := gcc
CFLAGS := -Wall -Wextra -O2 -g 
LDFLAGS := # -fsanitize=address

SRCS := $(wildcard $(SRCDIR)/*.c)
OBJS := $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SRCS))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BUILDDIR)
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR)