#pragma once

#include <stdint.h>

#include "defines.h"

typedef struct Tensor_{
    float* data;
    uint32_t size;
    int dims[MAX_DIMS];
    int dims_size;
    void (*new_tensor)(struct Tensor_*, int*, uint32_t);
    void (*delete_tensor)(struct Tensor_*);
    void (*set)(struct Tensor_*, uint32_t*, float*);
    float (*get)(struct Tensor_*, uint32_t*);
} Tensor;

void new_tensor(Tensor* tensor, int* dims, uint32_t dims_size);

void delete_tensor(Tensor* tensor);

void set(Tensor* this, uint32_t* indices, float* value);

float get(Tensor* this, uint32_t* indices);