#include <cstdint>

class Layer {
protected:
    uint16_t input_size[4];                             // 8 bytes
    uint16_t output_size[4];                            // 8 bytes
    float* weights;                                     // 4 bytes
    float* bias;                                        // 4 bytes
};