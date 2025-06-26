#ifndef LAYER_H
#define LAYER_H

#include <stdint.h>

#include "defines.h"
#include "LayerType.h"

typedef struct {
    uint16_t input_size[MAX_DIMS];                             // 8 bytes
    uint16_t output_size[MAX_DIMS];                            // 8 bytes
    float* weights;                                     // 4 bytes
    float* bias;                                        // 4 bytes
    __attribute__((aligned(4))) LayerType type;         // 4 bytes. enum 타입이 무조건 4바이트로 정렬됨
    union {
        struct { 
            uint8_t in_channels;        // 1 bytes
            uint8_t out_channels;       // 1 bytes
            uint8_t kernel_size;        // 1 bytes
            uint8_t stride;             // 1 bytes
            uint8_t padding;            // 1 bytes
            uint8_t dilation;           // 1 bytes, fixed to 1. not supported
            uint8_t groups;             // 1 bytes. not supported
            uint8_t padding_mode;       // 1 bytes
                // padding_mode
                // 0: zeroes
                // 1: reflect
                // 2: replicate
                // 3: circular
        } conv2d; // 8 bytes
    };
} Layer;

typedef struct multi_input_output_layer_ {
    uint32_t num_inputs;                             // 4 bytes
    uint32_t num_outputs;                            // 4 bytes
    uint32_t input_dim_count;            
    uint32_t output_dim_count;           // 4 bytes each
    uint32_t input_sizes[MAX_INPUTS][MAX_DIMS]; // 
    uint32_t output_sizes[MAX_OUTPUTS][MAX_DIMS]; // 
    
    uint32_t reference_layer_count; // number of layers that using this layer as input
    uint32_t previous_layer_count; // number of layers that using this layer as output
    uint32_t previous_layer_index[4]; // index of previous layers that using this layer as output

    uint32_t is_output; // 0: not output, 1: output
    union{
        float* outputs[MAX_OUTPUTS]; 
    };
    
    // delete when reference_layer_count is 0

    float* weights;                             // 4 bytes
    float* bias;                                // 4 bytes
    __attribute__((aligned(4))) LayerType type; // 4 bytes. enum 타입이 무조건 4바이트로 정렬됨
    union {
        struct { 
            uint8_t in_channels;        // 1 bytes
            uint8_t out_channels;       // 1 bytes
            uint8_t kernel_size;        // 1 bytes
            uint8_t stride;             // 1 bytes
            uint8_t padding;            // 1 bytes
            uint8_t dilation;           // 1 bytes, fixed to 1. not supported
            uint8_t groups;             // 1 bytes. not supported
            uint8_t padding_mode;       // 1 bytes
                // padding_mode
                // 0: zeroes
                // 1: reflect
                // 2: replicate
                // 3: circular
        } conv2d; // 8 bytes
    };

} MultiIOLayer;
/*
void get_previous_layer_output(const MultiIOLayer* layer, const float* input, float* output, uint32_t batch_size) {
    // This function retrieves the output from the previous layer based on the layer's configuration.
    // It is used to handle multi-input/output layers.
    for (int i = 0; i < layer->previous_layer_count; ++i) {
        int index = layer->previous_layer_index[i];
        const Layer* prev_layer = (const Layer*)layer->input_sizes[index];
        // Assuming prev_layer has a method to get its output
        // This is a placeholder for actual implementation
        // prev_layer->get_output(input, output, batch_size);
    }
}*/


#endif // LAYER_H

