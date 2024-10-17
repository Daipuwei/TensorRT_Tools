cmake_minimum_required(VERSION 3.10)
project(TensorRT_Tools)

# 设置cmakelist输入参数
set(SDK_PATH ${SDK_PATH})                          # SDK路径
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)    # 源代码路径

# 设定PLATFORM_TYPE默认值
if(NOT DEFINED PLATFORM_TYPE)
    SET(PLATFORM_TYPE x86_linux_tensorrt)
endif()
message(STATUS "PLATFORM_TYPE:${PLATFORM_TYPE}")

# 设置代码内部平台类型宏定义
if("${PLATFORM_TYPE}" STREQUAL "x86_linux_tensorrt")
    add_definitions(-DPLATFORM_TYPE=0)
endif ()

# 加载第三方依赖库
include(${CMAKE_CURRENT_SOURCE_DIR}/cmakes/platform/${PLATFORM_TYPE}/depend.cmake)

# 加载项目相关公共头文件和源文件
include(cmakes/common.cmake)

# 设置二级子项目CMakeLists.txt
add_subdirectory(${SOURCE_DIR}/tensorrt_tools TensorRT_Tools)