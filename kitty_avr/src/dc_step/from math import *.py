from math import *
LEN_WHEEL = 34
WID_WHEEL = 34
_set_angle = 30
set_speed = 300

h = LEN_WHEEL / tan(0.01745*_set_angle);
R_left = sqrt(pow((LEN_WHEEL/2),2) + pow((WID_WHEEL/2+h),2));
R_centre = sqrt(pow((LEN_WHEEL/2),2) + pow((WID_WHEEL/2),2));
R_right = sqrt(pow((LEN_WHEEL/2),2) + pow((WID_WHEEL/2-h),2));
QQ_left = atan((LEN_WHEEL / 2)/(h-WID_WHEEL/2));
QQ_centre = atan((LEN_WHEEL / 2)/h);
QQ_right = atan((LEN_WHEEL / 2)/(h+WID_WHEEL/2));
lw_speed = set_speed * QQ_left * R_left / (R_centre * QQ_centre);
rw_speed = set_speed * QQ_right * R_right / (R_centre * QQ_centre);  

print(lw_speed, rw_speed)