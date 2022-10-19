#ifndef _ROS_H_
#define _ROS_H_

#include "ros/node_handle.h"
#include "Atmega2560Hardware.h"

namespace ros
{
  typedef ros::NodeHandle_<Atmega2560Hardware> NodeHandle;
}

#endif