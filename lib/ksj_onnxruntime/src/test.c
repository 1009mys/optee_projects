#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "model.h"
#include "run.h"
#include "Tensor.h"
#include "parson.h"

#include "onnxruntime_c_api.h"


const int BatchSize = 4;

void read_tensor(const char* txtFile, Tensor* tensor)
{
    FILE* file = fopen(txtFile, "r");
    if (file == NULL) {
        printf("Error: Could not open file %s\n", txtFile);
        exit(1);
    }

    for (int i = 0; i < tensor->size; i++) {
        fscanf(file, "%f", &tensor->data[i]);
    }

    fclose(file);
}

void read_layer(const char* txtFile_weight, const char* txtFile_bias, Layer* layer)
{
    FILE* file_weight = fopen(txtFile_weight, "r");
    if (file_weight == NULL) {
        printf("Error: Could not open file %s\n", txtFile_weight);
        exit(1);
    }
    for (int i = 0; i < layer->output_size[1] * layer->input_size[1]; i++) {
        fscanf(file_weight, "%f", &layer->weights[i]);
    }
    fclose(file_weight);


    FILE* file_bias = fopen(txtFile_bias, "r");
    if (file_bias == NULL) {
        printf("Error: Could not open file %s\n", txtFile_bias);
        exit(1);
    }
    for (int i = 0; i < layer->output_size[1]; i++) {
        fscanf(file_bias, "%f", &layer->bias[i]);
    }
    fclose(file_bias);
}

void load_model(Model* model, const char* modelPath)
{
    // read layer list from json file

    // const char* layerList = "layer_list.json";
    const char* layerList = "layer_options.json";
    // modelPath와 합쳐라
    strcat (modelPath, layerList);
    printf("modelPath: %s\n", modelPath);
    
    JSON_Value* root_value = json_parse_file(modelPath);
    JSON_Array* layers_array = json_value_get_array(root_value);

    int size = json_array_get_count(layers_array);
    printf("num_layers: %d\n", size);

}

void test_AlexNet()
{
    Tensor* input_random_1 = (Tensor*)malloc(sizeof(Tensor));
    new_tensor(input_random_1, (int[]){BatchSize, 3, 224, 224}, 4);
    read_tensor("../../testcases/alexnet/input_tensor_random_1.txt", input_random_1);
    
    Model* model = (Model*)malloc(sizeof(Model));
    char* modelPath = (char*)malloc(200 * sizeof(char));
    strcpy(modelPath, "../../testcases/alexnet/");
    printf("modelPath: %s\n", modelPath);
    load_model(model, modelPath);


    free(input_random_1->data);
    free(input_random_1);
    free(model);
    free(modelPath);
}

int main3()
{
    OrtEnv* env;
    OrtStatus* status;
    const OrtApi* g_ort = OrtGetApiBase()->GetApi(ORT_API_VERSION);

    status = g_ort->CreateEnv(ORT_LOGGING_LEVEL_WARNING, "test", &env);
    if (status != NULL) {
        printf("Failed to create OrtEnv.\n");
        return -1;
    }

    // print version
    const char* version = OrtGetApiBase()->GetVersionString();
    printf("ONNX Runtime version: %s\n", version);



    return 0;
}

int main2()
{
    Tensor* input_random_1 = (Tensor*)malloc(sizeof(Tensor));
    input_random_1->dims[0] = BatchSize;
    input_random_1->dims[1] = 10;
    input_random_1->data = (float*)malloc(10 * BatchSize * sizeof(float));
    input_random_1->size = 10 * BatchSize;
    read_tensor("../../testcases/linear/input_tensor_random_1.txt", input_random_1);

    Model* model = (Model*)malloc(sizeof(Model));
    model->num_layers = 2;

    Layer* layer1 = (Layer*)malloc(sizeof(Layer));
    layer1->type = LAYER_LINEAR;
    layer1->input_size[0] = BatchSize;
    layer1->input_size[1] = 10;
    layer1->output_size[0] = BatchSize;
    layer1->output_size[1] = 6;
    layer1->weights = (float*)malloc(10 * 6 * sizeof(float));
    layer1->bias = (float*)malloc(6 * sizeof(float));
    read_layer("../../testcases/linear/model_weights.txt", "../../testcases/linear/model_bias.txt", layer1);

    Layer* layer2 = (Layer*)malloc(sizeof(Layer));
    layer2->type = LAYER_RELU;
    layer2->input_size[0] = BatchSize;
    layer2->input_size[1] = 6;
    layer2->output_size[0] = BatchSize;
    layer2->output_size[1] = 6;
    layer2->weights = NULL;
    layer2->bias = NULL;

    layer2->conv2d.kernel_size = 0;
    
    model->layers[0] = layer1;
    model->layers[1] = layer2;

    Tensor* output = (Tensor*)malloc(sizeof(Tensor));
    output->dims[0] = BatchSize;
    output->dims[1] = 6;
    output->data = (float*)malloc(6 * BatchSize * sizeof(float));
    output->size = 6 * BatchSize;

    run(model, input_random_1, output, BatchSize);

    for (uint32_t i = 0; i < output->size; ++i) {
        printf("%f\t", output->data[i]);
        if ((i + 1) % 6 == 0) {
            printf("\n");
        }
    }

    free(input_random_1->data);
    free(input_random_1);
    free(layer1->weights);
    free(layer1->bias);
    free(layer1);
    free(layer2);
    free(output->data);
    free(output);
    free(model);

}