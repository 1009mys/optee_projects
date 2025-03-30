import torch
import numpy as np
import random
import os
import sys
import argparse
import json
import time
import math
import copy
import torch.nn.functional as F
from torch.autograd import Variable
from torch import nn

# random seed
random.seed(0)

# Define the model
class LinearModel(nn.Module):
    def __init__(self, input_dim, output_dim):
        super(LinearModel, self).__init__()
        self.linear1 = nn.Linear(input_dim, output_dim)
        

    def forward(self, x):
        x = self.linear1(x)
        x = F.relu(x)
        return x
    

def generate_testcase_linear(testcases_dir = "./testcases/linear"):
    # Define the input and output dimension
    input_dim = 10
    output_dim = 6
    model = LinearModel(input_dim, output_dim)
    model.eval()

    batch_size = 4
    
    # Define the random input tensor 
    input_tensor_random_1 = torch.randn(batch_size, input_dim)
    input_tensor_random_2 = torch.randn(batch_size, input_dim)
    input_tensor_random_3 = torch.randn(batch_size, input_dim)

    # Define the input tensor with all zeros
    input_tensor_zeros = torch.zeros(batch_size, input_dim)

    # Define the input tensor with all ones
    input_tensor_ones = torch.ones(batch_size, input_dim)
    
    # Export the model
    output_random_1 = model(input_tensor_random_1)
    output_random_2 = model(input_tensor_random_2)
    output_random_3 = model(input_tensor_random_3)
    output_zeros = model(input_tensor_zeros)
    output_ones = model(input_tensor_ones)

    print("input_tensor_random_1: ", input_tensor_random_1)
    print("output_random_1: ", output_random_1)
    print("input_tensor_random_2: ", input_tensor_random_2)
    print("output_random_2: ", output_random_2)
    print("input_tensor_random_3: ", input_tensor_random_3)
    print("output_random_3: ", output_random_3)
    print("input_tensor_zeros: ", input_tensor_zeros)
    print("output_zeros: ", output_zeros)
    print("input_tensor_ones: ", input_tensor_ones)
    print("output_ones: ", output_ones)

    # print model weights
    print("model weights: ", model.linear1.weight)
    print("model bias: ", model.linear1.bias)

    # save tensors to txt files
    np.savetxt(testcases_dir + "input_tensor_random_1.txt", input_tensor_random_1.numpy())
    np.savetxt(testcases_dir + "output_random_1.txt", output_random_1.detach().numpy())
    np.savetxt(testcases_dir + "input_tensor_random_2.txt", input_tensor_random_2.numpy())
    np.savetxt(testcases_dir + "output_random_2.txt", output_random_2.detach().numpy())
    np.savetxt(testcases_dir + "input_tensor_random_3.txt", input_tensor_random_3.numpy())
    np.savetxt(testcases_dir + "output_random_3.txt", output_random_3.detach().numpy())
    np.savetxt(testcases_dir + "input_tensor_zeros.txt", input_tensor_zeros.numpy())
    np.savetxt(testcases_dir + "output_zeros.txt", output_zeros.detach().numpy())
    np.savetxt(testcases_dir + "input_tensor_ones.txt", input_tensor_ones.numpy())
    np.savetxt(testcases_dir + "output_ones.txt", output_ones.detach().numpy())

    np.savetxt(testcases_dir + "model_weights.txt", model.linear1.weight.detach().numpy())
    np.savetxt(testcases_dir + "model_bias.txt", model.linear1.bias.detach().numpy())

# Define the model
class AlexNet(nn.Module):
    """
    input dimension: Nx3x224x224
    """
    def __init__(self):
        super(AlexNet, self).__init__()
        layers = []
        layers.append(nn.Conv2d(3, 64, kernel_size=11, stride=4, padding=2))
        layers.append(nn.ReLU(inplace=True))
        layers.append(nn.MaxPool2d(kernel_size=3, stride=2))
        layers.append(nn.Conv2d(64, 192, kernel_size=5, padding=2))
        layers.append(nn.ReLU(inplace=True))
        layers.append(nn.MaxPool2d(kernel_size=3, stride=2))
        layers.append(nn.Conv2d(192, 384, kernel_size=3, padding=1))
        layers.append(nn.ReLU(inplace=True))
        layers.append(nn.Conv2d(384, 256, kernel_size=3, padding=1))
        layers.append(nn.ReLU(inplace=True))
        layers.append(nn.Conv2d(256, 256, kernel_size=3, padding=1))
        layers.append(nn.ReLU(inplace=True))
        layers.append(nn.MaxPool2d(kernel_size=3, stride=2))
        self.features = nn.Sequential(*layers)
        self.avgpool = nn.AdaptiveAvgPool2d((6, 6))
        self.classifier = nn.Sequential(
            nn.Dropout(),
            nn.Linear(256 * 6 * 6, 4096),
            nn.ReLU(inplace=True),
            nn.Dropout(),
            nn.Linear(4096, 4096),
            nn.ReLU(inplace=True),
            nn.Linear(4096, 1000),
        )

    def forward(self, x):
        x = self.features(x)
        x = self.avgpool(x)
        x = torch.flatten(x, 1)
        x = self.classifier(x)
        return x

        

