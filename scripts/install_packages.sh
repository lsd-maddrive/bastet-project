#!/usr/bin/env bash

sudo apt install \
    ros-$ROS_DISTRO-rosserial-server \
    ros-$ROS_DISTRO-rosserial-client \
    ros-$ROS_DISTRO-robot-localization \
    ros-$ROS_DISTRO-move-base \
    ros-$ROS_DISTRO-global-planner \
    ros-$ROS_DISTRO-teb-local-planner \
    ros-$ROS_DISTRO-joy \
    ros-$ROS_DISTRO-imu-filter-madgwick \
    ros-$ROS_DISTRO-rviz-imu-plugin \
    ros-$ROS_DISTRO-hector-gazebo-plugins \
    ros-$ROS_DISTRO-octomap-msgs \
    ros-$ROS_DISTRO-ddynamic-reconfigure \
    ros-$ROS_DISTRO-spatio-temporal-voxel-layer \
    ros-$ROS_DISTRO-robot-upstart \
    libopenvdb-dev
    
#    "ros-$ROS_DISTRO-rtabmap=0.20.18-3*" \
