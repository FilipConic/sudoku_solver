GCC = gcc
FLAGS = -Wall -Wextra -pedantic -ggdb -Wno-char-subscripts

SRCS = sudo_solve.c sudoku.c

TARGET = sudo_solve
BUILD_DIR = ./build

OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
ifeq ($(wildcard $(BUILD_DIR)),)
	mkdir -p $(BUILD_DIR)
endif

$(TARGET): $(OBJS)
	$(GCC) $(FLAGS) $^ -o $@

$(BUILD_DIR)/%.o: %.c
	$(GCC) $(FLAGS) -c $< -o $@

clear:
	rm -rf $(BUILD_DIR) $(TARGET)
