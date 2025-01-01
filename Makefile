CC = gcc
CFLAGS = -std=c23 -Wall -Wextra -O2
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Output binary name
TARGET = game

# Source files
SRCS = src/main.c
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LIBS)

# Compilation
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJS) $(TARGET)

# Run the game
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
