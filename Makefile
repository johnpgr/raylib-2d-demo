CC = gcc
CFLAGS = -std=c2x -Wall -Wextra -O2 -Ilib
CFLAGS_DEBUG = -std=c2x -Wall -Wextra -g -Ilib
LIBS = -lraylib -lraygui -lGL -lm -lpthread -ldl -lrt -lX11
RAYGUI_LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Directories
BIN_DIR = bin
TARGET = $(BIN_DIR)/main
INSTALL_HEADER_DIR = /usr/local/include
INSTALL_LIB_DIR = /usr/local/lib
RAYGUI_URL = https://raw.githubusercontent.com/raysan5/raygui/master/src/raygui.h
RAYGUI_SO = $(BIN_DIR)/libraygui.so

# Source files
SRCS = src/main.c
OBJS = $(SRCS:src/%.c=$(BIN_DIR)/%.o)

# Default target
all: $(BIN_DIR) $(TARGET)

# Create bin directory if it doesn't exist
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Download and install raylib and raygui
install: $(BIN_DIR)
	@echo "Installing raylib from source..."
	@mkdir -p /tmp/raylib_build
	@git clone https://github.com/raysan5/raylib.git /tmp/raylib_build
	@cd /tmp/raylib_build/src && make PLATFORM=PLATFORM_DESKTOP
	@sudo make install -C /tmp/raylib_build/src
	@rm -rf /tmp/raylib_build
	@echo "Raylib installation complete!"
	@echo "Downloading raygui.h..."
	@curl -s $(RAYGUI_URL) -o $(BIN_DIR)/raygui.h
	@echo "Building shared library..."
	@cp $(BIN_DIR)/raygui.h $(BIN_DIR)/raygui.c
	$(CC) -o $(RAYGUI_SO) $(BIN_DIR)/raygui.c -shared -fpic -DRAYGUI_IMPLEMENTATION $(RAYGUI_LIBS)
	@echo "Installing to system directories..."
	@sudo cp $(BIN_DIR)/raygui.h $(INSTALL_HEADER_DIR)/
	@sudo cp $(RAYGUI_SO) $(INSTALL_LIB_DIR)/
	@sudo ldconfig
	@rm $(BIN_DIR)/raygui.c
	@echo "Installation complete!"

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

.PHONY: all clean run install
