#ifndef MODEL_H
#define MODEL_H

#include "defines.h"
#include "Layer.h"

typedef struct {
    Layer* layers[MAX_LAYERS];
    uint32_t num_layers;
} Model;

#endif // MODEL_H