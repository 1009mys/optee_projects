#include <cstdint>

#include "layer.hpp"

class Linear : public Layer
{
private:
    uint16_t in_features;        // 1 bytes
    uint16_t out_features;       // 1 bytes
    bool bias;          // 1 bytes

public:
    Linear(uint16_t input_size[4], uint16_t output_size[4], float* weights, float* bias, uint8_t in_features, uint8_t out_features);
    ~Linear();

    void forward(const float* input, float* output);
};