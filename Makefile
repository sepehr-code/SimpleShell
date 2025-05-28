# Makefile for SimpleShell

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -g
TARGET = simpleshell
SOURCES = main.c parser.c executor.c builtins.c
OBJECTS = $(SOURCES:.c=.o)
HEADERS = shell.h

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

# Compile source files to object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)

# Install (copy to /usr/local/bin)
install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/

# Uninstall
uninstall:
	sudo rm -f /usr/local/bin/$(TARGET)

# Run the shell
run: $(TARGET)
	./$(TARGET)

# Debug build
debug: CFLAGS += -DDEBUG -O0
debug: $(TARGET)

# Release build
release: CFLAGS += -O2 -DNDEBUG
release: clean $(TARGET)

# Check for memory leaks with valgrind
memcheck: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)

# Format code (requires clang-format)
format:
	clang-format -i *.c *.h

# Show help
help:
	@echo "Available targets:"
	@echo "  all       - Build the shell (default)"
	@echo "  clean     - Remove build artifacts"
	@echo "  run       - Build and run the shell"
	@echo "  debug     - Build with debug flags"
	@echo "  release   - Build optimized release version"
	@echo "  install   - Install to /usr/local/bin"
	@echo "  uninstall - Remove from /usr/local/bin"
	@echo "  memcheck  - Run with valgrind memory checking"
	@echo "  format    - Format code with clang-format"
	@echo "  help      - Show this help message"

.PHONY: all clean install uninstall run debug release memcheck format help 