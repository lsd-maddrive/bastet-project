from math import *
WHEEL_D = 0.065
REVMIN_2_MS = (pi*WHEEL_D) / 60 
LEN_WHEEL = 35
x0 = 0
y0 = 0
alf = 0

def Light_Wheel_Odometry(set_speed, set_angle):
  global x0
  global y0
  global alf


  speed_ms = set_speed * REVMIN_2_MS

  dir_x = speed_ms / abs(speed_ms)
  dir_y = set_angle / abs(set_angle)
  set_angle= abs(set_angle)
  h = LEN_WHEEL /2/ tan(0.01745*set_angle)
  fi= speed_ms*0.08/h
  sf= sin(0.01745*fi)
  sa= sin(0.01745*alf)
  cf= cos(0.01745*fi)
  ca= cos(0.01745*alf)

  DeltaY=sa*sf*h+ca*pow(pow(h,2)*pow((-1 + cf),2) +pow(h,2)*pow(sf,2)-pow(h,2)*pow(sf,2),0.5)
  if (((abs(alf))<90) and (((abs(alf))>=0))):
    x1=x0+ dir_x*pow( pow(h-cf*h,2)+pow(sf*h,2)-pow(DeltaY,2), 0.5)
    y1=y0- dir_y*DeltaY
  elif (((abs(alf))<180) and (((abs(alf))>=90))):
    x1=x0- dir_x*pow( pow(h-cf*h,2)+pow(sf*h,2)-pow(DeltaY,2), 0.5)
    y1=y0- dir_y*DeltaY
  elif (((abs(alf))<270) and (((abs(alf))>=180))):
    x1=x0- dir_x*pow( pow(h-cf*h,2)+pow(sf*h,2)-pow(DeltaY,2), 0.5)
    y1=y0- dir_y*DeltaY
  elif (((abs(alf))<=360) and (((abs(alf))>=270))):
    x1=x0+ dir_x *pow( pow(h-cf*h,2)+pow(sf*h,2)-pow(DeltaY,2), 0.5)
    y1=y0- dir_y*DeltaY

  x0=x1
  y0=y1
  alf=alf+fi

  if (alf>360): alf=alf-360
  if (alf<-360): alf=alf+360

for x in range(0,450):
  Light_Wheel_Odometry(300,0)
print(x0,y0,alf)



clc;
clear;


LEN_WHEEL = 0.35;
WID_WHEEL = 0.35;
WHEEL_D = 0.065;
REVMIN_2_MS = (pi*WHEEL_D) / 60;

%syms set_angle
% set_speed = 500 * REVMIN_2_MS;
set_speed = 300 * REVMIN_2_MS ;

x0=0;
y0=0;
alf=0;
set_angle = 1;
    


# for c = 1:20000
#     % if (c>500)
#     %     set_angle=-1;        
#     % end
#     % if (c<=500)
#     %     set_angle=-1;        
#     % end    
#     dir_x= set_speed/ abs(set_speed);
#     dir_y= set_angle/ abs(set_angle);
#     set_angle= abs(set_angle);
#     h = LEN_WHEEL /2/ tan(0.01745*set_angle);
#     fi= set_speed*0.08/h;
#     sf= sin(0.01745*fi);
#     sa= sin(0.01745*alf);
#     cf= cos(0.01745*fi);
#     ca= cos(0.01745*alf);
    
# yy=sa*sf*h+ca*power(power(h,2)*power((-1 + cf),2) +power(h,2)*power(sf,2)-power(h,2)*power(sf,2),0.5);
#     if ((abs(alf))<90) && (((abs(alf))>=0))
# %       yy=sa*sf*h+ca*power(power(h,2)*power((-1 + cf),2) +power(h,2)*power(sf,2)-power(h,2)*power(sf,2),0.5);
#        x1=x0+dir_x*power( power(h-cf*h,2)+power(sf*h,2)-power(yy,2)    ,0.5);
#        y1=y0-dir_y*yy;
#     end   
#     if ((abs(alf))<180) && (((abs(alf))>=90))
# %       yy=sa*sf*h+ca*power(power(h,2)*power((-1 + cf),2) +power(h,2)*power(sf,2)-power(h,2)*power(sf,2),0.5);
#        x1=x0-dir_x*power( power(h-cf*h,2)+power(sf*h,2)-power(yy,2)    ,0.5);
#        y1=y0-dir_y*yy;
#     end
#     if ((abs(alf))<270) && (((abs(alf))>=180))
# %       yy=sa*sf*h+ca*power(power(h,2)*power((-1 + cf),2) +power(h,2)*power(sf,2)-power(h,2)*power(sf,2),0.5);
#        x1=x0-dir_x*power( power(h-cf*h,2)+power(sf*h,2)-power(yy,2)    ,0.5);
#        y1=y0-dir_y*yy;
       
#     end
#     if ((abs(alf))<=360) && (((abs(alf))>=270))  
# %       yy=sa*sf*h+ca*power(power(h,2)*power((-1 + cf),2) +power(h,2)*power(sf,2)-power(h,2)*power(sf,2),0.5);
#        x1=x0+dir_x*power( power(h-cf*h,2)+power(sf*h,2)-power(yy,2)    ,0.5);
#        y1=y0-dir_y*yy;
       
#     end
    
#     %x1 = x0+sin(0.01745*alf)*(sin(0.01745*fi)*h+h-cos(0.01745*fi)*h);
#     %y1 = y0+cos(0.01745*alf)*(sin(0.01745*fi)*h+h-cos(0.01745*fi)*h);
#     x0=x1;
#     y0=y1;
#     x_arr (c) = x1;
#     y_arr (c) = y1;
#     alf=alf+fi;
#     if (alf>360)
#         alf=alf-360;
#     end
#     if (alf<-360)
#         alf=alf+360;
#     end
# end
# plot (x_arr,y_arr, '*')
# grid on

# %R_left = sqrt(power((LEN_WHEEL / 2),2) + power((WID_WHEEL/2 + h),2));
# %R_centre = sqrt(power((LEN_WHEEL / 2),2) + power((h),2));
# %R_right = sqrt(power((LEN_WHEEL / 2),2) + power((h - WID_WHEEL/2 ),2));
# %QQ_left = atan((LEN_WHEEL / 2)/(h-WID_WHEEL/2));
# %QQ_centre = atan((LEN_WHEEL / 2)/h);
# %QQ_right = atan((LEN_WHEEL / 2)/(h+WID_WHEEL/2));%%

# %lw_speed = set_speed * QQ_left * R_left / (R_centre * QQ_centre);
# %rw_speed = set_speed * QQ_right * R_right / (R_centre * QQ_centre);  
# % 
# % plot(rw_speed,lw_speed)
# % plot(set_angle,rw_speed)
# %fplot(rw_speed, [-90 90])

# %xlabel('angle')
# %ylabel('lw_speed')
# %title('set speed = 500')
# %