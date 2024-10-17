//
// Created by dpw on 24-2-29.
//

#include "vector"
#include <numeric>
#include "NvInfer.h"
#include "NvOnnxParser.h"
#include "NvInferPlugin.h"
#include "device_launch_parameters.h"
#include "NvInferVersion.h"
#include "NvInferRuntimeCommon.h"
#include "spdlog/spdlog.h"
#include "fstream"
#include "ostream"
#include "iostream"
#include <cuda_runtime.h>

#include "logger.h"
#include "cmdline.h"
#include "common_utils.h"
#include "yolov5_trt_calibrator.h"

using namespace nvinfer1;
using namespace nvonnxparser;

#if NV_TENSORRT_MAJOR <= 7
/**
 * @brief 这是ONNX转TensorRT模型的函数,支持TensorRT7及其更低版本API
 * @param onnx_model_path onnx模型路径
 * @param tensorrt_model_path tensorrt模型路径
 * @param gpu_device_id gpu设备号
 * @param int8_calibrator INT8校准类
 * @return 返回是否成功生成tensorrt模型布尔量
 */
bool onnx2tensorrt_v7(std::string onnx_model_path, std::string tensorrt_model_path,
                      int gpu_device_id,std::string mode,nvinfer1::IInt8EntropyCalibrator2* int8_calibrator)
{
    Logger logger(Severity::kVERBOSE);
    // 设置显卡和加载TensorRT默认插件
    cudaSetDevice(gpu_device_id);
    initLibNvInferPlugins(&logger, "");
    spdlog::info("Start generating tensorRT model");


    // 创建ONNX模型解析类实例
    NetworkDefinitionCreationFlags flags=1;
    IBuilder* builder = createInferBuilder(logger);
    INetworkDefinition* network = builder->createNetworkV2(flags);
    // parse the caffe model to populate the network, then set the outputs
    IParser* parser = createParser(*network, logger);

    // 初始化模型精度标志位
    bool use_fp16 = builder->platformHasFastFp16() && (mode == "fp16");
    bool use_int8 = builder->platformHasFastInt8() && (mode == "int8");

    // 解析ONNX模型
    spdlog::info("parsing onnx model");
    bool parsed = parser->parseFromFile(onnx_model_path.c_str(),2);
    if (!parsed){
        spdlog::error("Parse failure");
        return false;          // 解析onnx文件失败，返回false
    }

    // 创建tensorRT引擎
    builder->setMaxWorkspaceSize(1 << 30);          // 设置最大空间
    if(use_int8){
        spdlog::info("generating INT8 tensorRT model");
        builder->setInt8Mode(true);
        builder->setInt8Calibrator(int8_calibrator);
    }else if(use_fp16){           // 设置FP16精度量化
        spdlog::info("generating FP16 tensorRT model");
        builder->setFp16Mode(true);
    }else{                 // FP32精度量化
        spdlog::info("generating FP32 tensorRT model");
    }
    ICudaEngine* engine = builder->buildCudaEngine(*network);
    if(engine == nullptr){
        spdlog::error("ERROR ONNX To TensorRT Model" );
    }
    assert(engine);

    // 引擎序列化，保存为文件
    std::ofstream tensorrt_file(tensorrt_model_path, std::ios::binary);
    if (!tensorrt_file){
        spdlog::error("could not open plan output file");
        return false;       // 无法打开tensorRT模型，返回false
    }
    IHostMemory *ptr = engine->serialize();
    assert(ptr);
    tensorrt_file.write(reinterpret_cast<const char*>(ptr->data()), ptr->size());   // 生成车牌OCR模型的tensorRT文件
    tensorrt_file.close();
    spdlog::info("Finish generating tensorRT model,tensorrt model saved in {}",tensorrt_model_path);

    // 释放内存空间
    delete ptr;
    delete engine;
    delete config;
    delete parser;
    delete network;
    delete builder;

    return true;
}
#else
/**
 * @brief 这是ONNX转TensorRT模型的函数,支持TensorRT８及其更高版本API
 * @param onnx_model_path onnx模型路径
 * @param tensorrt_model_path tensorrt模型路径
 * @param gpu_device_id gpu设备号
 * @param mode tensorrt模型精度
 * @param int8_calibrator INT8校准类
 * @return 返回是否成功生成tensorrt模型布尔量
 */
