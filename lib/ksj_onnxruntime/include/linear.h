#include <stdint.h>

#include "Layer.h"
#include "Tensor.h"

// linear layer with no batch processing
void linear_noBatch(const float *input, const MultiIOLayer *layer, float *output);
// linear layer with batch processing
void linear(const float *input, const MultiIOLayer *layer, float *output);
void linear_Tensor(const Tensor *input, const MultiIOLayer *layer, Tensor *output);
// linear layer with ARM NEON SIMD
#ifdef __ARM_NEON__
void linear_neon(const float *input, const MultiIOLayer *layer, float *output);
void linear_neon_Tensor(const Tensor *input, const MultiIOLayer *layer, Tensor *output);
#endif // __ARM_NEON__