# Makefile for UART Communication Simulator

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11
LDFLAGS = -lpthread

# Target executable
TARGET = uart_simulator

# Source files
SRC = uart_simulator.c

# Default target
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

# Clean up
clean:
	rm -f $(TARGET)

# Run the simulator
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
