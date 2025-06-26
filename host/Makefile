CXX     ?= $(CROSS_COMPILE)g++
LD      ?= $(CROSS_COMPILE)ld
AR      ?= $(CROSS_COMPILE)ar
NM      ?= $(CROSS_COMPILE)nm
OBJCOPY ?= $(CROSS_COMPILE)objcopy
OBJDUMP ?= $(CROSS_COMPILE)objdump
READELF ?= $(CROSS_COMPILE)readelf

OBJDIR  := build
OBJS    := $(OBJDIR)/main.o
CXXFLAGS+= -Wall -O0 -g -std=c++17 -I../ta/include -I$(TEEC_EXPORT)/include  -I/home/a1009mys/Project/op-tee/op-tee-package/optee_project/optee_my_gpio/lib
CXXFLAGS+= -I./include -I/home/a1009mys/Project/onnxruntime/include/ -I/home/a1009mys/Project/onnxruntime/include/onnxruntime/core/session/
LDADD   += -lteec -L$(TEEC_EXPORT)/lib -L/home/a1009mys/Project/onnxruntime/build/Linux/RelWithDebInfo 
LDADD   += -lonnxruntime /home/a1009mys/Project/op-tee/op-tee-package/optee_project/optee_my_gpio/lib/ksj_onnxruntime/build


BINARY  := $(OBJDIR)/optee_my_app

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
