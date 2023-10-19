#!/usr/bin/env bash

export CMAKE_PREFIX_PATH=/usr/local/lib/cmake/:$CMAKE_PREFIX_PATH
# export LD_LIBRARY_PATH=/usr/local/lib/:$LD_LIBRARY_PATH

catkin build \
    kitty-project \
    rtabmap \
    rtabmap_ros \
    spatio_temporal_voxel_layer \
    cv_bridge \
    maddrive_ros_shared \
    realsense2_camera \
    -j$(($(nproc)-2)) \
    --cmake-args \
    -DCATKIN_ENABLE_TESTING=False \
    -DCMAKE_BUILD_TYPE=Release \
    -DOpenCV_DIR="/usr/local/lib/cmake/opencv4"
##TODO - OPENCV_DIR replace like jetson nano
    
    
 
