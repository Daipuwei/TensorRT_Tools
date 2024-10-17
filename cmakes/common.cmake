# 初始化公共头文件和源文件
set(COMMON_INCLUDE_PATHS ${SOURCE_DIR}/utils/include)
FILE(GLOB UTILS_CPP_PATHS ${SOURCE_DIR}/utils/src/*.cpp)
set(COMMON_CPP_PATHS ${UTILS_CPP_PATHS})

foreach(cpp_file ${COMMON_CPP_PATHS})
    message(STATUS "Found common cpp file: ${cpp_file}")
endforeach()

# 导入头文件
foreach(include_file ${COMMON_INCLUDE_PATHS})
    message(STATUS "Found common include directory: ${include_file}")
endforeach()
include_directories(${COMMON_INCLUDE_PATHS})