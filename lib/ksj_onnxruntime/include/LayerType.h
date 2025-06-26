
#ifndef LAYERTYPE_H
#define LAYERTYPE_H

#include <stddef.h>

typedef enum {
    LAYER_LINEAR = 10001,
    LAYER_LINEAR_NEON = 10002,

    // activation layers 20000
    LAYER_RELU = 20001,
    LAYER_RELU_NEON = 20002,

    LAYER_SIGMOID = 20101,

    // convolution layers 30000
    LAYER_CONV2D = 30002,

    // pooling layers 40000
    LAYER_MAXPOOL2D = 40001,

    // calculation layers 50000
    LAYER_ADD = 50001,
    LAYER_SUB = 50002,
    LAYER_MUL = 50003,
    LAYER_DIV = 50004,
    LAYER_MATMUL = 50005,
    LAYER_MATMUL_NEON = 50006,
    LAYER_DOT = 50007,
    LAYER_DOT_NEON = 50008,
    // normalization layers 60000
    LAYER_BATCHNORM = 60001,

    LAYER_NOT_SUPPORTED = 0
} LayerType;

#endif // LAYERTYPE_H