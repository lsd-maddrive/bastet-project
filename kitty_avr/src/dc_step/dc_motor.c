#include "dc_motor.h"
#include "math.h"
#include <avr/io.h>


//uint16_t tim2_count=0, dc_mot_enc_count = 0, enc_result = 0;
uint16_t tim2_count=0, dc_mot_enc_count[4]={0,0,0,0}, enc_result[4]={0,0,0,0};
int16_t rotation_dir = 1;
float reg_speed[4]={0,0,0,0}, integral[4]={0,0,0,0};
//uint32_t enc_result = 0;
float speed_ms[4] = {0,0,0,0}; //for odom calculate
float set_speed = 0;
float _set_angle = 0; // only for calculate turning radius
float debug_formuls[6]={0,0,0,0,0,0};
float odom_info[3]={0,0,0};

float x0,y0,alf;



void Tim2DcMotInit(void){
	//����� PH6(OC2B), PB4(OC2A) � ���, ����� PH4 - �����������
	TCCR2A |= (1<<COM2A1);
	TCCR2A |= (1<<COM2B1);
	TIMSK2 |=(1<<TOIE2);
	//����� PH3(OC4A), PH5(OC4C) � ���
	TCCR4A |= (1<<COM4A1);
	TCCR4A |= (1<<COM4C1);
	/* TIMER2 - ��������� �������: ��� � ������� ����������,
	����������������� �����, ������������ �� 256 122.55hz */
	TCCR2A |= (1<<WGM20);
	TCCR2B |= (1<<CS22) | (1<<CS21);
	/* TIMER4 - ��������� �������: ������� ���,
	����������������� �����, 8 bit, TOP = 0xFF, ������������ �� 256*/
	TCCR4A |= (1 << WGM40);
	TCCR4B |= (1<<CS42);
}

void DcMotInit(void){
	Tim2DcMotInit();
	IntDcMotEcoderInit();
	DC_MOT1_SPEED_DDR|=(1<<DC_MOT1_SPEED_DDR_PIN);
	 DC_MOT2_SPEED_DDR|=(1<<DC_MOT2_SPEED_DDR_PIN);
	DC_MOT3_SPEED_DDR|=(1<<DC_MOT3_SPEED_DDR_PIN);
	DC_MOT4_SPEED_DDR|=(1<<DC_MOT4_SPEED_DDR_PIN);
	
	DC_MOT_DIR_DDR|=(1<<DC_MOT_DIR_DDR_PIN);
}



uint16_t* GetSpeed(void){
	return(enc_result);
}


float* GetSpeedMS(void){
	speed_ms[0] = (int16_t)enc_result[0] * rotation_dir * REVMIN_2_MS;
	speed_ms[1] = (int16_t)enc_result[1] * rotation_dir * REVMIN_2_MS;
	speed_ms[2] = (int16_t)enc_result[2] * rotation_dir * REVMIN_2_MS;
	speed_ms[3] = (int16_t)enc_result[3] * rotation_dir * REVMIN_2_MS;
	return(speed_ms);
}

float* GetFormuls(float _speed,float _angle)
{
float h = 0 ,R_left = 0 ,R_centre = 0 ,R_right = 0 , QQ_left=0, QQ_centre=0, QQ_right=0, lw_speed,rw_speed;
	if (_angle ==0)
		{
		lw_speed = _speed;
		rw_speed = _speed;
		}
	else
		{ 	 
			h = 0.5*LEN_WHEEL / tan(0.01745*(_set_angle));
			R_left = sqrt(pow((LEN_WHEEL/2),2) + pow((WID_WHEEL/2+h),2));
			R_centre = sqrt(pow((LEN_WHEEL/2),2) + pow((h),2));
			R_right = sqrt(pow((LEN_WHEEL/2),2) + pow((h-WID_WHEEL/2),2));
			QQ_left = atan((LEN_WHEEL / 2)/(h+WID_WHEEL/2));
			QQ_centre = atan((LEN_WHEEL / 2)/h);
			QQ_right = atan((LEN_WHEEL / 2)/(h-WID_WHEEL/2));
			lw_speed = set_speed  * R_left / (R_centre );
			rw_speed = set_speed  * R_right / (R_centre ); 

    
		}
	debug_formuls[0] = _angle;
	debug_formuls[1] = R_left;
	debug_formuls[2] = R_centre;
	debug_formuls[3] = R_right;
	debug_formuls[4] = lw_speed;
	debug_formuls[5] = rw_speed;

	return (debug_formuls);
}


