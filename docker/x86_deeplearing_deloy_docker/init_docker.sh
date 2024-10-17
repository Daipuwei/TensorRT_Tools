#!/bin/bash
# 初始化参数
PORT=$1
DOCKER_CONTAINER_NAME=$2
DOCKER_IMAGE_NAME=$3
START_CPU_ID=$4
END_CPU_ID=$5

# 定义初始化Docker容器的函数
init_docker_container() {
   local port=$1
   local docker_container_name=$2
   local docker_image_name=$3
   local start_cpu_id=$4
   local end_cpu_id=$5
   
   # 删除指定名称容器
   delete_container ${DOCKER_CONTAINER_NAME}
	
   # 获取 CPU ID 字符串
   cpu_ids=$(get_cpu_id_string "$start_cpu_id" "$end_cpu_id")
   echo "使用的 CPU 核心 ID 为: $cpu_ids"
   
   # 启动容器
   xhost +
   docker run -itd \
           --ipc=host \
           --restart=always \
           --privileged=true \
           --ulimit memlock=-1 \
           --ulimit stack=67108864 \
           --shm-size=32g \
           -p "$port:22" \
           --cpuset-cpus=$cpu_ids \
           --gpus all \
           --name $docker_container_name \
           -e LANG=zh_CN.UTF-8 \
           -e TZ="Asia/Shanghai" \
           -e DISPLAY=${DISPLAY} \
           -e NVIDIA_DRIVER_CAPABILITIES=video,compute,utility \
           -e NVIDIA_VISIBLE_DEVICES=all \
           -e GDK_SCALE \
           -e GDK_DPI_SCALE \
           -v /etc/localtime:/etc/localtime:ro \
           -v /tmp/.X11-unix:/tmp/.X11-unix \
           -v /dev/mem:/dev/mem \
           -v /store1:/store1 \
           -v /store2:/store2 \
           -v /home:/home \
           -v /etc/locale.conf:/etc/locale.conf \
           $docker_image_name

   # 暴露端口
   expose_container_port $docker_container_name $port
}

# 获取 CPU ID 字符串数组
get_cpu_id_string() {
   local start_cpu_id=$1
   local end_cpu_id=$2
   local cpu_id_str=""
   
   for ((i=start_cpu_id; i<=end_cpu_id; i++)); do
       if [ -n "$cpu_id_str" ]; then
           cpu_id_str="$cpu_id_str,$i"
       else
           cpu_id_str="$i"
       fi
   done

   # 输出结果
   echo "$cpu_id_str"
}

# 定义删除指定名称容器的函数
delete_container() {
   local container_name=$1
   # 获取容器id
   container_id=`docker ps | grep $container_name | awk '{print $1}'`
   if [ -z "$container_id" ]; then
       echo "没有找到容器名称为 $container_name 的容器"
   else
       echo "容器名称为 $container_name 的容器 ID 是 $container_id"
       echo "删除容器名称为 $container_name 的容器"
       docker stop $container_id
       docker rm $container_id
   fi
}


# 定义在每个容器中ssh配置文件中暴露端口的函数
expose_container_port() {
   local container_name=$1
   local port=$2
   # 获取容器id
   container_id=`docker ps | grep $container_name | awk '{print $1}'`
   echo "container id : $container_id"

   # 将port加入ssh配置文件
   docker exec -it $container_id /bin/bash sudo echo 'Port $port' >> /etc/ssh/sshd_config
   docker exec -it $container_id /bin/bash service ssh restart
}

# 初始化Docker镜像
init_docker_container ${PORT} ${DOCKER_CONTAINER_NAME} ${DOCKER_IMAGE_NAME} ${START_CPU_ID} ${END_CPU_ID}
