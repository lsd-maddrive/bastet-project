#!/bin/bash

##by ip
export ROS_HOSTNAME=192.168.88.242
export ROS_MASTER_URI=http://$ROS_HOSTNAME:11311

##by name
# export ROS_HOSTNAME=user-Strix-15-GL503GE.local
# export ROS_MASTER_URI=http://$ROS_HOSTNAME:11311


echo "Master: $ROS_MASTER_URI / Hostname: $ROS_HOSTNAME"