void SetSpeed(float desired_speed, float desired_angle){
	if(desired_speed>0){
		rotation_dir = 1;
		DC_MOT_FOR;
	}
	else if (desired_speed<0){
		rotation_dir = -1;
		DC_MOT_REV;
	} 
	
	set_speed = abs(desired_speed);
	_set_angle = desired_angle;
	// speed_ms = desired_speed * REVMIN_2_MS;
}


void DcMotGo(float* speed){
	// 1 and 2 front
	// 0 and 4 back

	// 0 and 1 right
	// 3 and 2 left


	// 0 and 1 front
	// 3 and 2 back

	// 0 and 3 right
	// 1 and 2 left
	OCR4A = speed[0];
	OCR2A = speed[3];
	OCR2B = speed[2];
	OCR4C = speed[1];
}

void DcMotPIDGo(float set_speed){
	//float speed_test[4]={0,0,0,0};
	// calculate each wheel
	float h, lw_speed = 50, rw_speed=50, R_left, R_centre, R_right, QQ_left=0, QQ_centre=0, QQ_right=0;
	if (_set_angle ==0)
		{
		lw_speed = set_speed;
		rw_speed = set_speed;
		}
	else
		{
			h = 0.5*LEN_WHEEL / tan(0.01745*(_set_angle));
			R_left = sqrt(pow((LEN_WHEEL/2),2) + pow((WID_WHEEL/2+h),2));
			R_centre = sqrt(pow((LEN_WHEEL/2),2) + pow((h),2));
			R_right = sqrt(pow((LEN_WHEEL/2),2) + pow((h-WID_WHEEL/2),2));
			QQ_left = atan((LEN_WHEEL / 2)/(h+WID_WHEEL/2));
			QQ_centre = atan((LEN_WHEEL / 2)/h);
			QQ_right = atan((LEN_WHEEL / 2)/(h-WID_WHEEL/2));
			lw_speed = set_speed * R_left / R_centre;
			rw_speed = set_speed * R_right / R_centre; 

		}
	float differential_speed[4]={rw_speed,rw_speed,lw_speed,lw_speed};
	//float differential_speed[4]={lw_speed, rw_speed, rw_speed, lw_speed};
	
	for(uint8_t i=0; i<4; i++){
		reg_speed[i] = ComputePI(GetSpeed()[i], differential_speed[i], i);
		// reg_speed[i] = ComputePI(GetSpeed()[i], set_speed, i);
	}
	DcMotGo(reg_speed);
}

void IntDcMotEcoderInit(void){
	EICRA=(1<<ISC01) | (1<<ISC11) | (1<<ISC21) | (1<<ISC31);
	EIMSK=(1<<INT0) | (1<<INT1) | (1<<INT2) | (1<<INT3);

}


//

ISR (INT0_vect)
{
	dc_mot_enc_count[3]+=1;
}

ISR (INT1_vect)
{
	//PORTB^=(1<<7);
	dc_mot_enc_count[0]+=1;
}

ISR (INT2_vect)
{
	//PORTB^=(1<<7);
	dc_mot_enc_count[2]+=1;
}

ISR (INT3_vect)
{
	PORTB^=(1<<7);
	dc_mot_enc_count[1]+=1;
}

ISR(TIMER2_OVF_vect){ //isr executes every 8 ms
	if(tim2_count<10) tim2_count++; //every 80 ms
	else{
		for(uint8_t i=0; i<4; i++){
			enc_result[i]=((uint32_t)dc_mot_enc_count[i]*12.5*60)/115; //rev per minute
			dc_mot_enc_count[i]=0;
		}
		DcMotPIDGo(set_speed);
		// Light_Wheel_Odometry(set_speed, _set_angle);
		//PORTB^=(1<<7);
		tim2_count=0;
	}
}


