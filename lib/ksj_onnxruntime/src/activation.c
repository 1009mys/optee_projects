
#include <stdint.h>
#ifdef __ARM_NEON__
#include <arm_neon.h>
#endif // __ARM_NEON__

#include "activation.h"

void relu_noBatch(const float* input, const MultiIOLayer* layer, float* output) {
    /*
    description:
        computes the output of a ReLU layer
        no batch processing

    dimensions:
        input   : [layer->input_size[1]]
        output  : [layer->output_size[1]]
    */
    for (uint32_t i = 0; i < layer->input_sizes[0][1]; ++i) {
        output[i] = input[i] > 0.0f ? input[i] : 0.0f;
    }
}

void relu(const float* input, const MultiIOLayer* layer, float* output) {
    /*
    description:
        computes the output of a ReLU layer
        with batch processing

    dimensions:
        input   : layer->input_size
        output  : layer->output_size
    */
    uint32_t size_of_input = 1;
    for (uint32_t i = 0; i < layer->input_dim_count; ++i) {
        size_of_input *= layer->input_sizes[0][i];
    }
    for (uint32_t i = 0; i < size_of_input; ++i) {
        output[i] = input[i] > 0.0f ? input[i] : 0.0f;
    }
}

#ifdef __ARM_NEON__
void relu_neon(const float* input, const MultiIOLayer* layer, float* output) {
    /*
    description:
        computes the output of a ReLU layer using ARM NEON SIMD
    */

    uint32_t total_size = layer->input_sizes[0][0] * layer->input_sizes[0][1];
    uint32_t i = 0;

    float32x4_t zero_vec = vdupq_n_f32(0.0f);  // 4개의 0.0f

    // 4개씩 SIMD 처리
    for (; i + 4 <= total_size; i += 4) {
        float32x4_t in_vec = vld1q_f32(&input[i]);          // input 로드
        float32x4_t out_vec = vmaxq_f32(in_vec, zero_vec);  // max(input, 0)
        vst1q_f32(&output[i], out_vec);                     // 결과 저장
    }

    // 나머지 처리 (SIMD로 안 되는 부분)
    for (; i < total_size; ++i) {
        output[i] = input[i] > 0.0f ? input[i] : 0.0f;
    }
}


void test_relu_neon() {
    // 테스트용 코드
    Layer layer;
    layer.input_size[0] = 1;  // 배치 크기
    layer.input_size[1] = 8;  // 입력 크기
    layer.output_size[0] = 1; // 출력 배치 크기
    layer.output_size[1] = 8; // 출력 크기

    float input[] = { -1.0f, 2.0f, -3.0f, 4.0f, -5.0f, 6.0f, -7.0f, 8.0f };
    float output[8];

    relu_neon(input, &layer, output);

    for (int i = 0; i < 8; ++i) {
        printf("%f ", output[i]);
    }
    printf("\n");
}

#endif // __ARM_NEON__