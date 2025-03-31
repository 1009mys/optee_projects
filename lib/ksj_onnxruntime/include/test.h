#include <stdint.h>
#include <stdio.h>

#include "Layer.h"
#include "Tensor.h"
#include "model.h"


void read_tensor(const char* txtFile, Tensor* tensor);

void read_layer(const char* txtFile_weight, const char* txtFile_bias, Layer* layer);

void load_model(Model* model, const char* modelPath);

void test_AlexNet();

int main2();
int main3();