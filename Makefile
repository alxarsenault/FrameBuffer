
PROGRAM_NAME = fb

## Platform name.
UNAME := $(shell uname)

## 32 vs 64 bits.
ARCH := $(shell getconf LONG_BIT)

## x86 vs arm.
PROCESSOR_TYPE := $(shell uname -m)

CC = gcc
CC_FLAGS = -std=c99 
INCLUDE_SRC = -Iinclude/

LINKER_INC = 
LINKER_FLAGS = -lm

OBJ_DIR = build
SRC_DIR = source

C_FILES := $(wildcard source/*.c)
OBJ_FILES := $(addprefix build/,$(notdir $(C_FILES:.c=.o)))

all: createDir $(OBJ_FILES)
	$(CC) $(CC_FLAGS) $(OBJ_FILES) $(LINKER_INC) $(LINKER_FLAGS) -o $(PROGRAM_NAME)

createDir:
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CC_FLAGS) $(INCLUDE_SRC) -c -o $@ $<

clean:
	rm -f $(OBJ_DIR)/*.o
