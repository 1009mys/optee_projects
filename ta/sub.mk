global-incdirs-y += include

# include /usr/aarch64-linux-gnu/include to find the header files
global-incdirs-y += ../../../../../../../../usr/aarch64-linux-gnu/include

#include onnxruntime header files
global-incdirs-y += /home/a1009mys/Project/onnxruntime/include

# add library path
global-ldflags-y += /home/a1009mys/Project/onnxruntime/build/Linux/RelWithDebInfo




srcs-y += my_gpio_ta.c


# To remove a certain compiler flag, add a line like this
#cflags-template_ta.c-y += -Wno-strict-prototypes
