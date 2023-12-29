# common.mk
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c99
LIBS = -lzmq

# Protocol library
SRC_DIR = src
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(SRC_FILES:.c=.o)
TARGET = libprotocol.so

# Default target
all: $(TARGET)

# Build the shared library
$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -shared -o $@ $^ $(LIBS)

# Compile each source file to an object file
%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -fPIC -c -o $@ $<

# Clean rule
clean:
	rm -f $(TARGET) $(OBJ_FILES)
