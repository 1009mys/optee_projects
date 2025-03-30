
#include <stdint.h>

#include "activation.h"

void relu_noBatch(const float* input, const Layer* layer, float* output) {
    /*
    description:
        computes the output of a ReLU layer
        no batch processing

    dimensions:
        input   : [layer->input_size]
        output  : [layer->input_size]
    */
    for (uint32_t i = 0; i < layer->input_size; ++i) {
        output[i] = input[i] > 0.0f ? input[i] : 0.0f;
    }
}

void relu(const float* input, const Layer* layer, float* output, uint32_t batch_size) {
    /*
    description:
        computes the output of a ReLU layer
        with batch processing

    dimensions:
        input   : [batch_size, layer->input_size]
        output  : [batch_size, layer->input_size]
    */
    for (uint32_t i = 0; i < batch_size * layer->input_size; ++i) {
        output[i] = input[i] > 0.0f ? input[i] : 0.0f;
    }
}
