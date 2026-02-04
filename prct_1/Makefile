# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++2b -w

# Program name (change this to your desired program name)
PROGRAM_NAME := inference

# Source files (all .cc files in the current directory)
SOURCES := $(wildcard *.cc)

# Header files (all .h files in the current directory)
HEADERS := $(wildcard *.h)

# Default target
all: $(PROGRAM_NAME)

# Direct compilation without .o files
$(PROGRAM_NAME): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $(SOURCES)

# Clean rule
clean:
	rm -rf $(PROGRAM_NAME)

# PHONY target to prevent conflicts with files named "clean"
.PHONY: clean

# Run the program
run: $(PROGRAM_NAME)
	./$(PROGRAM_NAME)