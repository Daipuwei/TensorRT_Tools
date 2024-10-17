# 使用编译器
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(CMAKE_C_COMPILER /usr/bin/gcc)
set(CMAKE_CXX_COMPILER /usr/bin/g++)

# 设置cuda编译器路径，并启用cuda编程语言支持
set(CMAKE_CUDA_COMPILER /usr/local/cuda/bin/nvcc)

# 设置C++标准和调试模式
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_definitions(-std=c++14)
add_definitions(-DAPI_EXPORTS)

# 设置通用编译选项，加入dl和pthread库支持
set(COMMON_FLAGS "-ldl -lpthread")

# Debug编译配置
set(CMAKE_CXX_FLAGS_DEBUG "${COMMON_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -g")
set(CMAKE_C_FLAGS_DEBUG "${COMMON_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -g")

# Release编译配置，编译选项优化性能O0<O1<O2<O3<Ofast<Omax
set(CMAKE_CXX_FLAGS_RELEASE "${COMMON_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -Ofast")
set(CMAKE_C_FLAGS_RELEASE "${COMMON_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -Ofast")
