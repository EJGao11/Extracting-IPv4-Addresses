CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -O2
TARGET = ipExtractor
SRC = ipExtractor.cpp
TEST_DIR = ../testing
INPUT = $(TEST_DIR)/ipv4.input
EXPECTED = $(TEST_DIR)/answer.txt
OUTPUT = $(TEST_DIR)/output.txt

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

$(OUTPUT): $(TARGET) $(INPUT)
	@./$(TARGET) < $(INPUT) > $(OUTPUT) 2>&1

test: $(OUTPUT)
	@diff -u $(EXPECTED) $(OUTPUT)
	@echo "Test passed: IPv4 sample suite"

clean:
	rm -f $(TARGET) $(OUTPUT)

.PHONY: all test clean
