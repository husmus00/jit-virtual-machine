# Makefile - build for x86_64 and aarch64
SRCDIR := src
BUILDDIR := build
CC := clang
CFLAGS := -Wall -Wextra -O2 -g -z execstack
LDFLAGS := # -fsanitize=address

SRCS := $(wildcard $(SRCDIR)/*.c)

# x86_64 settings
x86_64_CC := $(CC)
x86_64_CFLAGS := $(CFLAGS)
x86_64_LDFLAGS := $(LDFLAGS)
x86_64_TARGET := $(BUILDDIR)/x86_64/vm
x86_64_OBJS := $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/x86_64/%.o,$(SRCS))

# aarch64 settings
aarch64_CC := $(CC)
aarch64_CFLAGS := $(CFLAGS) -target aarch64-linux-gnu --sysroot=/usr/aarch64-linux-gnu
aarch64_LDFLAGS := $(LDFLAGS) -target aarch64-linux-gnu --sysroot=/usr/aarch64-linux-gnu
aarch64_TARGET := $(BUILDDIR)/aarch64/vm
aarch64_OBJS := $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/aarch64/%.o,$(SRCS))

.PHONY: all clean x86_64 aarch64 run-x86_64 run-aarch64

# Build both by default
all: x86_64 aarch64

# x86_64 build
x86_64: $(x86_64_TARGET)

$(x86_64_TARGET): $(x86_64_OBJS)
	@mkdir -p $(dir $@)
	$(x86_64_CC) $(x86_64_LDFLAGS) -o $@ $^

$(BUILDDIR)/x86_64/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(x86_64_CC) $(x86_64_CFLAGS) -c $< -o $@

# aarch64 build
aarch64: $(aarch64_TARGET)

$(aarch64_TARGET): $(aarch64_OBJS)
	@mkdir -p $(dir $@)
	$(aarch64_CC) $(aarch64_LDFLAGS) -o $@ $^

$(BUILDDIR)/aarch64/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(aarch64_CC) $(aarch64_CFLAGS) -c $< -o $@

# Run targets
run-x86_64: x86_64
	$(x86_64_TARGET)

run-aarch64: aarch64
	qemu-aarch64 -L /usr/aarch64-linux-gnu $(aarch64_TARGET)

clean:
	rm -rf $(BUILDDIR)