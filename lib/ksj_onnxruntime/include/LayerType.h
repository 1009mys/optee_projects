
#ifndef LAYERTYPE_H
#define LAYERTYPE_H

#include <stddef.h>

typedef enum {
    LAYER_LINEAR = 101,

    LAYER_RELU = 201,

    LAYER_CONV2D = 302,

    LAYER_MAXPOOL2D = 401,

    LAYER_NOT_SUPPORTED = 0
} LayerType;

#endif // LAYERTYPE_H