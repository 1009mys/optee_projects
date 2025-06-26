#include <stdint.h>

#include "Layer.h"

// linear layer with no batch processing
void relu_noBatch(const float* input, const MultiIOLayer* layer, float* output);
// linear layer with batch processing
void relu(const float* input, const MultiIOLayer* layer, float* output);
// linear layer with ARM NEON SIMD
// define only if ARM NEON is supported
#ifdef __ARM_NEON__
void relu_neon(const float* input, const MultiIOLayer* layer, float* output);
#endif // __ARM_NEON__