bool onnx2tensorrt_v8(std::string onnx_model_path, std::string tensorrt_model_path,
                      int gpu_device_id,std::string mode,nvinfer1::IInt8EntropyCalibrator2* int8_calibrator) {
    Logger logger(Severity::kVERBOSE);
    // 设置显卡和加载TensorRT默认插件
    cudaSetDevice(gpu_device_id);
    initLibNvInferPlugins(&logger, "");
//    std::cout<<"Start generating tensorRT model"<<std::endl;
    spdlog::info("Start generating tensorRT model");

    // 创建ONNX模型解析类实例
    NetworkDefinitionCreationFlags flags=1;
    IBuilder* builder = createInferBuilder(logger);
    INetworkDefinition* network = builder->createNetworkV2(flags);
    IBuilderConfig* config = builder->createBuilderConfig();
    // parse the caffe model to populate the network, then set the outputs
    IParser* parser = createParser(*network, logger);

    // 初始化模型精度标志位
    bool use_fp16 = builder->platformHasFastFp16() && (mode == "fp16");
    bool use_int8 = builder->platformHasFastInt8() && (mode == "int8");

    // 解析ONNX模型
    spdlog::info("Parsing ONNX model");
    bool parsed = parser->parseFromFile(onnx_model_path.c_str(),4);
    if (!parsed){
        std::cerr << "Parse failure" << std::endl;
        spdlog::error("Parse failure");
        return false;          // 解析onnx文件失败，返回false
    }

    // 创建tensorRT引擎
//    config->setMaxWorkspaceSize(1 << 30 );          // 设置最大空间,1G
    config->setMaxWorkspaceSize(256*(1 << 30 ));          // 设置最大空间,256M
    if(use_int8){
        spdlog::info("generating INT8 tensorRT model");
        config->setFlag(nvinfer1::BuilderFlag::kINT8);
        config->setInt8Calibrator(int8_calibrator);
    }else if(use_fp16){           // 设置FP16精度量化
        spdlog::info("generating FP16 tensorRT model");
        config->setFlag(nvinfer1::BuilderFlag::kFP16);
    }else{                 // FP32精度量化
        spdlog::info("generating FP32 tensorRT model");
    }
    ICudaEngine* engine = builder->buildEngineWithConfig(*network, *config);
    if(engine == nullptr){
        spdlog::error("ERROR ONNX To TensorRT Model");
    }
    assert(engine);

    // 引擎序列化，保存为文件
    std::ofstream tensorrt_file(tensorrt_model_path, std::ios::binary);
    if (!tensorrt_file){
//        std::cerr << "could not open plan output file" << std::endl;
        spdlog::error("could not open plan output file");
        return false;       // 无法打开tensorRT模型，返回false
    }
    IHostMemory *ptr = engine->serialize();
    assert(ptr);
    unsigned char *p = (unsigned char *)ptr->data();
    tensorrt_file.write((char*)p, ptr->size());
    tensorrt_file.close();
    spdlog::info("Finish generating tensorRT model,tensorrt model saved in {}",tensorrt_model_path);

    // 释放内存空间
    ptr->destroy();
    engine->destroy();
    config->destroy();
    parser->destroy();
    network->destroy();
    builder->destroy();

    return true;
}
#endif


/**
 * @brief 这是ONNX转TensorRT模型的哈市农户
 * @param parser 命令行解析类
*/
bool onnx2tensorrt(cmdline::parser &parser)
{
    std::string onnx_model_path = parser.get<std::string>("onnx_model_path");
    std::string mode = parser.get<std::string>("mode");
    int gpu_device_id = parser.get<int>("gpu_device_id");
    int batch_size = parser.get<int>("batch_size");
    int input_height = parser.get<int>("input_height");
    int input_width = parser.get<int>("input_width");
    int input_channel = parser.get<int>("input_channel");
    bool is_nchw = parser.get<bool>("is_nchw");
    bool use_normlize = parser.get<bool>("use_normlize");
    std::string input_data_type = parser.get<std::string>("input_data_type");
    std::string calibrator_image_dir = parser.get<std::string>("calibrator_image_dir");
    std::string calibrator_table_path = parser.get<std::string>("calibrator_table_path");
    IInt8EntropyCalibrator2 * calibrator = nullptr;
    std::string tensorrt_model_path = replace(onnx_model_path,".onnx",".trt");
    if(mode == "int8"){
        tensorrt_model_path = tensorrt_model_path+".int8";
        calibrator = new YOLOv5Int8EntropyCalibrator(batch_size,input_height,input_width,input_channel,is_nchw,
                                                     use_normlize,calibrator_image_dir,calibrator_table_path,input_data_type);
    }else if(mode == "fp16"){
        tensorrt_model_path = tensorrt_model_path+".fp16";
    } else{
        tensorrt_model_path = tensorrt_model_path+".fp32";
    }

    bool flag = false;
#if NV_TENSORRT_MAJOR < 8
    flag = onnx2tensorrt_v7(onnx_model_path,tensorrt_model_path,gpu_device_id);
#else
    flag = onnx2tensorrt_v8(onnx_model_path,tensorrt_model_path,gpu_device_id,mode,calibrator);
#endif
    return flag;
}

int main(int argc, char *argv[])
{
    // 初始化命令行解析器
    cmdline::parser a;
    a.add<std::string>("onnx_model_path", '\0', "onnx model path", false, "");
    a.add<std::string>("mode", '\0', "mode", false, "fp32");
    a.add<std::string>("calibrator_image_dir", '\0', "calibrator image  dir", false, "");
    a.add<std::string>("calibrator_table_path", '\0', "calibrator table path", false, "");
    a.add<int>("batch_size", '\0', "batch size", false, 1);
    a.add<int>("input_width", '\0', "input width", false, 0);
    a.add<int>("input_height", '\0', "input height", false, 0);
    a.add<int>("input_channel", '\0', "input channel", false, 3);
    a.add<bool>("is_nchw", '\0', "is nchw", false, 0);
    a.add<bool>("use_normlize", '\0', "use normlize", false, 0);
    a.add<int>("gpu_device_id", '\0', "gpu device id", false, 0);
    a.add<std::string>("input_data_type", '\0', "input data type", false, "float32");
    a.parse_check(argc, argv);

    // 根据onnx生成tensorrt模型
    onnx2tensorrt(a);

    return 0;
}

