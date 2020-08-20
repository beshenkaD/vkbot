TARGET   	?= bot
SRC_DIRS 	?= src
INC_DIRS  ?= include
BIN_DIR   ?= bin
BUILD_DIR ?= build

CFLAGS  ?= $(INC_FLAGS) -MMD -MP -O2 -march=native
LDFLAGS ?= -flto=thin
LDLIBS  ?= -lcurl -lsqlite3 -lcjson

SRCS := $(shell find $(SRC_DIRS) -name *.c)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_FLAGS  := $(addprefix -I,$(INC_DIRS))
EXECUTABLE := $(BIN_DIR)/$(TARGET)


all: $(SRCS) $(EXECUTABLE)

$(BIN_DIR)/$(TARGET): $(OBJS)
	$(MKDIR_P) $(BIN_DIR)
	$(CC) $(OBJS) -o $@ $(LDFLAGS) $(LDLIBS)

$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

debug: CFLAGS  := $(INC_FLAGS) -Wextra -Wall -MMD -MP -O2 -g -fno-omit-frame-pointer
debug: LDFLAGS := 
debug: all

release: all

.PHONY: clean
clean:
	$(RM) -r $(BUILD_DIR) $(BIN_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
