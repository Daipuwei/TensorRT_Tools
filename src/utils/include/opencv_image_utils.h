//
// Created by dpw on 24-2-29.
//

#ifndef YOLO_DEPLOY_CXX_OPENCV_IMAGE_UTILS_H
#define YOLO_DEPLOY_CXX_OPENCV_IMAGE_UTILS_H

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

/***
 * @brief 这是opencv完成图像缩放的函数
 * @param image 输入图像
 * @param tinput_width 缩放图像宽度
 * @param input_height 缩放图像高度
 * @return
 */
cv::Mat opencv_resize_image(cv::Mat image,int input_width, int input_height);

/**
 * @brief 这是图像标准化的函数
 * @param image 图像
 * @param mean 通道均值数组
 * @param std 通道标准差数组
 * @return 标准化后的图像
 */
cv::Mat image_normlize(cv::Mat image,std::vector<float> mean,std::vector<float> std);

/**
 * @brief 这是hwc格式的cv::Mat转chw格式vector的函数
 * @param image 图像
 * @return chw图像数组
 */
cv::Mat hwc2chw(cv::Mat image);

///**
// * @brief 这是hwc格式的cv::Mat转chw格式vector的函数
// * @param image 图像
// * @return chw图像数组
// */
//std::vector<float> hwc2chw(cv::Mat image);

/**
 * @brief 这是将cv::Mat转化成vector的函数
 * @param image 输入图像
 * @return vector图像数组
 */
std::vector<float> mat2vector(cv::Mat image);

/**
 * @brief 这是hsv转rgb的函数函数
 * @param h hsv颜色空间h分量,范围0-1
 * @param s hsv颜色空间s分量,范围0-1
 * @param v hsv颜色空间v分量,范围0-1
 * @return rbg颜色数组
 */
cv::Scalar hsv2rbg(float h, float s, float v);

/**
 * @brief 这是根据颜色种类随机生成RGB颜色数组的函数
 * @param num_colors 颜色数量
 * @return RGB颜色数组
 */
std::vector<cv::Scalar> random_generate_colors(int num_colors);

/**
 * @brief 这是加载图像的函数
 * @param image_path 图像文件路径
 * @return 图像矩阵
 */
cv::Mat load_image(std::string image_path);

#endif //YOLO_DEPLOY_CXX_OPENCV_IMAGE_UTILS_H
