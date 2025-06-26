#include <stdlib.h>

#include "Tensor.h"


void new_tensor(Tensor* tensor, int* dims, uint32_t dims_size)
{
    //tensor->new_tensor = new_tensor;
    tensor->set = set;
    tensor->get = get;

    //tensor = (Tensor*)malloc(sizeof(Tensor));
    tensor->size = 1;
    tensor->dims_size = dims_size;

    for (int i = 0; i < tensor->dims_size; i++) {
        tensor->dims[i] = dims[i];
        tensor->size *= dims[i];
    }

    tensor->data = (float*)malloc(tensor->size * sizeof(float));

    tensor->tensor_dim_type = NCHW;
}

void delete_tensor(Tensor* this)
{
    if (this->data) {
        free(this->data);
    }
    this->data = NULL;
    if (this) {
        free(this);
    }
    this = NULL;
}

void set(Tensor* this, uint32_t* indices, float* value)
{
    uint32_t index = 0;
    for (int i = 0; i < this->dims_size; i++) {
        index += indices[i] * this->dims[i];
    }
    this->data[index] = *value;
}

float get(Tensor* this, uint32_t* indices)
{
    uint32_t index = 0;
    for (int i = 0; i < this->dims_size; i++) {
        index += indices[i] * this->dims[i];
    }
    return this->data[index];
}

void convert_from_NCHW_to_NHWC(Tensor* this)
{
    if (this->tensor_dim_type == NCHW) {
        float* new_data = (float*)malloc(this->size * sizeof(float));
        int index = 0;
        for (int n = 0; n < this->dims[0]; n++) {
            for (int c = 0; c < this->dims[1]; c++) {
                for (int h = 0; h < this->dims[2]; h++) {
                    for (int w = 0; w < this->dims[3]; w++) {
                        new_data[index++] = this->data[n * this->dims[1] * this->dims[2] * this->dims[3] + c * this->dims[2] * this->dims[3] + h * this->dims[3] + w];
                    }
                }
            }
        }
        free(this->data);
        this->data = new_data;
        this->tensor_dim_type = NHWC;
    }
}

/*
#include <stdlib.h>
#include "Tensor.h"

void new_tensor(Tensor* tensor, int* dims, uint32_t dims_size) {
    tensor->set = set;
    tensor->get = get;

    tensor->size = 1;
    tensor->dims_size = dims_size;

    for (uint32_t i = 0; i < dims_size; ++i) {
        tensor->dims[i] = dims[i];
        tensor->size *= dims[i];
    }

    tensor->data = (float*)malloc(tensor->size * sizeof(float));
    tensor->tensor_dim_type = NCHW;
}

void delete_tensor(Tensor* this) {
    if (this && this->data) {
        free(this->data);
        this->data = NULL;
    }
}

void set(Tensor* this, uint32_t* indices, float* value) {
    uint32_t index = 0, stride = 1;
    for (int i = this->dims_size - 1; i >= 0; --i) {
        index += indices[i] * stride;
        stride *= this->dims[i];
    }
    this->data[index] = *value;
}

float get(Tensor* this, uint32_t* indices) {
    uint32_t index = 0, stride = 1;
    for (int i = this->dims_size - 1; i >= 0; --i) {
        index += indices[i] * stride;
        stride *= this->dims[i];
    }
    return this->data[index];
}

void convert_from_NCHW_to_NHWC(Tensor* this) {
    if (this->tensor_dim_type != NCHW || this->dims_size != 4) return;

    int N = this->dims[0];
    int C = this->dims[1];
    int H = this->dims[2];
    int W = this->dims[3];

    float* new_data = (float*)malloc(this->size * sizeof(float));
    for (int n = 0; n < N; n++)
        for (int h = 0; h < H; h++)
            for (int w = 0; w < W; w++)
                for (int c = 0; c < C; c++) {
                    int src_idx = n*C*H*W + c*H*W + h*W + w;
                    int dst_idx = n*H*W*C + h*W*C + w*C + c;
                    new_data[dst_idx] = this->data[src_idx];
                }

    free(this->data);
    this->data = new_data;

    // update shape
    this->dims[1] = H;
    this->dims[2] = W;
    this->dims[3] = C;
    this->tensor_dim_type = NHWC;
}
    */