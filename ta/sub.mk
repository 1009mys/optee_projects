global-incdirs-y += include

# include /usr/aarch64-linux-gnu/include to find the header files
global-incdirs-y += ../../../../../../../../usr/aarch64-linux-gnu/include 

#include onnxruntime header files
global-incdirs-y += /home/a1009mys/Project/onnxruntime/include

global-incdirs-y += /home/a1009mys/Project/op-tee/op-tee-package/optee_project/optee_my_gpio/lib

# add library path
global-ldflags-y += /home/a1009mys/Project/onnxruntime/build/Linux/RelWithDebInfo
global-ldflags-y += /home/a1009mys/Project/op-tee/op-tee-package/optee_project/optee_my_gpio/lib/ksj_onnxruntime/build




srcs-y += secure_storage_ta.c


# To remove a certain compiler flag, add a line like this
#cflags-template_ta.c-y += -Wno-strict-prototypes
