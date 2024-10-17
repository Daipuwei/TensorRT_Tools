# 设置cuda
set(CUDA_INCLUDE_PATH  /usr/local/cuda/include)
set(CUDA_LIB_PATH  /usr/local/cuda/lib64)
FILE(GLOB CUDA_LIBS ${CUDA_LIB_PATH}/*.so)
message(STATUS "CUDA library status:")
message(STATUS "    libraries: ${CUDA_LIBS}")
message(STATUS "    include path: ${CUDA_INCLUDE_PATH}")
include_directories(${CUDA_INCLUDE_PATH})
link_directories(${CUDA_LIB_PATH})

# 设置推理引擎SDK的so和头文件路径
set(ENGINE_API_PATH /opt/3rdparty/tensorrt)
set(ENGINE_API_INCLUDE ${ENGINE_API_PATH}/include)
FILE(GLOB ENGINE_API_LIBS ${ENGINE_API_PATH}/lib/*.so)
message(STATUS "TensorRT library status:")
message(STATUS "    libraries: ${ENGINE_API_LIBS}")
message(STATUS "    include path: ${ENGINE_API_INCLUDE}")
include_directories(${ENGINE_API_INCLUDE})
link_directories(${ENGINE_API_PATH}/lib/)

# 设置图像处理库的so和头文件路径
find_package(OpenCV REQUIRED PATHS /opt/3rdparty/opencv/ NO_DEFAULT_PATH)
include_directories(${OpenCV_INCLUDE_DIRS})
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

## 设置spdlog日志库so与头文件路径
set(SPDLOG_API_PATH /opt/3rdparty/spdlog)
set(SPDLOG_API_INCLUDE ${SPDLOG_API_PATH}/include)
set(SPDLOG_API_LIBS ${SPDLOG_API_PATH}/lib/libspdlog.a)
include_directories(${SPDLOG_API_INCLUDE})
message(STATUS "Spdlog library status:")
message(STATUS "    libraries: ${SPDLOG_API_LIBS}")
message(STATUS "    include path: ${SPDLOG_API_INCLUDE}")

# 设置第三方库so文件路径
list(APPEND THIRD_PARTY_LIBS ${SPDLOG_API_LIBS} ${OpenCV_LIBS} ${CUDA_LIBS}
        ${ENGINE_API_LIBS} nvinfer nvparsers nvonnxparser cudart nvinfer_plugin dl)
foreach(lib_file ${THIRD_PARTY_LIBS})
    message(STATUS "Found so file: ${lib_file}")
endforeach()
