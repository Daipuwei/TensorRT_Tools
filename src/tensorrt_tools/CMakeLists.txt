cmake_minimum_required(VERSION 3.10)
project(ONNX2TensorRT)

# 设置头文件
include_directories(${COMMON_INCLUDE_PATHS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include/
)

# 设置TensorRT Tools相关头文件
FILE(GLOB TENSORRT_TOOLS_CPP_PATHS src/*.cpp)
# 使用 list(FILTER ...) 过滤出不包含 EXCLUDE_PATTERN 的文件
set(EXCLUDE_PATTERN "onnx2tensorrt")
list(FILTER TENSORRT_TOOLS_CPP_PATHS EXCLUDE REGEX ".*${EXCLUDE_PATTERN}.*")

# 设置检测可执行文件相关
add_executable(onnx2tensorrt
        src/onnx2tensorrt.cpp ${TENSORRT_TOOLS_CPP_PATHS} ${COMMON_CPP_PATHS})
target_link_libraries(onnx2tensorrt ${THIRD_PARTY_LIBS})

# 安装文件到指定目录
install(TARGETS onnx2tensorrt DESTINATION ${SDK_PATH}/bin)