//
// Created by root on 24-10-17.
//

#ifndef TENSORRT_UTILS_H
#define TENSORRT_UTILS_H

#include "string"
#include "vector"
#include <numeric>
#include "NvInfer.h"
#include "NvInferPlugin.h"
#include "device_launch_parameters.h"
#include "NvInferRuntimeCommon.h"
#include "NvInferVersion.h"
#include <cuda_runtime_api.h>

using namespace nvinfer1;

#ifndef CUDA_CHECK
#define CUDA_CHECK(callstr)\
{\
cudaError_t error_code = callstr;\
if (error_code != cudaSuccess) { \
std::cerr << "CUDA error " << error_code << " at " << __FILE__ << ":" << __LINE__;\
assert(0);\
}\
}
#endif  // CUDA_CHECK

#if NV_TENSORRT_MAJOR >= 8
#define TRT_NOEXCEPT noexcept
#define TRT_CONST_ENQUEUE const
#else
#define TRT_NOEXCEPT
#define TRT_CONST_ENQUEUE
#endif

#endif //TENSORRT_UTILS_H
