CROSS_COMPILE ?= aarch64-linux-gnu-

# If _HOST or _TA specific compilers are not specified, then use CROSS_COMPILE
HOST_CROSS_COMPILE ?= $(CROSS_COMPILE)

CC      = $(CROSS_COMPILE)gcc
CXX     = $(CROSS_COMPILE)g++
LD      = $(CROSS_COMPILE)ld
AR      = $(CROSS_COMPILE)ar
NM      = $(CROSS_COMPILE)nm
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
READELF = $(CROSS_COMPILE)readelf

INCLUDE = ./include -I/home/a1009mys/onnx_lib/onnxruntime-linux-x64-1.21.0/include
SRC_DIR = ./src
OBJ_DIR = ./obj

BUILD_DIR = build

TARGET = $(BUILD_DIR)/my_onnxruntime_lib_test

C_SRCS = $(wildcard $(SRC_DIR)/*.c)
CPP_SRCS = $(wildcard $(SRC_DIR)/*.cpp)

C_OBJS   = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(C_SRCS))
CPP_OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(CPP_SRCS))

OBJECTS = $(C_OBJS) $(CPP_OBJS)
DEPS    = $(OBJECTS:.o=.d)

CFLAGS = -I$(INCLUDE)   -g -O0 # -Wall  -Werror -Wextra 
CXXFLAGS = -I$(INCLUDE) -g -O0 # -Wall  -Werror -Wextra
LDFLAGS = -L/home/a1009mys/onnx_lib/onnxruntime-linux-x64-1.21.0/lib/ -lonnxruntime -l

all: $(TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -MD

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ -MD

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

dll:
	$(CC) -shared -fPIC $(CFLAGS) $(SRCS) -o $(BUILD_DIR)/libmy_gpio.so

.PHONY: clean all
clean:
	rm -f $(OBJECTS) $(DEPS) $(TARGET)

-include $(DEPS)
