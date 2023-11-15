#!/usr/bin/env python

# based on https://github.com/winstxnhdw/KinematicBicycleModel

import rospy
import roslib
# roslib.load_manifest('differential_drive')
from math import sin, cos, pi, tan, atan,atan2,isclose

from kitty_msgs.msg import KittyState
from geometry_msgs.msg import Quaternion
import numpy as np
from geometry_msgs.msg import Twist
from nav_msgs.msg import Odometry
from tf.broadcaster import TransformBroadcaster
from tf.transformations import quaternion_from_euler
from std_msgs.msg import Int16
from std_msgs.msg import Float32


class KinematicBicycleModelTf:

    def __init__(self):
        rospy.init_node("bicycle_tf")
        self.nodename = rospy.get_name()
        rospy.loginfo("-I- %s started" % self.nodename)
        
        #### parameters #######
        self.rate = rospy.get_param('~rate',10.0)  # the rate at which to publish the transform
        self.wheelbase = float(rospy.get_param('~wheelbase', 0.175)) # The wheel base width in meters
        self.max_steer = float(rospy.get_param('~max_steer', 0.8)) # Radians
        self.base_frame_id = rospy.get_param('~base_frame_id','base_link') # the name of the base frame of the robot
        self.odom_frame_id = rospy.get_param('~odom_frame_id', 'odom') # the name of the odometry reference frame
        self.publish_tf = rospy.get_param('~publish_tf', 'false')
        self.self.robot_len = 0.35
        self.robot_width = 0.35

        
        self.t_delta = rospy.Duration(1.0/self.rate)
        self.t_next = rospy.Time.now() + self.t_delta
        
        # internal data
        self.x = 0                  # position in xy plane 
        self.y = 0
        self.yaw = 0
        self.velocity = 0
        self.accleration = 0
        self.steering_angle = 0
        self.angular_velocity = 0


        self.then = rospy.Time.now()
        
        # subscriptions
        rospy.Subscriber("/kitty/cmd_vel", Twist, self.VirtualVelociityAngleCb) # virtual
        rospy.Subscriber("/kitty_state", KittyState, self.RealVelociityAngleCb)


        # rospy.Subscriber("lwheel", Float32, self.lwheelCallback)
        # rospy.Subscriber("rwheel", Float32, self.rwheelCallback)
        self.odomPub = rospy.Publisher("odom", Odometry, queue_size=10)
        self.odomBroadcaster = TransformBroadcaster()
        
    def spin(self):
        r = rospy.Rate(self.rate)
        while not rospy.is_shutdown():
            self.update()
            r.sleep()
       
     
    def update(self):
        now = rospy.Time.now()
        if now > self.t_next:
            elapsed = now - self.then
            self.then = now
            elapsed = elapsed.to_sec()
            
            # calculate odometry

            normalise_angle = lambda angle: atan2(sin(angle), cos(angle))
            # Compute the local velocity in the x-axis
            # new_velocity = velocity + self.delta_time * acceleration

            # Limit steering angle to physical vehicle limits
            self.steering_angle = -self.max_steer if self.steering_angle < -self.max_steer else self.max_steer if self.steering_angle > self.max_steer else self.steering_angle

            # Compute the angular velocity
            self.angular_velocity = self.velocity*tan(self.steering_angle) / self.wheelbase

            # Compute the final state using the discrete time model
            self.x   = self.x + self.velocity*cos(self.yaw)*elapsed
            self.y   = self.y + self.velocity*sin(self.yaw)*elapsed
            self.yaw = normalise_angle(self.yaw + self.angular_velocity*elapsed)


                
            # publish the odom information

            q = quaternion_from_euler(0,0,self.yaw)
            quaternion = Quaternion()
            quaternion.x = 0.0
            quaternion.y = 0.0
            quaternion.z = q[2]
            quaternion.w = q[3]

            if self.publish_tf:
                self.odomBroadcaster.sendTransform(
                    (self.x, self.y, 0),
                    (quaternion.x, quaternion.y, quaternion.z, quaternion.w),
                    rospy.Time.now(),
                    self.base_frame_id,
                    self.odom_frame_id
                    )
            
            odom = Odometry()
            odom.header.stamp = now
            odom.header.frame_id = self.odom_frame_id
            odom.pose.pose.position.x = self.x
            odom.pose.pose.position.y = self.y
            odom.pose.pose.position.z = 0
            odom.pose.pose.orientation = quaternion
            odom.child_frame_id = self.base_frame_id
            odom.twist.twist.linear.x = self.velocity
            odom.twist.twist.linear.y = 0
            odom.twist.twist.angular.z = self.angular_velocity
            self.odomPub.publish(odom)
            


    def VirtualVelociityAngleCb(self,msg):
        pass
        # self.velocity = msg.linear.x
        # self.steering_angle = msg.angular.z

    def CalculateRealAngle(self, msg):
        self.robot_len = 0.35
        robot_width = 0.35

        right_steering_angle = abs(np.deg2rad((abs(msg.front_right_steering_angle) + abs(msg.rear_right_steering_angle)) / 2))
        left_steering_angle = abs(np.deg2rad((abs(msg.front_left_steering_angle) + abs(msg.rear_left_steering_angle)) / 2))
        # print(right_steering_angle,left_steering_angle)

        if isclose(right_steering_angle, 0,abs_tol=0.02) or isclose(left_steering_angle,0, abs_tol=0.02):
            cc_angle = 0 
        else:
            dir = msg.front_right_steering_angle / abs(msg.front_right_steering_angle)
            if dir >= 0:
                tan_cl_angle = 0.5*self.robot_len / (0.5*self.robot_len/tan(left_steering_angle) - robot_width/2)
                tan_cr_angle = 0.5*self.robot_len / (0.5*self.robot_len/tan(right_steering_angle) + robot_width/2)
            else:
                tan_cl_angle = 0.5*self.robot_len / (0.5*self.robot_len/tan(left_steering_angle) + robot_width/2)
                tan_cr_angle = 0.5*self.robot_len / (0.5*self.robot_len/tan(right_steering_angle) - robot_width/2)

            cc_angle = atan((tan_cl_angle + tan_cr_angle)/2) * dir
        #return
        self.steering_angle = -cc_angle
        print(self.steering_angle)

    def CalculateRealSpeed(self, msg):

        h = 0.5*self.robot_len / tan(0.01745*(self.steering_angle))
        left_rotation_speed = (msg.front_left_rotation_speed + msg.rear_left_rotation_speed) /2 
        right_rotation_speed = (msg.front_right_rotation_speed + msg.rear_right_rotation_speed) /2 

        Vcl = (left_rotation_speed * h) / (h + self.robot_width/2)
        Vcr = (right_rotation_speed * h) / (h - self.robot_width/2)
        Vc = (Vcl + Vcr) / 2
        self.velocity = Vc

    def RealVelociityAngleCb(self,msg):
        self.CalculateRealAngle(msg)
        self.CalculateRealSpeed(msg)




if __name__ == '__main__':
    """ main """
    try:
        BicycleTf = KinematicBicycleModelTf()
        BicycleTf.spin()
    except rospy.ROSInterruptException:
        pass