#include <stdint.h>

#include "activation.h"
#include "linear.h"

#include "Layer.h"
#include "defines.h"
#include "Tensor.h"
#include "model.h"


static void run(const Model* model, const Tensor* input, Tensor* output, uint32_t batch_size) {

    float tmp1[MAX_TENSOR_SIZE], tmp2[MAX_TENSOR_SIZE];
    const float* cur_in = input->data;
    float* cur_out = tmp1;

    for (uint32_t i = 0; i < model->num_layers; ++i) {
        const Layer* layer = model->layers[i];
        switch (layer->type) {
            case LAYER_LINEAR:
                linear(cur_in, layer, cur_out);
                break;
            case LAYER_RELU:
                relu(cur_in, layer, cur_out);
                break;
            default:
                return;
        }

        float *tmp = (float*)cur_in;
        cur_in = cur_out;
        cur_out = tmp;
    }

    for (uint32_t i = 0; i < output->size; ++i) {
        output->data[i] = cur_in[i];
    }
    
}