float ComputePI(uint16_t input, float setpoint, uint8_t integral_num){
	float kp = 0.7;
	float ki = 0.0;
	float dt = 0.08;
	float error = setpoint - input;
	integral[integral_num] = integral[integral_num] + (error*dt);
	float control = (error * kp) + (integral[integral_num] * ki);
	if(control > 255) //����������� ������� ���������� ������
	control = 255;
	if(control < 0) //����������� ������� ���������� �����
	control = 0;
	return(control);
}


float* GetOdom(void){

	odom_info[0] = x0;
	odom_info[1] = y0;
	// odom_info[2] = velocity;
	// odom_info[3] = yaw;
	return(odom_info);
}


float Light_Wheel_Odometry (float set_speed, float set_angle)
{
    float h, dir_x, dir_y, fi, sf, sa, cf, ca, DeltaY, x1, y1, len_wheel_m, h_angle;
	// float t = 0.08;

	// if (set_angle == 0){
 	// 	x1 = x0 + speed_ms * t;
	// 	x0 = x1;
	// }
	
	// else if (speed_ms !=0){

	// 	len_wheel_m = LEN_WHEEL / 100;
	// 	dir_x= speed_ms/ fabs(speed_ms);
	// 	dir_y= set_angle/ abs(set_angle);
	// 	h_angle = abs(set_angle)
	// 	h = len_wheel_m /2/ tan(0.01745*h_angle);
	// 	fi= speed_ms*t/h;
	// 	sf= sin(0.01745*fi);
	// 	sa= sin(0.01745*alf);
	// 	cf= cos(0.01745*fi);
	// 	ca= cos(0.01745*alf);
		
	// 	DeltaY=sa*sf*h+ca*pow(pow(h,2)*pow((-1 + cf),2) +pow(h,2)*pow(sf,2)-pow(h,2)*pow(sf,2),0.5);
	// 	if (((fabs(alf))<90) && (((fabs(alf))>=0)))
	// 		{
	// 		x1=x0+ dir_x*pow( pow(h-cf*h,2)+pow(sf*h,2)-pow(DeltaY,2), 0.5);
	// 		y1=y0- dir_y*DeltaY;
	// 		}
	// 	else if (((fabs(alf))<180) && (((fabs(alf))>=90)))
	// 		{
	// 		x1=x0- dir_x*pow( pow(h-cf*h,2)+pow(sf*h,2)-pow(DeltaY,2), 0.5);
	// 		y1=y0- dir_y*DeltaY;
	// 		}
	// 	else if (((fabs(alf))<270) && (((fabs(alf))>=180)))
	// 		{
	// 		x1=x0- dir_x*pow( pow(h-cf*h,2)+pow(sf*h,2)-pow(DeltaY,2), 0.5);
	// 		y1=y0- dir_y*DeltaY;
	// 		}
	// 	else if (((fabs(alf))<=360) && (((fabs(alf))>=270)))
	// 		{
	// 		x1=x0+ dir_x *pow( pow(h-cf*h,2)+pow(sf*h,2)-pow(DeltaY,2), 0.5);
	// 		y1=y0- dir_y *DeltaY;
	// 		}
		
	// 	alf=alf+fi;
	// 	if (alf>360) alf=alf-360;
	// 	if (alf<-360) alf=alf+360;
	// 	x0=x1;
    // 	y0=y1;
	// }
}
//previous_error = 0;
//integral = 0;
//OCRnA = 1500;
//CurrentOCR = 1500;
//dt = 50; //for instance
//while(1)
//{
//actual = ReadADC(); //read sonar value
//error = SetPoint - actual; //calculate P error
//integral = integral + (error*dt); //calculate integral
//derivative = (error - previous_error)*(1/dt); //calculate derivative
//output = (PGain*error) + (IGain*integral) + (DGain*derivative); //calculate PID sum
//OCRnA = CurrentOCR + output; //change PWM output based on the PID value
//previous_error = error; //set previous_error to current error
//_delay_ms(dt);
//}




//ISR(TIMER2_OVF_vect){ //isr executes every 8 ms
//if(tim2_count<12) tim2_count++; //every 100 ms
//else{
//SetSpeed(dc_mot_enc_count);
//dc_mot_enc_count=0;
//tim2_count=0;
//}
//}

