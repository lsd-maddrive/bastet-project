# from math import *

# #define REVMIN_2_MS (PI*WHEEL_D) / 60 
# # speed_ms = desired_speed * REVMIN_2_MS 
# x0 = 0
# y0 = 0
# x1 = 0
# y1 = 0
# alf = 0



# def Light_Wheel_Odometry (set_speed,set_angle):
#     global x0
#     global y0
#     global alf
#     x1, y1 = 0, 0
#     t = 0.08
#     robot_len = 0.35

#     if (set_angle == 0):
#         x1 = x0 + set_speed * t

#     # elif (set_speed !=0):
#     else:
#         dir_x= set_speed/ abs(set_speed)
#         dir_y= set_angle/ abs(set_angle)
#         h_angle = abs(set_angle)
#         h = robot_len /2/ tan(0.01745*h_angle)
#         fi= set_speed*t/h
#         sf= sin(0.01745*fi)
#         sa= sin(0.01745*alf)
#         cf= cos(0.01745*fi)
#         ca= cos(0.01745*alf)

#         DeltaY=sa*sf*h+ca*pow(pow(h,2)*pow((-1 + cf),2) +pow(h,2)*pow(sf,2)-pow(h,2)*pow(sf,2),0.5)
#         if (((abs(alf))<90) and (((abs(alf))>=0))):
#             x1=x0+ dir_x*pow( pow(h-cf*h,2)+pow(sf*h,2)-pow(DeltaY,2), 0.5)
#             y1=y0- dir_y*DeltaY

#         elif (((abs(alf))<180) and (((abs(alf))>=90))):
#             x1=x0- dir_x*pow( pow(h-cf*h,2)+pow(sf*h,2)-pow(DeltaY,2), 0.5)
#             y1=y0- dir_y*DeltaY

#         elif (((abs(alf))<270) and (((abs(alf))>=180))):
#             x1=x0- dir_x*pow( pow(h-cf*h,2)+pow(sf*h,2)-pow(DeltaY,2), 0.5)
#             y1=y0- dir_y*DeltaY

#         elif (((abs(alf))<=360) and (((abs(alf))>=270))):
#             x1=x0+ dir_x *pow( pow(h-cf*h,2)+pow(sf*h,2)-pow(DeltaY,2), 0.5)
#             y1=y0- dir_y *DeltaY

#         alf=alf+fi
#         if (alf>360): 
#             alf=alf-360
#         if (alf<-360): 
#             alf=alf+360
            
#     x0=x1
#     y0=y1


# if __name__ == "__main__":
#     for x in range (2000):
#         Light_Wheel_Odometry(1,1)
#     print(x0,y0)

from math import sin, cos, pi, tan, atan,atan2 ,sqrt
import numpy as np


# CalculateRealAngle

# robot_len = 0.35
# robot_width = 0.35
# front_left_steering_angle = 21
# front_right_steering_angle = 57
# rear_left_steering_angle = -21
# rear_right_steering_angle = -57

# dir = front_right_steering_angle / abs(front_right_steering_angle)

# right_steering_angle = abs(np.deg2rad((abs(front_right_steering_angle) + abs(rear_right_steering_angle)) / 2))
# left_steering_angle = abs(np.deg2rad((abs(front_left_steering_angle) + abs(rear_left_steering_angle)) / 2))


# tan_cl_angle = 0.5*robot_len / (0.5*robot_len/tan(left_steering_angle) - robot_width/2)
# tan_cr_angle = 0.5*robot_len / (0.5*robot_len/tan(right_steering_angle) + robot_width/2)
# cc_angle = atan((tan_cl_angle + tan_cr_angle)/2) * dir

# print(np.rad2deg(cc_angle))

# CalculateSpeed

robot_len = 0.35
robot_width = 0.35

## reverse task
front_left_rotation_speed = 418.33402701145803
front_right_rotation_speed = 192.7029002943127
set_angle = 25
h = 0.5*robot_len / tan(0.01745*(set_angle)) + robot_width/2


Vcl = (front_left_rotation_speed * h) / (h + robot_width/2)
Vcr = (front_right_rotation_speed * h) / (h - robot_width/2)
Vc = (Vcl + Vcr) / 2

print(Vc)

## direct task

# set_speed = 300
# set_angle = 25
# h = 0.5*robot_len / tan(0.01745*(set_angle))
# R_left = sqrt(pow((robot_len/2),2) + pow((robot_width/2+h),2))
# R_centre = sqrt(pow((robot_len/2),2) + pow((h),2))
# R_right = sqrt(pow((robot_len/2),2) + pow((h-robot_width/2),2))
# QQ_left = atan((robot_len / 2)/(h+robot_width/2))
# QQ_centre = atan((robot_len / 2)/h)
# QQ_right = atan((robot_len / 2)/(h-robot_width/2))
# lw_speed = set_speed * R_left / R_centre
# rw_speed = set_speed * R_right / R_centre
# print(lw_speed, rw_speed)