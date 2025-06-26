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
        // print layer->type for debugging
        printf("Running layer %d of type %d\n", i, layer->type);
        // print input tensor
        for (uint32_t j = 0; j < input->size; ++j) {
            printf("%f ", cur_in[j]);
        }
        printf("\n");

        switch (layer->type) {
            case LAYER_LINEAR:
                linear(cur_in, layer, cur_out);
                break;
            #ifdef __ARM_NEON__
            case LAYER_LINEAR_NEON:
                linear_neon(cur_in, layer, cur_out);
                break;
            #endif // __ARM_NEON__
            case LAYER_RELU:
                relu(cur_in, layer, cur_out);
                break;
            #ifdef __ARM_NEON__
            case LAYER_RELU_NEON:
                relu_neon(cur_in, layer, cur_out);
                break;
            #endif // __ARM_NEON__
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

void prepare_model(Model_Multi* model) {
    // 1. 모든 레이어의 reference count 초기화
    for (uint32_t i = 0; i < model->num_layers; ++i) {
        model->layers[i]->reference_layer_count = 0;
    }

    // 2. reference_layer_count 계산
    for (uint32_t i = 0; i < model->num_layers; ++i) {
        MultiIOLayer* layer = model->layers[i];

        for (int j = 0; j < layer->previous_layer_count; ++j) {
            int prev_idx = layer->previous_layer_index[j];
            if (prev_idx == -1 && i > 0) {
                prev_idx = i - 1;
            }
            if (prev_idx >= 0 && (uint32_t)prev_idx < model->num_layers) {
                model->layers[prev_idx]->reference_layer_count++;
            }
        }
    }

    // 3. 각 레이어의 output buffer 미리 할당
    for (uint32_t i = 0; i < model->num_layers; ++i) {
        MultiIOLayer* layer = model->layers[i];

        for (int out = 0; out < layer->num_outputs; ++out) {
            int size = 1;
            for (int d = 0; d < layer->output_dim_count; ++d) {
                size *= layer->output_sizes[out][d];
            }

            layer->outputs[out] = (float*)malloc(sizeof(float) * size);
            if (!layer->outputs[out]) {
                printf("Failed to allocate output buffer for layer %d\n", i);
                exit(1);
            }
        }
    }

    // 4. 각 레이어의 정보 출력
    for (uint32_t i = 0; i < model->num_layers; ++i) {
        MultiIOLayer* layer = model->layers[i];
        printf("Layer %d: type=%d, inputs=%d, outputs=%d, ref_count=%d, is_output=%d\n",
               i, layer->type, layer->num_inputs, layer->num_outputs, layer->reference_layer_count, layer->is_output);
        for (int j = 0; j < layer->num_inputs; ++j) {
            printf("  Input %d: size=", j);
            for (int d = 0; d < layer->input_dim_count; ++d) {
                printf("%d ", layer->input_sizes[j][d]);
            }
            printf("\n");
        }
        for (int j = 0; j < layer->num_outputs; ++j) {
            printf("  Output %d: size=", j);
            for (int d = 0; d < layer->output_dim_count; ++d) {
                printf("%d ", layer->output_sizes[j][d]);
            }
            printf("\n");
        }
    }
}

void run_multi(
    const Model_Multi* model,
    const Tensor* input[], const uint32_t input_num,
    Tensor* output[], const uint32_t output_num,
    uint32_t batch_size
) {
    printf("=========================================================\n");
    for (uint32_t l = 0; l < model->num_layers; ++l) {
        MultiIOLayer* layer = model->layers[l];
        // print layer type for debugging
        printf("Running layer %u of type %d\n", l, layer->type);

        // 1. 입력 포인터 설정
        const float* inputs[MAX_INPUTS];
        for (uint32_t i = 0; i < layer->num_inputs; ++i) {
            int prev = layer->previous_layer_index[i];

            if (prev == -1) {
                if (l == 0) {
                    // 첫 번째 레이어에서 입력은 input[] 배열로부터
                    if (i >= input_num) {
                        printf("ERROR: Invalid input index %d at layer %u\n", i, l);
                        return;
                    }
                    inputs[i] = input[i]->data;
                } else {
                    // 자동 연결: 직전 레이어의 출력 참조
                    inputs[i] = model->layers[l - 1]->outputs[0];
                }
            } else {
                inputs[i] = model->layers[prev]->outputs[0];
            }
        }

        // 2. 출력 포인터
        float* outputs[MAX_OUTPUTS];
        for (int i = 0; i < layer->num_outputs; ++i) {
            outputs[i] = layer->outputs[i];
        }

        // print input tensors
        printf("Layer %u inputs:\n", l);
        for (int i = 0; i < layer->num_inputs; ++i) {
            printf("Input %d: ", i);
            for (int d = 0; d < layer->input_dim_count; ++d) {
                printf("%d ", layer->input_sizes[i][d]);
            }
            printf("Data: ");
            for (int j = 0; j < layer->input_sizes[i][0]; ++j) {
                printf("%f ", inputs[i][j]);
            }
            printf("\n\n");
        }


        // 3. 연산 수행
        switch (layer->type) {
            case LAYER_LINEAR:
                linear(inputs[0], layer, outputs[0]);
                break;
            #ifdef __ARM_NEON__
            case LAYER_LINEAR_NEON:
                linear_neon(inputs[0], (const MultiIOLayer*)layer, outputs[0]);
                break;
            #endif // __ARM_NEON__
            case LAYER_RELU:
                relu(inputs[0], layer, outputs[0]);
                break;
            #ifdef __ARM_NEON__
            case LAYER_RELU_NEON:
                relu_neon(inputs[0], (const MultiIOLayer*)layer, outputs[0]);
                break;
            #endif // __ARM_NEON__
            default:
                printf("ERROR: Unsupported layer type: %d at layer %u\n", layer->type, l);
                return;
        }

        printf("Layer %u outputs:\n", l);
        for (int i = 0; i < layer->num_outputs; ++i) {
            printf("Output %d: ", i);
            for (int d = 0; d < layer->output_dim_count; ++d) {
                printf("%d ", layer->output_sizes[i][d]);
            }
            printf("Data: ");
            for (int j = 0; j < layer->output_sizes[i][0]; ++j) {
                printf("%f ", outputs[i][j]);
            }
            printf("\n\n");
        }

        if (l == model->num_layers - 1) {
            // 마지막 레이어의 출력은 최종 출력으로 설정
            for (uint32_t i = 0; i < layer->num_outputs && i < output_num; ++i) {
                if (output[i]) {
                    memcpy(output[i]->data, outputs[i], sizeof(float) * output[i]->size);
                } else {
                    printf("ERROR: Output %u is NULL at layer %u\n", i, l);
                }
            }
            //break;  // 마지막 레이어에서 중단
        }
        
        // 4. 사용된 이전 레이어의 참조 카운트 감소 → 0이면 출력 버퍼 해제
        for (int i = 0; i < layer->previous_layer_count; ++i) {
            int prev = layer->previous_layer_index[i];
            if (prev == -1) prev = (int)(l - 1);  // fallback

            if (prev >= 0) {
                MultiIOLayer* prev_layer = model->layers[prev];
                prev_layer->reference_layer_count--;
                if (prev_layer->reference_layer_count == 0) {
                    for (int j = 0; j < prev_layer->num_outputs; ++j) {
                        if (prev_layer->outputs[j] && prev_layer->is_output == 0) {
                            // free the output buffer if it's not an output layer
                            free(prev_layer->outputs[j]);
                            prev_layer->outputs[j] = NULL;
                        }
                    }
                }
            }
        }
    }

    // print final output tensor sizes
    printf("=========================================================\n");
    printf("Final output tensors:\n");
    for (uint32_t i = 0; i < output_num; ++i) {
        if (output[i]) {
            printf("Output %u: size=", i);
            for (int d = 0; d < output[i]->dims_size; ++d) {
                printf("%d ", output[i]->dims[d]);
            }
            printf("\n");
        } else {
            printf("Output %u: NULL\n", i);
        }
    }
    // 5. 최종 출력 텐서 복사
    uint32_t out_count = 0;
    for (uint32_t l = 0; l < model->num_layers && out_count < output_num; ++l) {
        MultiIOLayer* layer = model->layers[l];
        if (layer->reference_layer_count == 0 && layer->is_output == 1) {
            for (int i = 0; i < layer->num_outputs && out_count < output_num; ++i) {
                //Tensor* out = (Tensor*)malloc(sizeof(Tensor));
                //new_tensor(out, layer->output_sizes[i], layer->output_dim_count);
                //memcpy(out->data, layer->outputs[i], sizeof(float) * out->size);
                //output[out_count++] = out;
                // output is already allocated in the model, so just copy the data
                if (output[out_count] == NULL) {
                    printf("ERROR: Output %u is NULL at layer %u\n", out_count, l);
                }
                memcpy(output[out_count]->data, layer->outputs[i], sizeof(float) * output[out_count]->size);
                out_count++;
                // free the output buffer of the layer
                if (layer->outputs[i]) {
                    free(layer->outputs[i]);
                }
                layer->outputs[i] = NULL;  // set to NULL after freeing
            }
        }
    }

    // 추가: 마지막 cleanup
    for (uint32_t l = 0; l < model->num_layers; ++l) {
        MultiIOLayer* layer = model->layers[l];
        for (int i = 0; i < layer->num_outputs; ++i) {
            if (layer->outputs[i] != NULL) {
                free(layer->outputs[i]);
                layer->outputs[i] = NULL;
            }
        }
    }
}