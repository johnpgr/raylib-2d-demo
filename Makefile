CC = gcc
CFLAGS = -std=c2x -Wall -Wextra -O2
CFLAGS_DEBUG = -std=c23 -Wall -Wextra -g
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Directories
BIN_DIR = bin
TARGET = $(BIN_DIR)/main

# Source files
SRCS = src/main.c
OBJS = $(SRCS:src/%.c=$(BIN_DIR)/%.o)

# Default target
all: $(BIN_DIR) $(TARGET)

# Create bin directory if it doesn't exist
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Linking
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LIBS)

# Compilation
$(BIN_DIR)/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJS)
	rm -rf $(BIN_DIR)

# Run the Program
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
