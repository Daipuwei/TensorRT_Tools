//
// Created by dpw on 24-2-29.
//

#include "stack"
#include <sys/stat.h>
#include <dirent.h>
#include "sstream"
#include "iostream"
#include <ostream>
#include <fstream>
#include "vector"
#include "spdlog/spdlog.h"

#include "common_utils.h"
#if _WIN64
char sep = '\\';
#pragma comment(lib, "Shlwapi.lib")
#else
char sep = '/';
#endif

/**
 * @brief 这是从txt文件中读取标签名称数组的函数
 * @param txt_path txt文件路径
 * @return 标签名称数组
 */
std::vector<std::string> read_label_dict(std::string txt_path)
{
    std::vector <std::string> label_list;
    std::ifstream in(txt_path);
    std::string line;
    if (in) {
        while (getline(in, line)) {
            label_list.emplace_back(line);
        }
    } else {
        std::cout << "no such label txt file: " << txt_path << ", exit the program..."
                  << std::endl;
        exit(1);
    }
    return label_list;
}

/**
 * @brief 这是判断字符串是否为文件的函数
 * @param path 文件路径
 * @return
 */
bool is_file(const std::string& path)
{
    struct stat s;
    if (stat(path.c_str(), &s) == 0) {
        return S_ISREG(s.st_mode);
    }
    return false;
}

/**
 * @brief 这是判断子字符串是否为文件夹的函数
 * @param path 文件夹路径
 * @return
 */
bool is_directory(const std::string& path)
{
    struct stat s;
    if (stat(path.c_str(), &s) == 0) {
        return S_ISDIR(s.st_mode);
    }
    return false;
}

/**
 * @brief 这是地址拼接的函数
 * @param path 文件夹地址
 * @param str 字符串
 * @return 拼接后的字符串
 */
std::string join_address(std::string path,std::string str)
{
    std::string _sep(1,sep);
    if(end_with(path,_sep)){
        int size = path.length();
        path = path.substr(0,size-1);
    }
    std::string new_path = path+_sep+str;
    return new_path;
}

/**
 * @brief 这是地址拼接的函数
 * @param path 地址
 * @param strs 字符串数组
 * @return 拼接后的字符串
 */
std::string join_address(std::string path,std::vector<std::string> strs)
{
    std::string _sep(1,sep);
    if(end_with(path,_sep)){
        int size = path.length();
        path = path.substr(0,size-1);
    }
    std::string new_path = path;
    for(int i = 0 ; i < strs.size() ; i++){
        //new_path = join_address(new_path,strs[i]);
        new_path = new_path+_sep+strs[i];
    }
    return new_path;
}

/**
 * @brief 这是判断文件是否存在的函数
 * @param file_path 文件路径
 * @return 文件是否存在布尔量
 */
bool is_file_exists(const std::string &file_path)
{
    std::ifstream f(file_path.c_str());
    return f.good();        // true代表文件存在，false代表文件不存在
}

/**
 * @brief 这是判断字符串是否是以指定后缀结尾
 * @param str 文件字符串
 * @param suffix 后缀字符串
 * @return True代表文件字符串是以指定字符串结尾,False则代表文件字符串不是以指定字符串结尾
 */
