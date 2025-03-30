import onnx

onnx_model = onnx.load("/home/a1009mys/Downloads/alexnet_Opset17.onnx")

print("==[ 모델 가중치 목록 ]==")
for initializer in onnx_model.graph.initializer:
    name = initializer.name
    dims = initializer.dims
    data_type = initializer.data_type
    array = onnx.numpy_helper.to_array(initializer)
    print(f"\n[Name]: {name}")
    print(f"Shape: {dims}")
    print(f"Values (first 5): {array.flatten()[:5]}")