#!/usr/bin/env bash

export CMAKE_PREFIX_PATH=/usr/local/lib/cmake/:$CMAKE_PREFIX_PATH
# export LD_LIBRARY_PATH=/usr/local/lib/:$LD_LIBRARY_PATH

catkin build \
    kitty-project \
    four_wheel_steering_controller \
    swerve_steering_controller \
    four_wheel_steering_msgs \
    urdf_geometry_parser \
    geometry2 \
    hector_slam \
    hector_gazebo \
    cv_bridge \
    maddrive_ros_shared \
    rtabmap_ros \
    ydlidar_ros_driver \
    realsense2_camera \
    realsense2_description \
    -j$(($(nproc)-2)) \
    --cmake-args \
    -D OpenCV_DIR="/usr/local/lib/cmake/opencv4" \
    -D RTABMAP_SYNC_MULTI_RGBD=ON \
    -D CATKIN_ENABLE_TESTING=False \
    -D CMAKE_BUILD_TYPE=Release
##TODO - OPENCV_DIR replace like jetson nano

# catkin build \
#     rtabmap \
#     rtabmap_ros \
#     realsense2_camera \
#     cv_bridge