bool end_with(const std::string &str, const std::string &suffix)
{
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

/**
 * @brief 这是判断字符串中是否包含字符串的函数
 * @param str 字符串
 * @param sub_str 子字符串
 * @return 是否包含子字符串的布尔量
 */
bool is_contain_sub_string(std::string str,std::string sub_str) {
    int pos = str.find(sub_str);  // 查找sub_str1字符串位置
    bool flag = false;
    if (pos != std::string::npos){
        flag = true;
    }
    return flag;
}

/**
 * @brief 这是实现字符串中子字符串的替代
 * @param str 字符串
 * @param sub_str1 子字符串1
 * @param sub_str2 子字符串2
 * @return 替换后的字符串
 */
std::string replace(std::string str,std::string sub_str1,std::string sub_str2)
{
    std::string new_str = str;
    int pos = new_str.find(sub_str1);  // 查找sub_str1字符串位置
    if (pos != std::string::npos) {   // 如果找到了sub_str1字符串
        new_str.replace(pos, sub_str1.length(), sub_str2);  // 替换sub_str1为sub_str2
    }
    return new_str;
}

/**
 * @brief 这是根据分割符实现字符串分割的函数
 * @param str 字符串
 * @param delimiter 分割符
 * @return 分割后的字符串数组
 */
std::vector<std::string> split(std::string str, char delimiter)
{
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

/**
 * @brief 这是分割文件路径为父目录和文件名的函数
 * @param str 文件(夹)路径
 * @return 父目录和文件名的数组
 */
std::vector<std::string> split_filename(std::string file_path)
{
    // 根据文件路径分割符进行分割文件路径
    std::vector<std::string> tokens = split(file_path,sep);
    // 还原父目录和文件名
    int size = tokens.size();
    std::string dir = "";
    std::string file_name = tokens[size-1];
    for(int i = 0; i < size-1; i++){
        dir.append(tokens[i].c_str());
        dir.append(&sep);
    }
    std::vector<std::string> res = {dir,file_name};
    return res;
}

/**
 * @brief 这是对文件名划分后缀的函数
 * @param file_name 文件名字符串
 * @return 文件名称和后缀的数组
 */
std::vector<std::string> split_ext(std::string file_name)
{
    // 划分文件名称和后缀
    std::vector<std::string> res = split(file_name,'.');
    res[1] = "."+res[1];
    return res;
}

/**
 * @brief 这是获取文件夹指定后缀的文件路径数组
 * @param dir 文件夹路径
 * @param file_paths 图像路径数组
 * @param ext_array 后缀数组
 * @return 指定后缀的文件路径数组
 */
void get_file_paths(std::string dir,std::vector<std::string>& file_paths,std::vector<std::string> ext_array)
{

    std::stack<std::string> subdirs;
    subdirs.push(dir);

    while (!subdirs.empty()){
        // 获取堆栈栈顶元素
        std::string current_dir = subdirs.top();
        subdirs.pop();

        // 打开文件夹
        DIR *dirp = opendir(current_dir.c_str());
        if (!dirp) {
            continue;
        }

        // 迭代获取文件中
        struct dirent *dp;
        while ((dp = readdir(dirp)) != NULL) {
            // 获取子文件(夹)名称
            std::string child_name(dp->d_name);
            if (child_name == "." || child_name == "..") {
                continue;
            }
            // 初始化子文件(夹)路径
            std::string child_path = join_address(current_dir, child_name);
            struct stat statbuf;
            if (stat(child_path.c_str(), &statbuf) == 0) {
                if (S_ISDIR(statbuf.st_mode)) {
                    subdirs.push(child_path);
                } else if (S_ISREG(statbuf.st_mode)) {
                    for (auto &ext: ext_array) {
                        if (is_contain_sub_string(child_path, dir)) {
                            if (end_with(child_path, ext)) {
                                file_paths.emplace_back(child_path);
                            }
                        }
                    }
                }
            }
        }
        closedir(dirp);
    }
}

/**
 * @brief 这是判断文件后缀是否在后缀数组中
 * @param file_path 文件路径
 * @param ext_array 后缀数组
 * @return 文件名是否有指定后缀标志位
 */
bool is_contain_ext(std::string file_path,std::vector<std::string> ext_array)
{
    bool flag = false;
    std::vector<std::string> dir_video_name = split_filename(file_path);
    std::vector<std::string> fname_ext = split_ext(dir_video_name[1]);
    for(int i = 0 ; i < ext_array.size() ; i++){
        if(fname_ext[1] == ext_array[i]){
            flag = true;
            break;
        }
    }
    return flag;
}

/**
 * @brief 这是判断字符串是否在后缀数组中
 * @param str 文件路径
 * @param str_array 后缀数组
 * @return 字符串是否在后缀数组标志位
 */
bool is_contain_str(std::string str,std::vector<std::string> str_array)
{
    bool flag = false;
    for(int i = 0 ; i < str_array.size() ; i++){
        if(str == str_array[i]){
            flag = true;
            break;
        }
    }
    return flag;
}

/**
 * @brief 这是设置debug级别日志的函数
 */
void enable_debug_msg()
{
    spdlog::set_level(spdlog::level::debug);
}

/**
 * @brief 这是创建目录的函数
 * @param path 文件夹路径
 * @return 是否创建成功布尔量
 */
bool mkdir(std::string& path)
{
    // 使用mkdir函数创建文件夹
    int result = mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (result == 0) {
//        std::cout << "Folder created: " << path << std::endl;
        return true;
    } else if (errno == EEXIST) {
        // 如果文件夹已存在，则不需要创建
        return true;
    } else {
        std::cerr << "Error: Failed to create folder " << path << " - " << strerror(errno) << std::endl;
        return false;
    }
}

/**
 * 这是递归创建文件夹的函数
 * @param path 文件夹路径
 * @return 是否创建成功布尔量
 */
bool makedirs(std::string& path){
//    // 不是系统分隔符结尾的加上系统分隔符
//    std::string _sep(1,sep);
//    if(!end_with(path,_sep)){
//        path = path+_sep;
//    }

    // 检查文件夹是否已存在
    if (mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0) {
        return true;
    } else {
        // 递归创建上级文件夹
        size_t pos = path.find_last_of('/');
        if (pos != std::string::npos) {
            std::string parent = path.substr(0, pos);
            if (makedirs(parent)) {
                // 如果上级文件夹创建成功，则创建当前文件夹
                return mkdir(path);
            } else {
                return false;
            }
        } else {
            // 已经到达根目录，无法继续递归
            return false;
        }
    }
}

/**
 * @brief 这是限制边界值的函数
 * @param val 候选值
 * @param min 最小值
 * @param max 最大值
 * @return
 */
float clamp(float val, int min, int max) {
    return val > min ? (val < max ? val : (float)max) : (float)min;
}