#ifndef LAYER_H
#define LAYER_H

#include <stdint.h>

#include "defines.h"
#include "LayerType.h"

typedef struct {
    uint16_t input_size[4];                             // 8 bytes
    uint16_t output_size[4];                            // 8 bytes
    float* weights;                                     // 4 bytes
    float* bias;                                        // 4 bytes
    __attribute__((aligned(4))) LayerType type;         // 4 bytes. enum 타입이 무조건 4바이트로 정렬됨
    union {
        struct { 
            uint8_t kernel_size;        // 1 bytes
            uint8_t stride;             // 1 bytes
            uint8_t padding;            // 1 bytes
            uint8_t dilation;           // 1 bytes
            uint8_t groups;             // 1 bytes
            uint8_t output_padding;     // 1 bytes
            uint8_t output_size;        // 1 bytes
            uint8_t input_size;         // 1 bytes
        } conv2d; // 8 bytes
    };
} Layer;

#endif // LAYER_H

