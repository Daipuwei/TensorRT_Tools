#!/bin/bash
# 编译平台
supports="x86_linux_tensorrt"

# 初始化命令行参数
if (( $# == 0 )); then
  echo ""
  echo "Please Select the Compile Platform and options: build type"
  echo ""
  echo "-platform : platform which you use to compile."
  echo "support platform: ${supports}"
  echo ""
  echo "-build : build type, Debug or Release"
  echo ""
  echo "for example: $0 -platform x86_linux_tensorrt -build Debug"
  exit
fi

# 解析
i=1
for args in "$@"; do
  index=$((i+1))
  case "${args}" in
    -platform)
      if (( ${index} <= $# )); then
        PLATFRORM_TYPE=${!index}
        echo "Compile platform ${platform}"
      else
        echo "Please Select the Compile Platform!"
        echo "support platform: ${supports}"
        echo "for example: $0 -platform x86_tensorrt"
        exit
      fi
    ;;

    -build)
      if (( ${index} <= $# )); then
        if [ "${!index}" != "Debug" ] && [ "${!index}" != "Release" ]; then
          echo "Build Type Only Support Debug or Release"
          echo "for example: $0 -build Debug"
          exit
        else
          echo "Build Type ${!index}"
          COMPILE_BUILD_TYPE=${!index}
        fi
      else
        exit
      fi
    ;;
  esac
  i=$((i + 1))
done

# 初始化相关路径
u=`whoami`
BASE=$(cd $(dirname ${0})>/dev/null;pwd)
CURRENT_TIME=$(date +%Y%m%d%H%M%S)
SDK_PATH=${BASE}/build/TensorRT_Tools_${COMPILE_BUILD_TYPE}_${PLATFRORM_TYPE}_${CURRENT_TIME}
SDK_ZIP_PATH=${BASE}/build/TensorRT_Tools_${COMPILE_BUILD_TYPE}_${PLATFRORM_TYPE}_${CURRENT_TIME}.zip

# 创建SDK文件夹及其子目录
mkdir -p ${SDK_PATH}/bin/
chmod 777 -R ${SDK_PATH}

# 初始化交叉编译链cmake
TOOLCHAIN_PATH=${BASE}/cmakes/platform/${PLATFRORM_TYPE}/toolchain.cmake

# 创建搭建目录
if [ ! -d "./build/build_${PLATFORM_TYPE}" ];then
  mkdir -p ./build/build_${PLATFRORM_TYPE}
fi
chmod 777 -R ./build/
cd ./build/build_${PLATFRORM_TYPE}

cmake -DCMAKE_BUILD_TYPE=${COMPILE_BUILD_TYPE} \
      -DSDK_PATH=${SDK_PATH} \
      -DPLATFORM_TYPE=${PLATFRORM_TYPE} \
      -DCMAKE_TOOLCHAIN_FILE=${TOOLCHAIN_PATH} ../..
#make
make -j$(($(nproc)/4))
make install

# SDK打包
cd ../../
chmod 777 -R ${SDK_PATH}
7z a ${SDK_ZIP_PATH} ${SDK_PATH}
chmod 777 ${SDK_ZIP_PATH}
