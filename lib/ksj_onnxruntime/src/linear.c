#include <stdint.h>

#include "Layer.h"
#include "linear.h"




void linear_noBatch(const float *input, const Layer *layer, float *output) {
    /*
    description:
        computes the output of a linear layer
        no batch processing

    dimensions:
        input   : [layer->input_size]
        weights : [layer->output_size, layer->input_size]
        bias    : [layer->output_size]
        output  : [layer->output_size]
    */
    for (uint32_t i = 0; i < layer->output_size[1]; ++i) {
        float sum = layer->bias[i];
        for (uint32_t j = 0; j < layer->input_size[1]; ++j) {
            sum += layer->weights[i * layer->input_size[1] + j] * input[j];
        }
        output[i] = sum;
    }
}


void linear(const float *input, const Layer *layer, float *output) {
    /*
    description:
        computes the output of a linear layer
        with batch processing

    dimensions:
        input   : [batch_size, layer->input_size]
        weights : [layer->output_size, layer->input_size]
        bias    : [layer->output_size]
        output  : [batch_size, layer->output_size]
    */
    for (uint32_t i = 0; i < layer->input_size[0]; ++i) {
        linear_noBatch(input + i * layer->input_size[1], layer, output + i * layer->output_size[1]);
    }
}