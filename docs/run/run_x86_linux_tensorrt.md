# X86_Linux_TensorRT
这是X86 Linux平台上TensorRT Tools工具脚本的说明文档。

---
# 一、启动Docker
首先拉取编译运行镜像，命令如下：
```bash
docker pull daipuwei/x86_deeplearning_runtime:cuda11.8_ubuntu20.04
```
拉取镜像成功后，启动镜像，命令如下：
```bash
# 根据自身需要修改docker/x86_deeplearing_deloy_docker/init_docker.sh文件中宿主机挂载路径
# 然后选择合适端口号和cpu序列号，用如下命令启动镜像
bash docker/x86_deeplearing_deloy_docker/init_docker.sh 44873 x86_deeplearning_runtime daipuwei/x86_deeplearning_runtime:cuda11.8_ubuntu20.04 0 19
```
接下来可以选择SSH工具进行配置进入容器，也可以使用系统终端进入容器。使用系统终端进入容器命令如下：
```bash
# 查看容器id
docker ps -a
# 进入容器
docker exec -it container_id /bin/bash 
```

---

# 二、编译
进行编译代码，命令如下：
```bash
# x86_linux_tensorrt,Debug模式
./build.sh -platform x86_linux_tensorrt -build Debug
# x86_linux_tensorrt,Release模式
./build.sh -platform x86_linux_tensorrt -build Release
```

---

# 三、运行
可执行程序主要包括:`onnx2tensorrt`。
## 3.1 onnx2tensorrt
该工具实现ONNX模型转换为TensorRT模型，支持fp32、fp16和int8，仅支持静态模型转换，动态模型转换尚不支持，其中运行参数详情如下。

| 参数名称                  | 备注                                                                    |
|:----------------------|:----------------------------------------------------------------------|
| onnx_model_path       | ONNX模型路径                                                              |
| mode                  | tensorrt模型类型，候选值为fp32，fp16，int8                                       |
| batch_size            | ONNX模型batch_size                                                      |
| input_width           | ONNX模型input_width                                                     |
| input_height          | ONNX模型input_height                                                    |
| input_channel         | ONNX模型input_channel                                                   |
| is_nchw               | ONNX模型输入是否为NCHW格式标志位，候选值为0和1，0代表ONNX模型输入为NHWC格式，1代表ONNX模型输入为NCHW格式    |
| use_normlize          | ONNX模型是否使用归一化操作节点标志位，候选值为0和1，0代表ONNX模型没有使用归一化操作节点，1代表ONNX模型使用了归一化操作节点 |
| gpu_device_id         | GPU设备号，默认为0                                                           |
| calibrator_image_dir  | INT8量化校准图像集文件夹路径，一般在mode设置成int8时使用，fp32和fp16时可以不赋值  |
| calibrator_table_path | INT8量化校准表路径，一般在mode设置成int8时使用，fp32和fp16时可以不赋值|


ONNX模型转TensorRT模型命令如下：
```bash
# 执行转换模型命令前，使用Netron工具查看ONNX模型输入节点，确定is_nchw和use_normlize
# fp32
./bin/onnx2tensorrt --onnx_model_path /home/dpw/dpw/code/YOLO_Deploy_CXX/model_data/yolov5n_div255_nhwc.onnx \
                    --mode fp32 \
                    --batch_size 1 \
                    --input_width 640 \
                    --input_height 640 \
                    --input_channel 3 \
                    --is_nchw 0 \
                    --use_normlize 1 \
                    --gpu_device_id 0

# fp16
./bin/onnx2tensorrt --onnx_model_path /home/dpw/dpw/code/YOLO_Deploy_CXX/model_data/yolov5n_div255_nhwc.onnx \
                    --mode fp16 \
                    --batch_size 1 \
                    --input_width 640 \
                    --input_height 640 \
                    --input_channel 3 \
                    --is_nchw 0 \
                    --use_normlize 1 \
                    --gpu_device_id 0

# int8
./bin/onnx2tensorrt --onnx_model_path /home/dpw/dpw/code/YOLO_Deploy_CXX/model_data/yolov5n_div255_nhwc.onnx \
                    --mode int8 \
                    --calibrator_image_dir /store1/dataset/test/coco_calib/ \
                    --calibrator_table_path /home/dpw/dpw/code/YOLO_Deploy_CXX/model_data/yolov5n_div255_nhwc.cache \
                    --batch_size 1 \
                    --input_width 640 \
                    --input_height 640 \
                    --input_channel 3 \
                    --is_nchw 0 \
                    --use_normlize 1 \
                    --gpu_device_id 0
```