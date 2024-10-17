//
// Created by dpw on 24-2-29.
//

#ifndef YOLO_DEPLOY_YOLOV5_TRT_CALIBRATOR_H
#define YOLO_DEPLOY_YOLOV5_TRT_CALIBRATOR_H

#include "string"
#include "vector"
#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

#include "NvInfer.h"
#include "NvInferRuntimeCommon.h"

#include "tensorrt_utils.h"

using namespace nvinfer1;

const std::vector<std::string> IMAGE_EXTS = {".bmp", ".dng", ".jpeg", ".jpg", ".mpo", ".png", ".tif", ".tiff", ".webp"};

class YOLOv5Int8EntropyCalibrator:public nvinfer1::IInt8EntropyCalibrator2 {
public:
    /**
     * @brief 这是YOLOv5模型TensorRT校准类的构造函数
     * @param batch_size 小批量数据规模
     * @param input_height 输入高度
     * @param input_width 输入宽度
     * @param input_channel 输入通道数
     * @param input_name 输入节点名称
     * @param is_nchw onnx模型输入节点是否为NCHW格式
     * @param use_normlize onnx模型是否使用数据归一化
     * @param calibrator_image_dir 校准图像集目录地址
     * @param calibrator_table_path 校准量化表路径
     * @param input_data_type 模型输入节点数据类型
     */
    YOLOv5Int8EntropyCalibrator(int batch_size,int input_height,int input_width,int input_channel,bool is_nchw,
                                bool use_normlize,std::string calibrator_image_dir,
                                std::string calibrator_table_path,std::string input_data_type);

    /**
     * @brief 这是YOLOv5模型TensorRT校准类的析构函数
     */
    ~YOLOv5Int8EntropyCalibrator();

    void set_input_data_type(DataType input_data_type);

    /**
     * @brief 这是获取获取小批量数据规模的函数
     * @return 小批量数据规模
     */
    int getBatchSize() const TRT_NOEXCEPT override;

    /**
     * @brief 这是获取一个批次输入数据的函数
     * @param bindings
     * @param names
     * @param nbBindings
     * @return
     */
    bool getBatch(void* bindings[], const char* names[], int nbBindings) TRT_NOEXCEPT override;

    /**
     * @brief 这是读取校准量化表的函数
     * @param length 长度
     * @return
     */
    const void* readCalibrationCache(size_t& length) TRT_NOEXCEPT override;

    /**
     * @brief 这是写入校准量化表的函数
     * @param cache 缓存数组
     * @param length 缓存数组长度
     */
    void writeCalibrationCache(const void* cache, size_t length) TRT_NOEXCEPT override;

    /**
     * @brief 这是YOLOv5模型图像预处理函数
     * @param image 图像
     * @return 预处理后的图像张量
     */
    cv::Mat preprocess(cv::Mat image);

private:
    int batch_size;                                         // 小批量数据规模
    int input_width;                                        // 模型输入宽度
    int input_height;                                       // 模型输入高度
    int input_channel;                                      // 模型输入通道数
    bool is_nchw;                                           // 模型输入是否为NCHW格式
    bool use_normlize;                                      // 模型使用使用归一化操作
    int input_size;                                         // 输入节点大小
    int image_index;                                        // 图像索引
    std::string input_name;                                 // 输入节点名称
    std::string calibrator_image_dir;                       // 校准图像集目录地址
    std::vector<std::string> calibrator_image_paths;        // 校准图像路径数组
    std::string calibrator_table_path;                      // 校准表路径
    void* input;                                            // 输入数组指针
    std::vector<char> calibrator_cache;                     // 校准缓存数组
    int input_data_type_size;                               // 输入节点类型大小
};


#endif //YOLO_DEPLOY_YOLOV5_TRT_CALIBRATOR_H
