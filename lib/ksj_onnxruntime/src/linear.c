#include <stdint.h>

#ifdef __ARM_NEON__
#include <arm_neon.h>
#endif // __ARM_NEON__

#include <stdio.h>

#include "Layer.h"
#include "linear.h"
#include "Tensor.h"




void linear_noBatch(const float *input, const MultiIOLayer *layer, float *output) {
    /*
    description:
        computes the output of a linear layer
        no batch processing

    dimensions:
        input   : 
        weights : 
        bias    : 
        output  : 
    */
    for (uint32_t i = 0; i < layer->output_sizes[0][1]; ++i) {
        float sum = layer->bias[i];
        for (uint32_t j = 0; j < layer->input_sizes[0][1]; ++j) {
            sum += layer->weights[i * (uint32_t)layer->input_sizes[0][1] + j] * input[j];
        }
        output[i] = sum;
    }
}


void linear(const float *input, const MultiIOLayer *layer, float *output) {
    /*
    description:
        computes the output of a linear layer
        with batch processing
        only supports 2D input and output

    dimensions:
        input   : [batch, in_feat]
        weights : [out_feat, in_feat]
        bias    : [out_feat]
        output  : [batch, out_feat]
    */
    uint32_t in_feat = layer->input_sizes[0][1];    // input shape: [batch, in_feat]
    uint32_t out_feat = layer->output_sizes[0][1];  // output shape: [batch, out_feat]

    for (uint32_t i = 0; i < layer->input_sizes[0][0]; ++i) { // batch 크기
        linear_noBatch(input + i * in_feat, layer, output + i * out_feat);
    }
}

void linear_Tensor(const Tensor *input, const MultiIOLayer *layer, Tensor *output) {
    /*
    description:
        computes the output of a linear layer
        with batch processing
        only supports 2D input and output

    dimensions:
        input   : [batch, in_feat]
        weights : [out_feat, in_feat]
        bias    : [out_feat]
        output  : [batch, out_feat]
    */
    linear(input->data, layer, output->data);
}

#ifdef __ARM_NEON__
void linear_noBatch_neon(const float *input, const MultiIOLayer *layer, float *output) {
    /*
    description:
        computes the output of a linear layer using ARM NEON SIMD
        no batch processing

    dimensions:
        input   : [layer->input_sizes[0][1]]
        weights : [layer->output_sizes[0][1], layer->input_sizes[0][1]]
        bias    : [layer->output_sizes[0][1]]
        output  : [layer->output_sizes[0][1]]
    */

    for (uint32_t i = 0; i < layer->output_sizes[0][1]; i++) {
        float sum = 0.0f;  // sum 초기화
        float32x4_t sum_vec = vdupq_n_f32(0.0f);  // NEON 벡터 초기화

        uint32_t j = 0;
        for (j = 0; j + 4 < layer->input_sizes[0][1]; j += 4) {
            float32x4_t input_vec = vld1q_f32(&input[j]);  // input 로드
            float32x4_t weight_vec = vld1q_f32(&layer->weights[i * layer->input_sizes[0][1] + j]);  // weights 로드
            sum_vec = vmlaq_f32(sum_vec, weight_vec, input_vec);  // sum += weight * input
        }
        // sum_vec를 스칼라 값으로 변환하여 output에 저장
        sum = vaddvq_f32(sum_vec);
        // 나머지 요소 처리 (4의 배수가 아닐 경우)
        // simd 처리 후 남은 요소가 있을 경우
        // simd로 처리하지 않음
        // j는 4의 배수로 증가했으므로, j가 layer->input_sizes[0][1]보다 작을 때만 처리
        if (j < layer->input_sizes[0][1]) {
            for (; j < layer->input_sizes[0][1]; j++) {
                sum += layer->weights[i * layer->input_sizes[0][1] + j] * input[j];
            }
        }
        sum += layer->bias[i];  // bias 추가
        output[i] = sum;  // 결과 저장
    }
}

void linear_neon(const float *input, const MultiIOLayer *layer, float *output) {
    /*
    description:
        computes the output of a linear layer using ARM NEON SIMD
        with batch processing
        only supports 2D input and output

    dimensions:
        input   : [batch, in_feat]
        weights : [out_feat, in_feat]
        bias    : [out_feat]
        output  : [batch, out_feat]
    */
    uint32_t in_feat = layer->input_sizes[0][1];    // input shape: [batch, in_feat]
    uint32_t out_feat = layer->output_sizes[0][1];  // output shape: [batch, out_feat]
    for (uint32_t i = 0; i < layer->input_sizes[0][0]; ++i) { // batch 크기
        linear_noBatch_neon(input + i * in_feat, layer, output + i * out_feat);
    }
}

void linear_neon_Tensor(const Tensor *input, const MultiIOLayer *layer, Tensor *output) {
    /*
    description:
        computes the output of a linear layer using ARM NEON SIMD
        with batch processing
        only supports 2D input and output

    dimensions:
        input   : [batch, in_feat]
        weights : [out_feat, in_feat]
        bias    : [out_feat]
        output  : [batch, out_feat]
    */
    linear_neon(input->data, layer, output->data);
}


void test_linear_neon() {
    float input[8] = {1, 2, 3, 4, 5, 6, 7, 8};
float weights[2 * 8] = {
    0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8,
    1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8
};
float bias[2] = {0.5, -1.0};
float output[2];

Layer layer = {
    .input_size = {1, 8},
    .output_size = {1, 2},
    .weights = weights,
    .bias = bias
};

linear_noBatch_neon(input, &layer, output);

printf("out0 = %f\n", output[0]);
printf("out1 = %f\n", output[1]);
}

#endif // __ARM_NEON__