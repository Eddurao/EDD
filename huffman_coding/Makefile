# Compiler
CC = g++

# Compiler flags
CFLAGS = -Wall -g -Iinclude

# Executable names
ENCODER = encoder
DECODER = decoder

# Source directories
SRC_DIR = src
INCLUDE_DIR = include

# Source files
SRC_ENCODER = $(SRC_DIR)/encoder.cpp $(SRC_DIR)/Huffman.cpp
SRC_DECODER = $(SRC_DIR)/decoder.cpp $(SRC_DIR)/Huffman.cpp

# Object files
OBJ_ENCODER = $(SRC_ENCODER:.cpp=.o)
OBJ_DECODER = $(SRC_DECODER:.cpp=.o)

# Default target
all: $(ENCODER) $(DECODER)

# Pattern rule to build object files
$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCLUDE_DIR)/Huffman.hpp
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to build encoder
$(ENCODER): $(OBJ_ENCODER)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to build decoder
$(DECODER): $(OBJ_DECODER)
	$(CC) $(CFLAGS) -o $@ $^

# Clean up
clean:
	rm -f $(OBJ_ENCODER) $(OBJ_DECODER) $(ENCODER) $(DECODER)

.PHONY: all clean
