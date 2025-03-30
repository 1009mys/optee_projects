CROSS_COMPILE ?= aarch64-linux-gnu-

# If _HOST or _TA specific compilers are not specified, then use CROSS_COMPILE
HOST_CROSS_COMPILE ?= $(CROSS_COMPILE)

CC      = $(CROSS_COMPILE)gcc
LD      = $(CROSS_COMPILE)ld
AR      = $(CROSS_COMPILE)ar
NM      = $(CROSS_COMPILE)nm
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
READELF = $(CROSS_COMPILE)readelf

INCLUDE = ./include 
SRC_DIR = ./src
OBJ_DIR = ./obj

BUILD_DIR = build

TARGET = $(BUILD_DIR)/my_onnxruntime_lib_test

SRCS = $(wildcard $(SRC_DIR)/*.c)

OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)

OBJECTS = $(patsubst $(SRC_DIR)/%.o,$(OBJ_DIR)/%.o,$(OBJS))

DEPS = $(OBJECTS:.o=.d)

CFLAGS = -I$(INCLUDE) -g -O0 # -Wall  -Werror -Wextra
LDFLAGS =

all: $(TARGET)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -MD

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

dll:
	$(CC) -shared -fPIC $(CFLAGS) $(SRCS) -o $(BUILD_DIR)/libmy_gpio.so

.PHONY: clean all
clean:
	rm -f $(OBJECTS) $(DEPS) $(TARGET)

-include $(DEPS)
