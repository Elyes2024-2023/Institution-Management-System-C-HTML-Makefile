# Copyright (c) 2024-2025 ELYES. All rights reserved.
# 
# This Makefile is part of the ELYES Employee and Student Management System Library.
# Created by ELYES

# Compiler settings - Done by ELYES
CC = gcc
CFLAGS = -Wall -Wextra -fPIC
LDFLAGS = -shared

# Version information - Done by ELYES
VERSION = 1.0.0
LIB_NAME = libelyes_management.so.$(VERSION)
SONAME = libelyes_management.so

# Source files - Done by ELYES
SRCS = management_lib.c management_system.c student.c employee.c utils.c
OBJS = $(SRCS:.c=.o)

# Build configurations - Done by ELYES
DEBUG_CFLAGS = -g -DDEBUG -O0
RELEASE_CFLAGS = -O2 -DNDEBUG
PROFILE_CFLAGS = -g -pg -O1

# Default target - Done by ELYES
all: release

# Debug build - Done by ELYES
debug: CFLAGS += $(DEBUG_CFLAGS)
debug: all

# Release build - Done by ELYES
release: CFLAGS += $(RELEASE_CFLAGS)
release: all

# Profile build - Done by ELYES
profile: CFLAGS += $(PROFILE_CFLAGS)
profile: all

# Compile source files to object files - Done by ELYES
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Create shared library - Done by ELYES
$(LIB_NAME): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)
	ln -sf $@ $(SONAME)

# Clean build files - Done by ELYES
clean:
	rm -f $(OBJS) $(LIB_NAME) $(SONAME)
	rm -f *.gcno *.gcda *.gcov

# Install library - Done by ELYES
install: $(LIB_NAME)
	install -d /usr/local/lib
	install -m 755 $(LIB_NAME) /usr/local/lib/
	ln -sf /usr/local/lib/$(LIB_NAME) /usr/local/lib/$(SONAME)
	ldconfig

# Uninstall library - Done by ELYES
uninstall:
	rm -f /usr/local/lib/$(LIB_NAME) /usr/local/lib/$(SONAME)
	ldconfig

# Run tests - Done by ELYES
test: debug
	./run_tests

# Generate coverage report - Done by ELYES
coverage: CFLAGS += --coverage
coverage: clean test
	gcov $(SRCS)
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory coverage_report

.PHONY: all clean install uninstall debug release profile test coverage 