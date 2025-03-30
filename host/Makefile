CXX     ?= $(CROSS_COMPILE)g++
LD      ?= $(CROSS_COMPILE)ld
AR      ?= $(CROSS_COMPILE)ar
NM      ?= $(CROSS_COMPILE)nm
OBJCOPY ?= $(CROSS_COMPILE)objcopy
OBJDUMP ?= $(CROSS_COMPILE)objdump
READELF ?= $(CROSS_COMPILE)readelf

OBJDIR  := build
OBJS    := $(OBJDIR)/main.o
CXXFLAGS+= -Wall -O0 -g -std=c++17 -I../ta/include -I$(TEEC_EXPORT)/include 
CXXFLAGS+= -I./include -I/home/a1009mys/Project/onnxruntime/include/
LDADD   += -lteec -L$(TEEC_EXPORT)/lib -L/home/a1009mys/Project/onnxruntime/build/Linux/RelWithDebInfo 
LDADD   += -lonnxruntime


BINARY  := $(OBJDIR)/optee_my_gpio

.PHONY: all
all: $(BINARY)

$(BINARY): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $< $(LDADD)

$(OBJDIR):
	mkdir -p $(OBJDIR)

.PHONY: clean
clean:
	rm -rf $(OBJDIR)

$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@
