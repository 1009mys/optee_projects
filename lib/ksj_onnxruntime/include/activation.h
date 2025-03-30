#include <stdint.h>

#include "Layer.h"

// linear layer with no batch processing
void relu_noBatch(const float* input, const Layer* layer, float* output);
// linear layer with batch processing
void relu(const float* input, const Layer* layer, float* output, uint32_t batch_size);