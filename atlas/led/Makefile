SRC_DIR   := src
INC_DIR   := include
BUILD_DIR := build

TARGET    := $(BUILD_DIR)/iot

SRCS      := main.cpp
include $(SRC_DIR)/Makefile

OBJS      := $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(SRCS))

LD        := g++
LDFLAGS   := -g -O2
CC        := g++
CFLAGS    := -g -c -I$(INC_DIR) -O2 -MMD -Wall -Werror

.PHONY: run clean

# .c -> .o
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -o $@ $<

# .o -> exe
$(TARGET): $(OBJS)
	@$(LD) $(LDFLAGS) -o $@ $^

run: $(TARGET)
	@$(TARGET) $(LED)

clean:
	@rm -rf $(BUILD_DIR)
