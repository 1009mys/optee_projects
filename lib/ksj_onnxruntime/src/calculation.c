#include "calculation.h"



void copy_Tensor(const Tensor* src, Tensor* dst);

void plus_Tensor(const Tensor* a, const Tensor* b, Tensor* result) {
    if (a->size != b->size || a->size != result->size) {
        // Handle size mismatch error
        return;
    }
    for (uint32_t i = 0; i < a->size; ++i) {
        result->data[i] = a->data[i] + b->data[i];
    }
}

void minus_Tensor(const Tensor* a, const Tensor* b, Tensor* result);

void multiply_Tensor(const Tensor* a, const Tensor* b, Tensor* result);

void matrix_multiply_Tensor(const Tensor* a, const Tensor* b, Tensor* result);

void multiply_scalar_Tensor(const Tensor* a, float scalar, Tensor* result);

void dot_Tensor(const Tensor* a, const Tensor* b, Tensor* result);