//
// Created by dpw on 24-2-29.
//

#include "NvOnnxParser.h"
#include "spdlog/spdlog.h"

#include "iostream"
#include <ostream>
#include <fstream>
#include <iterator>

#include "common_utils.h"
#include "tensorrt_utils.h"
#include "opencv_image_utils.h"
#include "yolov5_trt_calibrator.h"

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
YOLOv5Int8EntropyCalibrator::YOLOv5Int8EntropyCalibrator(int batch_size, int input_height, int input_width, int input_channel,
                                                         bool is_nchw,bool use_normlize,std::string calibrator_image_dir,
                                                         std::string calibrator_table_path,std::string input_data_type){
    // 初始化相关参数
    this->batch_size = batch_size;
    this->input_height = input_height;
    this->input_width = input_width;
    this->input_channel = input_channel;
    this->is_nchw = is_nchw;
    this->use_normlize = use_normlize;
    this->input_size = this->input_height*this->input_width*this->input_channel;
    this->calibrator_image_dir = calibrator_image_dir;
    this->calibrator_table_path = calibrator_table_path;
    this->image_index = 0;
    // 初始化数据类型大小
    if(input_data_type == "float16"){
        this->input_data_type_size = 2;
    } else{
        this->input_data_type_size = 4;
    }

    // 若校准表不存在则初始化校准图像路径
    if(!is_file_exists(this->calibrator_table_path)){
        // 获取校准图像集中所有图像路径,并对齐batchsize
        get_file_paths(this->calibrator_image_dir,
                       this->calibrator_image_paths,
                       IMAGE_EXTS);
        this->calibrator_image_paths.resize(static_cast<int>(this->calibrator_image_paths.size() / this->batch_size) * this->batch_size);
        std::random_shuffle(this->calibrator_image_paths.begin(), this->calibrator_image_paths.end(),
                            [](int i) { return rand() % i; });
    }

    //初始化输入张量
    CUDA_CHECK(cudaMalloc(&this->input, this->batch_size*this->input_size*sizeof(float)));
}


/**
 * @brief 这是YOLOv5模型TensorRT校准类的析造函数
 */
YOLOv5Int8EntropyCalibrator::~YOLOv5Int8EntropyCalibrator() {
    // 释放资源
    std::vector<std::string>().swap(this->calibrator_image_paths);
    std::vector<char>().swap(calibrator_cache);
    CUDA_CHECK(cudaFree(this->input));
}

/**
 * @brief 这是获取获取小批量数据规模的函数
 * @return 小批量数据规模
 */
int YOLOv5Int8EntropyCalibrator::getBatchSize() const TRT_NOEXCEPT {
    return this->batch_size;
}


/**
 * @brief 这是YOLOv5模型图像预处理函数
 * @param image 图像
 * @return 预处理后的图像张量
 */
cv::Mat YOLOv5Int8EntropyCalibrator::preprocess(cv::Mat image) {
    int image_channel = image.channels();
    // 图像resize
    cv::Mat input_image;
    input_image = opencv_resize_image(image,this->input_width,this->input_height);
    // bgr2rgb
    cv::cvtColor(input_image, input_image, cv::COLOR_BGR2RGB);
//    // 图像归一化
//    input_image.convertTo(input_image, CV_32FC3,1./255);
//    // nhwc转nchw
//    std::vector<float> image_data = hwc2chw(input_image);
    // if(image_channel==3){
    //     input_image.convertTo(input_image, CV_32FC3);
    // } else{
    //     input_image.convertTo(input_image, CV_32FC1);
    // }

    // 图像归一化
    if(this->use_normlize) {
        // ONNX模型存在归一化操作，则不做opencv归一化操作
        if(image_channel==3){
            input_image.convertTo(input_image, CV_32FC3);
        } else{
            input_image.convertTo(input_image, CV_32FC1);
        }
    }else{
        // ONNX模型不存在归一化操作，则做opencv归一化操作
        if(image_channel==3){
            input_image.convertTo(input_image, CV_32FC3,1./255);
        } else{
            input_image.convertTo(input_image, CV_32FC1,1./255);
        }
    }

    // hwc2chw
    cv::Mat input_tensor;
    if(this->is_nchw) {         // nchw输入则需要做opencv的hwc2chw操作
        input_tensor = hwc2chw(input_image);
    }else {                     // nhwc输入则不需要做opencv的hwc2chw操作
        input_tensor = input_image;
    }
    return input_tensor;
}

/**
 * @brief 这是获取一个批次输入数据的函数
 * @param bindings
 * @param names
 * @param nbBindings
 * @return
 */
bool YOLOv5Int8EntropyCalibrator::getBatch(void **bindings,const char *names[], int nbBindings) TRT_NOEXCEPT {
    if (this->image_index + this->batch_size > this->calibrator_image_paths.size()) {
        spdlog::info("批量图像下标越界，image_index={}",this->image_index);
        return false;
    }
    
    // 遍历所有校准图片，做数据预处理
    float* input_tensor = new float[this->batch_size*this->input_size];
    memset(input_tensor,0,this->batch_size*this->input_size*sizeof(float));
    for(int i = 0 ; i < this->batch_size ; i++){
        // 加载图像并作图像预处理
        cv::Mat image = cv::imread(this->calibrator_image_paths[this->image_index]);
//        std::vector<float> image_data = this->preprocess(image);
//        // 图像预处理数据数组复制到tensorRT模型输入数组指定位置
//        memcpy(input_tensor+i*this->input_size,&image_data[0],image_data.size()*this->input_data_type_size);
        cv::Mat image_data = this->preprocess(image);
        // 图像预处理数据数组复制到tensorRT模型输入数组指定位置
        memcpy(input_tensor+i*this->input_size,image_data.data,this->input_size*this->input_data_type_size);
        spdlog::info("progress:[{}/{}]",
                      this->image_index + 1,this->calibrator_image_paths.size());
        this->image_index++;
    }
    cudaMemcpy(this->input, input_tensor, 
               this->batch_size * this->input_size *this->input_data_type_size, cudaMemcpyHostToDevice);
    bindings[0] = this->input;

    return true;
}

/**
 * @brief 这是读取校准量化表的函数
 * @param length 长度
 * @return
 */
const void *YOLOv5Int8EntropyCalibrator::readCalibrationCache(size_t &length) TRT_NOEXCEPT {
    void *output;
    this->calibrator_cache.clear();
    assert(!this->calibrator_table_path.empty());
    std::ifstream input(this->calibrator_table_path, std::ios::binary);
    input >> std::noskipws;
    //　读取量化表中的内容
    if (input.good()) {
        std::copy(std::istream_iterator<char>(input), std::istream_iterator<char>(),
                  std::back_inserter(this->calibrator_cache));
    }
    length = this->calibrator_cache.size();
    if (length){
        spdlog::info("Using cached calibration table to build the engine");
        output = &this->calibrator_cache[0];
    }else{
        spdlog::info("New calibration table will be created to build the engine");
        output = nullptr;
    }

    return output;
}

/**
 * @brief 这是写入校准量化表的函数
 * @param cache 缓存数组
 * @param length 缓存数组长度
 */
void YOLOv5Int8EntropyCalibrator::writeCalibrationCache(const void *cache, size_t length) TRT_NOEXCEPT {
    assert(!this->calibrator_table_path.empty());
    std::ofstream output(this->calibrator_table_path, std::ios::binary);
    output.write(reinterpret_cast<const char *>(cache), length);
    output.close();
}
