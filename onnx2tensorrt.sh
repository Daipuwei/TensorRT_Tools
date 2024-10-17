# yolov5n fp32
./bin/onnx2tensorrt --onnx_model_path ./model_data/yolov5n.onnx \
                    --mode fp32 \
                    --batch_size 1 \
                    --input_width 640 \
                    --input_height 640 \
                    --input_channel 3 \
                    --gpu_device_id 0

# yolov5n fp16
./bin/onnx2tensorrt --onnx_model_path /home/dpw/dpw/code/YOLO_Deploy_CXX/model_data/yolov5n_div255_nhwc.onnx \
                    --mode fp16 \
                    --batch_size 1 \
                    --input_width 640 \
                    --input_height 640 \
                    --input_channel 3 \
                    --is_nchw 0 \
                    --use_normlize 1 \
                    --gpu_device_id 0

./bin/onnx2tensorrt --onnx_model_path ./model_data/yolov5_lite_c_0.5_rknn_relu_v2_vot_20240613+vot_coco2017_20240613_nchw_div255.onnx \
                    --mode fp16 \
                    --batch_size 1 \
                    --input_width 640 \
                    --input_height 640 \
                    --input_channel 3 \
                    --gpu_device_id 0

./bin/onnx2tensorrt --onnx_model_path ./model_data/yolov5_lite_c_0.5_rknn_relu_v2_vot_20240613+vot_coco2017_20240613_nhwc_div255.onnx \
                    --mode fp16 \
                    --batch_size 1 \
                    --input_width 640 \
                    --input_height 640 \
                    --input_channel 3 \
                    --gpu_device_id 0

./bin/onnx2tensorrt --onnx_model_path ./model_data/yolov5n_nhwc.onnx \
                    --mode fp16 \
                    --batch_size 1 \
                    --input_width 640 \
                    --input_height 640 \
                    --input_channel 3 \
                    --gpu_device_id 0

./bin/onnx2tensorrt --onnx_model_path ./model_data/yolov5n_nhwc_div_255.onnx \
                    --mode fp16 \
                    --batch_size 1 \
                    --input_width 640 \
                    --input_height 640 \
                    --input_channel 3 \
                    --gpu_device_id 0

# yolov5n int8
./bin/onnx2tensorrt --onnx_model_path ./model_data/yolov5n.onnx \
                    --mode int8 \
                    --batch_size 1 \
                    --input_width 640 \
                    --input_height 640 \
                    --input_channel 3 \
                    --calibrator_image_dir /home/dpw/daipuwei/coco_calib \
                    --calibrator_table_path  ./model_data/yolov5n_coco2017_calibration.cache \
                    --gpu_device_id 0