def generate_testcase_Alexnet(testcases_dir = "./testcases/alexnet/"):
    if not os.path.exists(testcases_dir):
        os.makedirs(testcases_dir)
    # Define the input and output dimension
    batch_size = 4
    Channels = 3
    Height = 224
    Width = 224
    input_dim = (batch_size, Channels, Height, Width)
    model = AlexNet()
    model.eval()

    
    # Define the random input tensor 
    input_tensor_random_1 = torch.randn(batch_size, Channels, Height, Width)
    input_tensor_random_2 = torch.randn(batch_size, Channels, Height, Width)
    input_tensor_random_3 = torch.randn(batch_size, Channels, Height, Width)

    # Define the input tensor with all zeros
    input_tensor_zeros = torch.zeros(batch_size, Channels, Height, Width)

    # Define the input tensor with all ones
    input_tensor_ones = torch.ones(batch_size, Channels, Height, Width)
    
    # Export the model
    output_random_1 = model(input_tensor_random_1)
    output_random_2 = model(input_tensor_random_2)
    output_random_3 = model(input_tensor_random_3)
    output_zeros = model(input_tensor_zeros)
    output_ones = model(input_tensor_ones)

    print("input_tensor_random_1: ", input_tensor_random_1)
    print("output_random_1: ", output_random_1)
    print("input_tensor_random_2: ", input_tensor_random_2)
    print("output_random_2: ", output_random_2)
    print("input_tensor_random_3: ", input_tensor_random_3)
    print("output_random_3: ", output_random_3)
    print("input_tensor_zeros: ", input_tensor_zeros)
    print("output_zeros: ", output_zeros)
    print("input_tensor_ones: ", input_tensor_ones)
    print("output_ones: ", output_ones)

    # save layer list
    layer_list = []
    for layer in model.features:
        if isinstance(layer, nn.Conv2d):
            layer_list.append("Conv2d")
        if isinstance(layer, nn.ReLU):
            layer_list.append("ReLU")
        if isinstance(layer, nn.MaxPool2d):
            layer_list.append("MaxPool2d")

    for layer in model.classifier:
        if isinstance(layer, nn.Linear):
            layer_list.append("Linear")
        if isinstance(layer, nn.ReLU):
            layer_list.append("ReLU")
        if isinstance(layer, nn.Dropout):
            layer_list.append("Dropout")

    with open(testcases_dir + "layer_list.json", "w") as f:
        json.dump(layer_list, f)



    # save tensors to txt files
    # change dimension to 1D
    input_tensor_random_1 = input_tensor_random_1.view(-1)
    input_tensor_random_2 = input_tensor_random_2.view(-1)
    input_tensor_random_3 = input_tensor_random_3.view(-1)
    input_tensor_zeros = input_tensor_zeros.view(-1)
    input_tensor_ones = input_tensor_ones.view(-1)
    output_random_1 = output_random_1.view(-1)
    output_random_2 = output_random_2.view(-1)
    output_random_3 = output_random_3.view(-1)
    output_zeros = output_zeros.view(-1)
    output_ones = output_ones.view(-1)
    np.savetxt(testcases_dir + "input_tensor_random_1.txt", input_tensor_random_1.numpy())
    np.savetxt(testcases_dir + "output_random_1.txt", output_random_1.detach().numpy())
    np.savetxt(testcases_dir + "input_tensor_random_2.txt", input_tensor_random_2.numpy())
    np.savetxt(testcases_dir + "output_random_2.txt", output_random_2.detach().numpy())
    np.savetxt(testcases_dir + "input_tensor_random_3.txt", input_tensor_random_3.numpy())
    np.savetxt(testcases_dir + "output_random_3.txt", output_random_3.detach().numpy())
    np.savetxt(testcases_dir + "input_tensor_zeros.txt", input_tensor_zeros.numpy())
    np.savetxt(testcases_dir + "output_zeros.txt", output_zeros.detach().numpy())
    np.savetxt(testcases_dir + "input_tensor_ones.txt", input_tensor_ones.numpy())
    np.savetxt(testcases_dir + "output_ones.txt", output_ones.detach().numpy())

    # print model weights of all layers
    """
    print("model weights of all layers: ")
    for layer in model.features:
        if isinstance(layer, nn.Conv2d):
            print("layer weight: ", layer.weight)
            print("layer bias: ", layer.bias)
    for layer in model.classifier:
        if isinstance(layer, nn.Linear):
            print("layer weight: ", layer.weight)
            print("layer bias: ", layer.bias)
    """

    # save model weights of all layers with layer number
    layer_num = 0
    for layer in model.features:
        if isinstance(layer, nn.Conv2d):
            np.savetxt(testcases_dir + "layer" + str(layer_num) + "_weight.txt", layer.weight.view(-1).detach().numpy())
            np.savetxt(testcases_dir + "layer" + str(layer_num) + "_bias.txt", layer.bias.view(-1).detach().numpy())
            layer_num += 1
    for layer in model.classifier:
        if isinstance(layer, nn.Linear):
            np.savetxt(testcases_dir + "layer" + str(layer_num) + "_weight.txt", layer.weight.view(-1).detach().numpy())
            np.savetxt(testcases_dir + "layer" + str(layer_num) + "_bias.txt", layer.bias.view(-1).detach().numpy())
            layer_num += 1
    """
    for i, layer in enumerate(model.features):
        if isinstance(layer, nn.Conv2d):
            np.savetxt(testcases_dir + "model_features_" + str(i) + "_weight.txt", layer.weight.view(-1).detach().numpy())
            np.savetxt(testcases_dir + "model_features_" + str(i) + "_bias.txt", layer.bias.detach().numpy())
    for i, layer in enumerate(model.classifier):
        if isinstance(layer, nn.Linear):
            np.savetxt(testcases_dir + "model_classifier_" + str(i) + "_weight.txt", layer.weight.view(-1).detach().numpy())
            np.savetxt(testcases_dir + "model_classifier_" + str(i) + "_bias.txt", layer.bias.detach().numpy())
    """
    


if __name__ == '__main__':
    #generate_testcase_linear()
    generate_testcase_Alexnet()