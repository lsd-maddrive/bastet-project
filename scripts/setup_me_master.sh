#!/bin/bash

export ROS_HOSTNAME=192.168.88.48
export ROS_MASTER_URI=http://$ROS_HOSTNAME:11311

echo "Master: $ROS_MASTER_URI / Hostname: $ROS_HOSTNAME"
