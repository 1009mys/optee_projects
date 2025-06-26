#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "model.h"
#include "run.h"
#include "Tensor.h"
#include "parson.h"

//#include <arm_neon.h>

// #include "onnxruntime_c_api.h"


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


void read_multi(const char* txtFile_weight, const char* txtFile_bias, MultiIOLayer* layer)
{
    FILE* file_weight = fopen(txtFile_weight, "r");
    if (file_weight == NULL) {
        printf("Error: Could not open file %s\n", txtFile_weight);
        exit(1);
    }
    for (int i = 0; i < layer->output_sizes[0][1] * layer->input_sizes[0][1]; i++) {
        fscanf(file_weight, "%f", &layer->weights[i]);
    }
    fclose(file_weight);
    FILE* file_bias = fopen(txtFile_bias, "r");
    if (file_bias == NULL) {
        printf("Error: Could not open file %s\n", txtFile_bias);
        exit(1);
    }
    for (int i = 0; i < layer->output_sizes[0][1]; i++) {
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
/*
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
}*/

int test_single()
{
    
    Tensor* input_random_1 = (Tensor*)malloc(sizeof(Tensor));
    new_tensor(input_random_1, (int[]){BatchSize, 10}, 2);
    //input_random_1->dims[0] = BatchSize;
    //input_random_1->dims[1] = 10;
    //input_random_1->data = (float*)malloc(10 * BatchSize * sizeof(float));
    //input_random_1->size = 10 * BatchSize;
    //read_tensor("/mnt/sdcard/testcases/linear/input_tensor_random_1.txt", input_random_1);
    read_tensor("/home/a1009mys/Project/op-tee/op-tee-package/optee_project/optee_my_gpio/testcases/linear/input_tensor_random_1.txt", input_random_1);

    Model* model = (Model*)malloc(sizeof(Model));
    model->num_layers = 2;

    Layer* layer1 = (Layer*)malloc(sizeof(Layer));
    //layer1->type = LAYER_LINEAR_NEON; // or LAYER_LINEAR, depending on your implementation
    layer1->type = LAYER_LINEAR; // or LAYER_LINEAR_NEON, depending on your implementation
    layer1->input_size[0] = BatchSize;
    layer1->input_size[1] = 10;
    layer1->output_size[0] = BatchSize;
    layer1->output_size[1] = 6;
    layer1->weights = (float*)malloc(10 * 6 * sizeof(float));
    layer1->bias = (float*)malloc(6 * sizeof(float));
    //read_layer("/mnt/sdcard/testcases/linear/model_weights.txt", "/mnt/sdcard/testcases/linear/model_bias.txt", layer1);
    read_layer("/home/a1009mys/Project/op-tee/op-tee-package/optee_project/optee_my_gpio/testcases/linear/model_weights.txt", 
        "/home/a1009mys/Project/op-tee/op-tee-package/optee_project/optee_my_gpio/testcases/linear/model_bias.txt", layer1);

    Layer* layer2 = (Layer*)malloc(sizeof(Layer));
    //layer2->type = LAYER_RELU_NEON; // or LAYER_RELU, depending on your implementation
    layer2->type = LAYER_RELU; // or LAYER_RELU_NEON, depending on your implementation
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
    new_tensor(output, (int[]){BatchSize, 6}, 2);
    //output->dims[0] = BatchSize;
    //output->dims[1] = 6;
    //output->data = (float*)malloc(6 * BatchSize * sizeof(float));
    //output->size = 6 * BatchSize;

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

int test_multi()
{
    
    Tensor* input_random_1 = (Tensor*)malloc(sizeof(Tensor));
    new_tensor(input_random_1, (int[]){BatchSize, 10}, 2);
    //read_tensor("/mnt/sdcard/testcases/linear/input_tensor_random_1.txt", input_random_1);
    read_tensor("/home/a1009mys/Project/op-tee/op-tee-package/optee_project/optee_my_gpio/testcases/linear/input_tensor_random_1.txt", input_random_1);
    // print tensor data
    printf("Input tensor data:\n");
    for (uint32_t i = 0; i < input_random_1->size; ++i) {
        printf("%f\t", input_random_1->data[i]);
        if ((i + 1) % 10 == 0) {
            printf("\n");
        }
    }
    printf("\n\n");

    Model_Multi* model = (Model_Multi*)malloc(sizeof(Model_Multi));
    model->num_layers = 2;

    MultiIOLayer* layer1 = (MultiIOLayer*)malloc(sizeof(MultiIOLayer));
    //layer1->type = LAYER_LINEAR_NEON; // or LAYER_LINEAR, depending on your implementation
    layer1->type = LAYER_LINEAR; // or LAYER_LINEAR_NEON, depending on your implementation
    layer1->num_inputs = 1;
    layer1->num_outputs = 1;
    layer1->input_dim_count = 2; // 2D input
    layer1->output_dim_count = 2; // 2D output

    layer1->input_sizes[0][0] = BatchSize;
    layer1->input_sizes[0][1] = 10;
    layer1->output_sizes[0][0] = BatchSize;
    layer1->output_sizes[0][1] = 6;
    layer1->weights = (float*)malloc(10 * 6 * sizeof(float));
    layer1->bias = (float*)malloc(6 * sizeof(float));
    layer1->reference_layer_count = 1;
    layer1->previous_layer_count = 0;
    layer1->previous_layer_index[0] = -1;
    layer1->is_output = 0; // not output layer
    //read_layer("/mnt/sdcard/testcases/linear/model_weights.txt", "/mnt/sdcard/testcases/linear/model_bias.txt", layer1);
    read_multi("/home/a1009mys/Project/op-tee/op-tee-package/optee_project/optee_my_gpio/testcases/linear/model_weights.txt", 
        "/home/a1009mys/Project/op-tee/op-tee-package/optee_project/optee_my_gpio/testcases/linear/model_bias.txt", layer1);
    // print layer1 weights
    printf("Layer 1 weights:\n");
    for (uint32_t i = 0; i < 10 * 6; ++i) {
        printf("%f\t", layer1->weights[i]);
        if ((i + 1) % 6 == 0) {
            printf("\n");
        }
    }
    // print layer1 bias
    for (uint32_t i = 0; i < 6; ++i) {
        printf("%f\t", layer1->bias[i]);
    }
    printf("\n\n");

    MultiIOLayer* layer2 = (MultiIOLayer*)malloc(sizeof(MultiIOLayer));
    //layer2->type = LAYER_RELU_NEON; // or LAYER_RELU, depending on your implementation
    layer2->type = LAYER_RELU; // or LAYER_RELU_NEON, depending on your implementation
    layer2->num_inputs = 1;
    layer2->num_outputs = 1;
    layer2->input_dim_count = 2; // 2D input
    layer2->output_dim_count = 2; // 2D output
    layer2->input_sizes[0][0] = BatchSize;
    layer2->input_sizes[0][1] = 6;
    layer2->output_sizes[0][0] = BatchSize;
    layer2->output_sizes[0][1] = 6;
    layer2->weights = NULL;
    layer2->bias = NULL;
    layer2->reference_layer_count = 0;
    layer2->previous_layer_count = 1; // one previous layer
    layer2->previous_layer_index[0] = -1; // 
    layer2->is_output = 1; // this is the output layer


    
    model->layers[0] = layer1;
    model->layers[1] = layer2;

    Tensor* output = (Tensor*)malloc(sizeof(Tensor));
    new_tensor(output, (int[]){BatchSize, 6}, 2);
    //output->dims[0] = BatchSize;
    //output->dims[1] = 6;
    //output->data = (float*)malloc(6 * BatchSize * sizeof(float));
    //output->size = 6 * BatchSize;

    prepare_model(model);
    run_multi(model, (const Tensor*[]){input_random_1}, 1, &output, 1, BatchSize);

    for (uint32_t i = 0; i < output->size; ++i) {
        printf("%f\t", output->data[i]);
        if ((i + 1) % 6 == 0) {
            printf("\n");
        }
    }

    delete_tensor(input_random_1);
    free(layer1->weights);
    free(layer1->bias);
    free(layer1);
    free(layer2);
    delete_tensor(output);
    free(model);
    printf("test_multi finished\n");
}

int test_multi_linear3() {
    //char* testcasesPath = "/home/a1009mys/Project/op-tee/op-tee-package/optee_project/optee_my_gpio/testcases/linear3/";
    char* testcasesPath = "/mnt/sdcard/testcases/linear3/";
    // test multi linear 3 layers
    /*
    class LinearModel3(nn.Module):
    def __init__(self, input_dim, middle_dim, output_dim):
        super(LinearModel3, self).__init__()
        self.linear1 = nn.Linear(input_dim, middle_dim)
        self.linear2 = nn.Linear(middle_dim, middle_dim)
        self.linear3 = nn.Linear(middle_dim, output_dim)

        

    def forward(self, x):
        x = self.linear1(x)
        x = F.relu(x)
        x = self.linear2(x)
        x = F.relu(x)
        x = self.linear3(x)
        x = F.relu(x)
        return x
    */
    int input_dim = 10;
    int middle_dim = 8;
    int output_dim = 6;

    Tensor* input_random_1 = (Tensor*)malloc(sizeof(Tensor));
    new_tensor(input_random_1, (int[]){BatchSize, 10}, 2);
    //read_tensor("/mnt/sdcard/testcases/linear/input_tensor_random_1.txt", input_random_1);
    //read_tensor("/home/a1009mys/Project/op-tee/op-tee-package/optee_project/optee_my_gpio/testcases/linear3/input_tensor_random_1.txt", input_random_1);
    // combine testcasesPath and input_tensor_random_1.txt
    char input_tensor_path[256];
    snprintf(input_tensor_path, sizeof(input_tensor_path), "%sinput_tensor_random_1.txt", testcasesPath);
    read_tensor(input_tensor_path, input_random_1);
    // print tensor data
    printf("Input tensor data:\n");
    for (uint32_t i = 0; i < input_random_1->size; ++i) {
        printf("%f\t", input_random_1->data[i]);
        if ((i + 1) % 10 == 0) {
            printf("\n");
        }
    }
    printf("\n\n");
    
    Model_Multi* model = (Model_Multi*)malloc(sizeof(Model_Multi));
    model->num_layers = 6; 

    MultiIOLayer* layer1 = (MultiIOLayer*)malloc(sizeof(MultiIOLayer));
    #ifdef __ARM_NEON__
    layer1->type = LAYER_LINEAR_NEON; // or LAYER_LINEAR, depending on your implementation
    #else
    layer1->type = LAYER_LINEAR;
    #endif // __ARM_NEON__
    layer1->num_inputs = 1;
    layer1->num_outputs = 1;
    layer1->input_dim_count = 2; // 2D input
    layer1->output_dim_count = 2; // 2D output
    layer1->input_sizes[0][0] = BatchSize;
    layer1->input_sizes[0][1] = input_dim;
    layer1->output_sizes[0][0] = BatchSize;
    layer1->output_sizes[0][1] = middle_dim;
    layer1->weights = (float*)malloc(input_dim * middle_dim * sizeof(float));
    layer1->bias = (float*)malloc(middle_dim * sizeof(float));
    layer1->reference_layer_count = 1;
    layer1->previous_layer_count = 0;
    layer1->previous_layer_index[0] = -1; // only one input layer
    //read_layer("/mnt/sdcard/testcases/linear3/model_weights_linear1.txt", "/mnt/sdcard/testcases/linear3/model_bias_linear1.txt", layer1);
    //read_multi("/home/a1009mys/Project/op-tee/op-tee-package/optee_project/optee_my_gpio/testcases/linear3/model_weights_linear1.txt", 
    //    "/home/a1009mys/Project/op-tee/op-tee-package/optee_project/optee_my_gpio/testcases/linear3/model_bias_linear1.txt", layer1);
    char layer1_weights_path[256];
    snprintf(layer1_weights_path, sizeof(layer1_weights_path), "%smodel_weights_linear1.txt", testcasesPath);
    char layer1_bias_path[256];
    snprintf(layer1_bias_path, sizeof(layer1_bias_path), "%smodel_bias_linear1.txt", testcasesPath);
    read_multi(layer1_weights_path, layer1_bias_path, layer1);
    // print layer1 weights
    printf("Layer 1 weights:\n");
    for (uint32_t i = 0; i < input_dim * middle_dim; ++i) {
        printf("%f\t", layer1->weights[i]);
        if ((i + 1) % middle_dim == 0) {
            printf("\n");
        }
    }
    // print layer1 bias
    for (uint32_t i = 0; i < middle_dim; ++i) {
        printf("%f\t", layer1->bias[i]);
    }
    printf("\n\n");
    MultiIOLayer* layer2 = (MultiIOLayer*)malloc(sizeof(MultiIOLayer));
    #ifdef __ARM_NEON__
    layer2->type = LAYER_RELU_NEON; // or LAYER_RELU, depending on your implementation
    #else
    layer2->type = LAYER_RELU;
    #endif // __ARM_NEON__
    layer2->num_inputs = 1;
    layer2->num_outputs = 1;
    layer2->input_dim_count = 2; // 2D input
    layer2->output_dim_count = 2; // 2D output
    layer2->input_sizes[0][0] = BatchSize;
    layer2->input_sizes[0][1] = middle_dim;
    layer2->output_sizes[0][0] = BatchSize;
    layer2->output_sizes[0][1] = middle_dim;
    layer2->weights = NULL;
    layer2->bias = NULL;
    layer2->reference_layer_count = 1;
    layer2->previous_layer_count = 1; // one previous layer
    layer2->previous_layer_index[0] = -1; //
    layer2->is_output = 0; // not output layer

    MultiIOLayer* layer3 = (MultiIOLayer*)malloc(sizeof(MultiIOLayer));
    #ifdef __ARM_NEON__
    layer3->type = LAYER_LINEAR_NEON; // or LAYER_LINEAR_NEON, depending on your implementation
    #else
    layer3->type = LAYER_LINEAR; // or LAYER_LINEAR_NEON, depending on your implementation
    #endif // __ARM_NEON__
    layer3->num_inputs = 1;
    layer3->num_outputs = 1;
    layer3->input_dim_count = 2; // 2D input
    layer3->output_dim_count = 2; // 2D output
    layer3->input_sizes[0][0] = BatchSize;
    layer3->input_sizes[0][1] = middle_dim;
    layer3->output_sizes[0][0] = BatchSize;
    layer3->output_sizes[0][1] = middle_dim;
    layer3->weights = (float*)malloc(middle_dim * middle_dim * sizeof(float));
    layer3->bias = (float*)malloc(middle_dim * sizeof(float));
    layer3->reference_layer_count = 1;
    layer3->previous_layer_count = 1;
    layer3->previous_layer_index[0] = -1; // previous layer is layer2
    layer3->is_output = 0; // not output layer
    //read_layer("/mnt/sdcard/testcases/linear3/model_weights_linear2.txt", "/mnt/sdcard/testcases/linear3/model_bias_linear2.txt", layer3);
    //read_multi("/home/a1009mys/Project/op-tee/op-tee-package/optee_project/optee_my_gpio/testcases/linear3/model_weights_linear2.txt", 
    //    "/home/a1009mys/Project/op-tee/op-tee-package/optee_project/optee_my_gpio/testcases/linear3/model_bias_linear2.txt", layer3);
    char layer3_weights_path[256];
    snprintf(layer3_weights_path, sizeof(layer3_weights_path), "%smodel_weights_linear2.txt", testcasesPath);
    char layer3_bias_path[256];
    snprintf(layer3_bias_path, sizeof(layer3_bias_path), "%smodel_bias_linear2.txt", testcasesPath);
    read_multi(layer3_weights_path, layer3_bias_path, layer3);
    // print layer3 weights
    printf("Layer 3 weights:\n");
    for (uint32_t i = 0; i < middle_dim * middle_dim; ++i) {
        printf("%f\t", layer3->weights[i]);
        if ((i + 1) % middle_dim == 0) {
            printf("\n");
        }
    }
    // print layer3 bias
    for (uint32_t i = 0; i < middle_dim; ++i) {
        printf("%f\t", layer3->bias[i]);
    }
    printf("\n\n");

    MultiIOLayer* layer4 = (MultiIOLayer*)malloc(sizeof(MultiIOLayer));
    #ifdef __ARM_NEON__
    layer4->type = LAYER_RELU_NEON; // or LAYER_RELU_NEON, depending on your implementation
    #else
    layer4->type = LAYER_RELU; // or LAYER_RELU_NEON, depending on your implementation
    #endif // __ARM_NEON__
    layer4->num_inputs = 1;
    layer4->num_outputs = 1;
    layer4->input_dim_count = 2; // 2D input
    layer4->output_dim_count = 2; // 2D output
    layer4->input_sizes[0][0] = BatchSize;
    layer4->input_sizes[0][1] = middle_dim;
    layer4->output_sizes[0][0] = BatchSize;
    layer4->output_sizes[0][1] = middle_dim;
    layer4->weights = NULL;
    layer4->bias = NULL;
    layer4->reference_layer_count = 1;
    layer4->previous_layer_count = 1; // one previous layer
    layer4->previous_layer_index[0] = -1; // previous layer is layer3
    layer4->is_output = 0; // not output layer

    MultiIOLayer* layer5 = (MultiIOLayer*)malloc(sizeof(MultiIOLayer));
    #ifdef __ARM_NEON__
    layer5->type = LAYER_LINEAR_NEON; // or LAYER_LINEAR_NEON, depending on your implementation
    #else
    layer5->type = LAYER_LINEAR; // or LAYER_LINEAR_NEON, depending on your implementation
    #endif // __ARM_NEON__
    layer5->num_inputs = 1;
    layer5->num_outputs = 1;
    layer5->input_dim_count = 2; // 2D input
    layer5->output_dim_count = 2; // 2D output
    layer5->input_sizes[0][0] = BatchSize;
    layer5->input_sizes[0][1] = middle_dim;
    layer5->output_sizes[0][0] = BatchSize;
    layer5->output_sizes[0][1] = output_dim;
    layer5->weights = (float*)malloc(middle_dim * output_dim * sizeof(float));
    layer5->bias = (float*)malloc(output_dim * sizeof(float));
    layer5->reference_layer_count = 0;
    layer5->previous_layer_count = 1; // one previous layer
    layer5->previous_layer_index[0] = -1; // previous layer is layer4
    layer5->is_output = 0; // not output layer
    //read_layer("/mnt/sdcard/testcases/linear3/model_weights_linear3.txt", "/mnt/sdcard/testcases/linear3/model_bias_linear3.txt", layer5);
    //read_multi("/home/a1009mys/Project/op-tee/op-tee-package/optee_project/optee_my_gpio/testcases/linear3/model_weights_linear3.txt", 
    //    "/home/a1009mys/Project/op-tee/op-tee-package/optee_project/optee_my_gpio/testcases/linear3/model_bias_linear3.txt", layer5);
    char layer5_weights_path[256];
    snprintf(layer5_weights_path, sizeof(layer5_weights_path), "%smodel_weights_linear3.txt", testcasesPath);
    char layer5_bias_path[256];
    snprintf(layer5_bias_path, sizeof(layer5_bias_path), "%smodel_bias_linear3.txt", testcasesPath);
    read_multi(layer5_weights_path, layer5_bias_path, layer5);

    // print layer5 weights
    printf("Layer 5 weights:\n");
    for (uint32_t i = 0; i < middle_dim * output_dim; ++i) {
        printf("%f\t", layer5->weights[i]);
        if ((i + 1) % output_dim == 0) {
            printf("\n");
        }
    }
    // print layer5 bias
    for (uint32_t i = 0; i < output_dim; ++i) {
        printf("%f\t", layer5->bias[i]);
    }
    printf("\n\n");

    MultiIOLayer* layer6 = (MultiIOLayer*)malloc(sizeof(MultiIOLayer));
    #ifdef __ARM_NEON__
    layer6->type = LAYER_RELU_NEON; // or LAYER_RELU_NEON, depending on your implementation
    #else
    layer6->type = LAYER_RELU; // or LAYER_RELU_NEON, depending on your implementation
    #endif // __ARM_NEON__
    layer6->num_inputs = 1;
    layer6->num_outputs = 1;
    layer6->input_dim_count = 2; // 2D input
    layer6->output_dim_count = 2; // 2D output
    layer6->input_sizes[0][0] = BatchSize;
    layer6->input_sizes[0][1] = output_dim;
    layer6->output_sizes[0][0] = BatchSize;
    layer6->output_sizes[0][1] = output_dim;
    layer6->weights = NULL;
    layer6->bias = NULL;
    layer6->reference_layer_count = 0;
    layer6->previous_layer_count = 1; // one previous layer
    layer6->previous_layer_index[0] = -1; // previous layer is layer5
    layer6->is_output = 1; // this is the output layer

    model->layers[0] = layer1;
    model->layers[1] = layer2;
    model->layers[2] = layer3;
    model->layers[3] = layer4;
    model->layers[4] = layer5;
    model->layers[5] = layer6;
    Tensor* output = (Tensor*)malloc(sizeof(Tensor));
    new_tensor(output, (int[]){BatchSize, output_dim}, 2);
    //output->dims[0] = BatchSize;
    //output->dims[1] = output_dim;
    //output->data = (float*)malloc(output_dim * BatchSize * sizeof(float));
    //output->size = output_dim * BatchSize;
    prepare_model(model);
    run_multi(model, (const Tensor*[]){input_random_1}, 1, &output, 1, BatchSize);
    printf("Output tensor data:\n");
    for (uint32_t i = 0; i < output->size; ++i) {
        printf("%f\t", output->data[i]);
        if ((i + 1) % output_dim == 0) {
            printf("\n");
        }
    }
    printf("\n\n");
    delete_tensor(input_random_1);
    free(layer1->weights);
    free(layer1->bias);
    free(layer1);
    free(layer2);
    free(layer3->weights);
    free(layer3->bias);
    free(layer3);
    free(layer4);
    free(layer5->weights);
    free(layer5->bias);
    free(layer5);
    free(layer6);
    delete_tensor(output);
    free(model);
    printf("test_multi_linear3 finished\n");
    return 0;



}

int main()
{

    // test_AlexNet();
    //test_single();
    //test_multi();
    test_multi_linear3();

    return 0;
}