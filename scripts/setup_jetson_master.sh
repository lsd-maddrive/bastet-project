#!/bin/bash

export ROS_HOSTNAME=$(hostname).local

##by name
# export ROS_MASTER_URI=http://$ROS_HOSTNAME:11311
##by ip
export ROS_MASTER_URI=http://192.168.88.242:11311
echo "Master: $ROS_MASTER_URI / Hostname: $ROS_HOSTNAME"
