#ifndef LAYER_H
#define LAYER_H

#include <stdint.h>

#include "defines.h"
#include "LayerType.h"

typedef struct {
    LayerType type;
    uint32_t input_size;
    uint32_t output_size;
    float* weights;
    float* bias;
} Layer;

#endif // LAYER_H