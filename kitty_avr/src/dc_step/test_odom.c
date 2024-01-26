#include <iostream>

float Light_Wheel_Odometry (float set_speed, float set_angle)
{
    float h, dir_x, dir_y, fi, sf, sa, cf, ca, DeltaY, x1, y1, len_wheel_m, h_angle;
	float t = 0.08;

	if (set_angle == 0){
 		x1 = x0 + speed_ms * t;
		x0 = x1;
	}
	
	else if (speed_ms !=0){

		len_wheel_m = LEN_WHEEL / 100;
		dir_x= speed_ms/ fabs(speed_ms);
		dir_y= set_angle/ abs(set_angle);
		h_angle = abs(set_angle)
		h = len_wheel_m /2/ tan(0.01745*h_angle);
		fi= speed_ms*t/h;
		sf= sin(0.01745*fi);
		sa= sin(0.01745*alf);
		cf= cos(0.01745*fi);
		ca= cos(0.01745*alf);
		
		DeltaY=sa*sf*h+ca*pow(pow(h,2)*pow((-1 + cf),2) +pow(h,2)*pow(sf,2)-pow(h,2)*pow(sf,2),0.5);
		if (((fabs(alf))<90) && (((fabs(alf))>=0)))
			{
			x1=x0+ dir_x*pow( pow(h-cf*h,2)+pow(sf*h,2)-pow(DeltaY,2), 0.5);
			y1=y0- dir_y*DeltaY;
			}
		else if (((fabs(alf))<180) && (((fabs(alf))>=90)))
			{
			x1=x0- dir_x*pow( pow(h-cf*h,2)+pow(sf*h,2)-pow(DeltaY,2), 0.5);
			y1=y0- dir_y*DeltaY;
			}
		else if (((fabs(alf))<270) && (((fabs(alf))>=180)))
			{
			x1=x0- dir_x*pow( pow(h-cf*h,2)+pow(sf*h,2)-pow(DeltaY,2), 0.5);
			y1=y0- dir_y*DeltaY;
			}
		else if (((fabs(alf))<=360) && (((fabs(alf))>=270)))
			{
			x1=x0+ dir_x *pow( pow(h-cf*h,2)+pow(sf*h,2)-pow(DeltaY,2), 0.5);
			y1=y0- dir_y *DeltaY;
			}
		
		alf=alf+fi;
		if (alf>360) alf=alf-360;
		if (alf<-360) alf=alf+360;
		x0=x1;
    	y0=y1;
	}
}

int main(){
    
 std::cout <<   Light_Wheel_Odometry() << std::endl;
}