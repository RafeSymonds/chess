# Change 'executable' to match the command name given in the project spec.
EXECUTABLE = main

# Disable built-in rules
.SUFFIXES:

# Designate which compiler to use
CXX = g++

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

# Create the build directory if it doesn't exist
$(shell mkdir -p $(BUILD_DIR))

# Default Flags
CXXFLAGS = -std=c++23 -Wconversion -Wall -Werror -Wextra -pedantic -I$(INCLUDE_DIR)

# List of sources used in the project
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Default target to build debug
all: debug

# Debug target (no object files)
debug: CXXFLAGS += -g3 -DDEBUG -fsanitize=address -fsanitize=undefined
debug: $(BUILD_DIR)/$(EXECUTABLE)_debug

$(BUILD_DIR)/$(EXECUTABLE)_debug: $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $@

.PHONY: debug

# Release target
release: CXXFLAGS += -Ofast -DNDEBUG
release: $(BUILD_DIR)/$(EXECUTABLE)

$(BUILD_DIR)/$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@

# Rule for creating objects
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: release

# Clean target
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/$(EXECUTABLE) $(BUILD_DIR)/$(EXECUTABLE)_debug
	rm -rf $(BUILD_DIR)  # Remove entire build directory
