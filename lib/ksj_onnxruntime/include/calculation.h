#include <stdint.h>

#include "Layer.h"
#include "Tensor.h"

void copy_Tensor(const Tensor* src, Tensor* dst);

void plus_Tensor(const Tensor* a, const Tensor* b, Tensor* result);

void minus_Tensor(const Tensor* a, const Tensor* b, Tensor* result);

void multiply_Tensor(const Tensor* a, const Tensor* b, Tensor* result);

void matrix_multiply_Tensor(const Tensor* a, const Tensor* b, Tensor* result);

void multiply_scalar_Tensor(const Tensor* a, float scalar, Tensor* result);

void dot_Tensor(const Tensor* a, const Tensor* b, Tensor* result);


void plus_Tensor_neon(const Tensor* a, const Tensor* b, Tensor* result);

void minus_Tensor_neon(const Tensor* a, const Tensor* b, Tensor* result);

void multiply_Tensor_neon(const Tensor* a, const Tensor* b, Tensor* result);

void matrix_multiply_Tensor_neon(const Tensor* a, const Tensor* b, Tensor* result);

void multiply_scalar_Tensor_neon(const Tensor* a, float scalar, Tensor* result);

void dot_Tensor_neon(const Tensor* a, const Tensor* b, Tensor* result);



