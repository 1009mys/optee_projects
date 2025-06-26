#include <stdint.h>

#include "Layer.h"

void slow_conv2d_noBatch(const float *input, const Layer *layer, float *output) {
    /*
    description:
        computes the output of a conv2d layer
        no batch processing
        NCHW format

    dimensions:
        input   : [layer->input_size[1], 
                   layer->input_size[2], 
                   layer->input_size[3]]
        weights : [layer->conv2d->out_channels, 
                   layer->conv2d->in_channels, 
                   layer->conv2d->kernel_size, 
                   layer->conv2d->kernel_size]
        bias    : [layer->conv2d->out_channels]
        output  : [layer->output_size[1], 
                   layer->output_size[2], 
                   layer->output_size[3]]
    */
    uint32_t ouput_tensor_width = (layer->input_size[2] - layer->conv2d.kernel_size + 2 * layer->conv2d.padding) / layer->conv2d.stride + 1;

    for (uint32_t i = 0; i < layer->conv2d.out_channels; ++i) {
        float sum = layer->bias[i];
        for (uint32_t j = 0; j < layer->conv2d.in_channels; ++j) {
            for (uint32_t y=0;y<ouput_tensor_width;++y) {
                for (uint32_t x=0;x<ouput_tensor_width;++x) {
                    for (uint32_t ky=0;ky<layer->conv2d.kernel_size;++ky) {
                        for (uint32_t kx=0;kx<layer->conv2d.kernel_size;++kx) {
                            sum += input[
                                j * layer->input_size[2] * layer->input_size[3] + 
                                (y * layer->conv2d.stride + ky) * layer->input_size[3] + 
                                (x * layer->conv2d.stride + kx)] *


                                layer->weights[
                                    i * layer->conv2d.in_channels * layer->conv2d.kernel_size * layer->conv2d.kernel_size + 
                                    j * layer->conv2d.kernel_size * layer->conv2d.kernel_size + 
                                    ky * layer->conv2d.kernel_size + 
                                    kx];
                        }
                    }
                }
            }
        }
        output[i] = sum;
    }
}

void slow_conv2d(const float *input, const Layer *layer, float *output) {
    /*
    description:
        computes the output of a conv2d layer
        with batch processing

    dimensions:
        input   : layer->input_size
        weights : [layer->out_channels, layer->in_channels, layer->kernel_size, layer->kernel_size]
        bias    : [layer->out_channels]
        output  : layer->output_size
    */
    for (uint32_t i = 0; i < layer->input_size[0]; ++i) {
        slow_conv2d_noBatch(input + i * layer->input_size[1], layer, output + i * layer->output_size[1]);
    }
}