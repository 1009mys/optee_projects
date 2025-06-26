cmake_minimum_required(VERSION 3.30)
project(ksj_DLruntime)

set(CMAKE_CXX_STANDARD 20)

add_executable(ksj_DLruntime main.cpp
        Tensor.cpp
        Tensor.h
        defines.h)
