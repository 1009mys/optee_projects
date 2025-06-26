#include <cstdint>

#include "layer.hpp"

class Conv2d : public Layer
{
private:
    uint16_t in_channels;        // 1 bytes
    uint16_t out_channels;       // 1 bytes
    bool bias;
    uint16_t kernel_size;        // 1 bytes
    uint16_t stride;             // 1 bytes
    uint16_t padding;            // 1 bytes
    uint16_t dilation;           // 1 bytes, fixed to 1. not supported
    uint16_t groups;             // 1 bytes. not supported
    uint16_t padding_mode;       // 1 bytes
    // padding_mode
    // 0: zeroes
    // 1: reflect
    // 2: replicate
    // 3: circular

public:
    Conv2d(uint16_t input_size[4], uint16_t output_size[4], float* weights, float* bias, uint8_t in_channels, uint8_t out_channels, uint8_t kernel_size, uint8_t stride, uint8_t padding, uint8_t dilation, uint8_t groups, uint8_t padding_mode);
    ~Conv2d();
    
    void forward(const float* input, float* output);

};