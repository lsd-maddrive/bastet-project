#include "ros.h"
#include "ros_proto.h"

ros::NodeHandle ros_node;

void rosInit()
{
	ros_node.init();
}

