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
}

void delete_tensor(Tensor* this)
{
    free(this->data);
    free(this);
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
//