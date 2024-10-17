//
// Created by dpw on 24-2-29.
//

#include "fstream"
#include "spdlog/spdlog.h"

#include "algorithm"
#include "common_utils.h"
#include "opencv_image_utils.h"

/***
 * @brief 这是opencv完成图像缩放的函数
 * @param image 输入图像
 * @param tinput_width 缩放图像宽度
 * @param input_height 缩放图像高度
 * @return
 */
cv::Mat opencv_resize_image(cv::Mat image,int input_width, int input_height) {
    // 初始化原始图像宽高
    int image_height = image.rows;
    int image_width = image.cols;

    // 计算宽高的缩放比，选择按照那个方向尽享缩放
    double scale_h = input_height * 1. / image_height;
    double scale_w = input_width * 1. / image_width;
    double scale = std::min(scale_h,scale_w);
    int new_h = int(image_height * scale);
    int new_w = int(image_width * scale);

    // 缩放图像
    cv::Mat resize_image;
    if (scale >= 1) {
        resize_image = image.clone();
        new_h = image_height;
        new_w = image_width;
    } else{
        cv::resize(image, resize_image,
                   cv::Size(new_w, new_h), 0, 0, cv::INTER_NEAREST);
    }

    // 将reszie后图像贴到全128图像上的左上角
    cv::Mat input_image = cv::Mat::zeros(input_height, input_width,CV_32FC3);
    input_image = input_image + cv::Scalar(128, 128, 128);
    cv::Rect roi(0, 0, resize_image.cols, resize_image.rows);
    resize_image.copyTo(input_image(roi));
    return input_image;
}

/**
 * @brief 这是图像标准化的函数
 * @param image 图像
 * @param mean 通道均值数组
 * @param std 通道标准差数组
 * @return 标准化后的图像
 */
cv::Mat image_normlize(cv::Mat image,std::vector<float> mean,std::vector<float> std) {
    std::vector<cv::Mat> bgrChannels(3);
    // 图像划分为BGR通道向量
    cv::split(image, bgrChannels);
    // 遍历每个通道，对每个通道向数组进行处理
    for (auto i = 0; i < bgrChannels.size(); i++){
        bgrChannels[i].convertTo(bgrChannels[i], CV_32FC1, 1.0 / std[i], (0.0 - mean[i]) / std[i]);
    }
    cv::Mat dst;
    cv::merge(bgrChannels, dst);
    // 释放资源
    std::vector<cv::Mat>().swap(bgrChannels);
    return dst;
}

///**
// * @brief 这是hwc合适的cv::Mat转chw格式vector的函数
// * @param image 图像
// * @return chw图像数组
// */
//std::vector<float> hwc2chw(cv::Mat image) {
//    std::vector<float> dst_data;
//    std::vector<cv::Mat> bgrChannels(3);        // BGR通道图像每个通道数组
//    cv::split(image, bgrChannels);
//    // 遍历每个通道，将每个通道的像素值加入float向量中，实现hwc到chw的转换
//    for(auto i = 0; i < bgrChannels.size(); i++) {
//        std::vector<float> data = std::vector<float>(bgrChannels[i].reshape(1, 1));   // cv::Mat数组转换float数组
//        dst_data.insert(dst_data.end(), data.begin(), data.end());
//        std::vector<float>().swap(data);        // 释放资源
//    }
//    std::vector<cv::Mat>().swap(bgrChannels);      // 释放资源
//    return dst_data;
//}

/**
 * @brief 这是hwc格式的cv::Mat转chw格式vector的函数
 * @param hwc_image HWC格式图像
 * @return CHW格式的图像
 */
cv::Mat hwc2chw(cv::Mat hwc_image) {
    int height = hwc_image.rows;
    int width = hwc_image.cols;
    int channels = hwc_image.channels();

    // 拆分HWC图像的通道
    std::vector<cv::Mat> split_channels(channels);
    for (int c = 0; c < channels; ++c) {
        split_channels[c] = cv::Mat(height, width, CV_32FC1);
    }
    cv::split(hwc_image, split_channels);

    // 按顺序拼接通道（C, H, W）
    cv::Mat chw_image(channels, height*width, CV_32FC1);
    for (int c = 0; c < channels; ++c) {
        split_channels[c].reshape(1,1).copyTo(chw_image.row(c));
    }
    return chw_image;
}

/**
 * @brief 这是将cv::Mat转化成vector的函数
 * @param image 输入图像
 * @return vector图像数组
 */
std::vector<float> mat2vector(cv::Mat image) {
    std::vector<float> dst_data;
    dst_data = image.reshape(1,1);
    return dst_data;
}


/**
 * @brief 这是hsv转rgb的函数函数
 * @param h hsv颜色空间h分量,范围0-1
 * @param s hsv颜色空间s分量,范围0-1
 * @param v hsv颜色空间v分量,范围0-1
 * @return rbg颜色数组
 */
cv::Scalar hsv2rbg(float h, float s, float v) {
    const int h_i = static_cast<int>(h * 6);
    const float f = (h * 6.0) - h_i;
    const float p = v * (1.0 - s);
    const float q = v * (1.0 - f * s);
    const float t = v * (1.0 - (1.0 - f) * s);
    double r,g,b;
    switch(h_i){
        case 0:
            r = v; g = t; b = p;
            break;
        case 1:
            r = q; g = v; b = p;
            break;
        case 2:
            r = p; g = v; b = t;
            break;
        case 3:
            r = p; g = q; b = v;
            break;
        case 4:
            r = t; g = p; b = v;
            break;
        case 5:
            r = v; g = p; b = q;
            break;
        default:
            r = 1; g = 1; b = 1;
            break;
    }
//    std::cout <<int(b*255)<<" "<<int(g*255)<<" "<<int(r*255)<<std::endl;
    cv::Scalar color = cv::Scalar(b*255, g*255, r*255);
    return color;
}

/**
 * @brief 这是根据颜色种类随机生成RGB颜色数组的函数
 * @param num_colors 颜色数量
 * @return RGB颜色数组
 */
std::vector<cv::Scalar> random_generate_colors(int num_colors) {
    std::vector<cv::Scalar> colors;
    cv::RNG rng(12345);             // 随机种子
    const float golden_ratio_conjugate = 0.618033988749895;         // 黄金切割率

    const float s = 1.0;
    const float v = 1.0;
    for (int i = 0; i < num_colors; i++){
        //始终返回（0，1）区间的小数，取余fmod(a,b)=a-n*b(n为最大整除得到的整数商)商的符号取决于a
        const float h = i*1.0 / num_colors;
        colors.emplace_back(hsv2rbg(h, s, v));
    }
    return colors;
}


/**
 * @brief 这是加载图像的函数
 * @param image_path 图像文件路径
 * @return 图像矩阵
 */
cv::Mat load_image(std::string image_path) {
    cv::Mat image;
    std::ifstream file(image_path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        spdlog::error("Failed to open image file: {}",image_path);
        return image;
    }
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> buffer(size);
    if (!file.read(buffer.data(), size)) {
        spdlog::error("Failed to read image data from file: {}",image_path);
        return image;
    }
    file.close();
    // 将图像数据转换为 OpenCV Mat 格式
    image = cv::imdecode(cv::Mat(buffer), cv::IMREAD_COLOR);
    return image;
}
