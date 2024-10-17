//
// Created by dpw on 24-2-29.
//

#ifndef YOLO_DEPLOY_CXX_COMMON_UTILS_H
#define YOLO_DEPLOY_CXX_COMMON_UTILS_H

#include <vector>
#include "string"
#include <iostream>
#include <algorithm>

/**
 * @brief 这是从txt文件中读取标签名称数组的函数
 * @param txt_path txt文件路径
 * @return 标签名称数组
 */
std::vector<std::string> read_label_dict(std::string txt_path);

/**
 * @brief 这是判断字符串是否为文件夹的函数
 * @param path 文件夹路径
 * @return
 */
bool is_directory(const std::string& path);

/**
 * @brief 这是判断字符串是否为文件的函数
 * @param path 文件路径
 * @return
 */
bool is_file(const std::string& path);

/**
 * @brief 这是判断文件是否存在的函数
 * @param file_path 文件路径
 * @return 文件是否存在布尔量
 */
bool is_file_exists(const std::string &file_path);

/**
 * @brief 这是判断字符串是否是以指定后缀结尾
 * @param str 文件字符串
 * @param suffix 后缀字符串
 * @return True代表文件字符串是以指定字符串结尾,False则代表文件字符串不是以指定字符串结尾
 */
bool end_with(const std::string &str, const std::string &suffix);

/**
 * @brief 这是地址拼接的函数
 * @param path 地址
 * @param str 字符串
 * @return 拼接后的字符串
 */
std::string join_address(std::string path,std::string str);

/**
 * @brief 这是地址拼接的函数
 * @param path 地址
 * @param strs 字符串数组
 * @return 拼接后的字符串
 */
std::string join_address(std::string path,std::vector<std::string> strs);

/**
 * @brief 这是判断字符串中是否包含字符串的函数
 * @param str 字符串
 * @param sub_str 子字符串
 * @return 是否包含子字符串的布尔量
 */
bool is_contain_sub_string(std::string str,std::string sub_str);

/**
 * @brief 这是实现字符串中子字符串的替代
 * @param str 字符串
 * @param sub_str1 子字符串1
 * @param sub_str2 子字符串2
 * @return 替换后的字符串
 */
std::string replace(std::string str,std::string sub_str1,std::string sub_str2);

/**
 * @brief 这是根据分割符实现字符串分割的函数
 * @param str 字符串
 * @param delimiter 分割符
 * @return 分割后的字符串数组
 */
std::vector<std::string> split(std::string str, char delimiter);


/**
 * @brief 这是分割文件路径为父目录和文件名的函数
 * @param str 文件(夹)路径
 * @return 父目录和文件名的数组
 */
std::vector<std::string> split_filename(std::string file_path);

/**
 * @brief 这是对文件名划分后缀的函数
 * @param file_name 文件名字符串
 * @return 文件名称和后缀的数组
 */
std::vector<std::string> split_ext(std::string file_name);

/**
 * @brief 这是获取文件夹指定后缀的文件路径数组
 * @param dir 文件夹路径
 * @param file_paths 图像路径数组
 * @param ext_array 后缀数组
 * @param is_include_subdir 是否迭代搜索子目录,0代表迭代搜索子目录,1代表迭代搜索子目录
 * @return 指定后缀的文件路径数组
 */
void get_file_paths(std::string dir,std::vector<std::string>& file_paths,std::vector<std::string> ext_array);

/**
 * @brief 这是判断文件后缀是否在后缀数组中
 * @param file_path 文件路径
 * @param ext_array 后缀数组
 * @return 文件名是否有指定后缀标志位
 */
bool is_contain_ext(std::string file_path,std::vector<std::string> ext_array);

/**
 * @brief 这是判断字符串是否在后缀数组中
 * @param str 文件路径
 * @param str_array 后缀数组
 * @return 字符串是否在后缀数组标志位
 */
bool is_contain_str(std::string str,std::vector<std::string> str_array);

/**
 * @brief 这是设置debug级别日志的函数
 */
void enable_debug_msg();

/**
 * @brief 这是创建目录的函数
 * @param path 文件夹路径
 * @return 是否创建成功布尔量
 */
bool mkdir(std::string& path);

/**
 * 这是递归创建文件夹的函数
 * @param path 文件夹路径
 * @return 是否创建成功布尔量
 */
bool makedirs(std::string& path);

// argmax定义
template <class ForwardIterator>
inline static size_t argmax(ForwardIterator first, ForwardIterator last) {
    return std::distance(first, std::max_element(first, last));
}

/**
 * @brief 这是限制边界值的函数
 * @param val 候选值
 * @param min 最小值
 * @param max 最大值
 * @return
 */
float clamp(float val, int min, int max);

#endif //YOLO_DEPLOY_CXX_COMMON_UTILS_H
