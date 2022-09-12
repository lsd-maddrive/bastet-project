#include "st_mot.h"

uint8_t operate_flag=0, direction_flag=0;
uint8_t st_mot_chosen=0;
uint16_t pulse_count=0, pulse_setpoint=0;
float angle_setpoint=0, current_angle=0, set_angle = 0, real_mot_pos = 0;

uint16_t set_counter =0;


void StMotTim1Init(void){
	////timer in normal mode
	TCCR1B|=(1<<CS11) | (1<<CS10); //prescaler 64
	//TCCR1B|=(1<<CS12); //prescaler 256
	
	TCNT1=65535-100; //timer period = 400 us, delta = 100
	TIMSK1|=(1<<TOIE1);
}

void StMotInit(void){
	StMotTim1Init();
	
	//ST_MOT_PUL_DDR|=(1<<DD_PUL1)|(1<<DD_PUL2)|(1<<DD_PUL3)|(1<<DD_PUL4);
	//ST_MOT_DIR_DDR|=(1<<DD_DIR1)|(1<<DD_DIR2)|(1<<DD_DIR3)|(1<<DD_DIR4);
	
	ST_MOT_PUL_DDR|=(1<<DD_PUL1);
	ST_MOT_DIR_DDR|=(1<<DD_DIR1);

	pulse_setpoint=0;
	
}

ISR(TIMER1_OVF_vect){
	TCNT1=65535-100; //timer period = 400 us

	//StMotGo();
	if (operate_flag)
	{
		if (pulse_count < pulse_setpoint){
			StMotPul();
			pulse_count++;
			
		}
		else
		{
			float real_mot_pos;
			real_mot_pos = GetMotPos();
			operate_flag=0;
			current_angle=set_angle;
			pulse_count=0;
			
			if((real_mot_pos>=(set_angle-POS_ERR)) && (real_mot_pos<=(set_angle+POS_ERR)))
			{
				
			
			}
			else
			{
				//operate_flag=1;
				////angle_setpoint_delta is a difference between setpoint and real angle of the shaft
				//float angle_setpoint_delta=set_angle-real_mot_pos;
				////setting up the direction of rotation according to delta
				//StMotDir(angle_setpoint_delta);
				////3200 steps (LOOK FOR THE STEPPER MODE!) / 180 degrees
				//pulse_setpoint=abs(angle_setpoint_delta) * ANGLE_TO_STEPS;
				//pulse_count=0;
			}
			
		}
	}
}


void StMotCorrectPos(void){
	//reading real real shaft pos relative to 512
	float real_mot_pos;
	//~300 potentiometer degrees / 1024 = 0.29..
	real_mot_pos=(512.0f-(float)AdcGetPos()[st_mot_chosen-1])*0.290323;
	//position error for the shaft is POS_ERR
	if((real_mot_pos>=(angle_setpoint-POS_ERR)) && (real_mot_pos<=(angle_setpoint+POS_ERR))) operate_flag=0;
	else operate_flag=1;
	//angle_setpoint_delta is a difference between setpoint and real angle of the shaft
	float angle_setpoint_delta=angle_setpoint-real_mot_pos;
	//setting up the direction of rotation according to delta
	StMotDir(angle_setpoint_delta);
	//annihilating negativeness
	angle_setpoint_delta=abs(angle_setpoint_delta);
	
	//3200 steps (LOOK FOR THE STEPPER MODE!) / 180 degrees
	pulse_setpoint=abs(angle_setpoint) * ANGLE_TO_STEPS;
	
}
float* GetInfo(void){
	float info[3]={0, 0 ,0};
	//if((real_mot_pos>=(set_angle-POS_ERR)) && (real_mot_pos<=(set_angle+POS_ERR)))
	info[0] = real_mot_pos;
	info[1] = set_angle;
	info[2] = POS_ERR;
	return info;
	
}

//ISR(TIMER2_OVF_vect){
//TCNT0=256-100;
//if(operate_flag){
//StMotDir();
//if(pulse_count>=pulse_setpoint){
////if the motion is finished but setpoint hasn`t been reached yet
//pulse_count=0;
//float real_mot_pos;
//real_mot_pos=(512.0f-(float)AdcGetPos()[st_mot_chosen-1])*0.290323;
//if((real_mot_pos>=(angle_setpoint-POS_ERR)) && (real_mot_pos<=(angle_setpoint+POS_ERR))) operate_flag=0;
//else StMotCorrectPos();
//}
//else{
////if the motion hasn`t been finished yet
//pulse_count++;
//StMotPul();
//}
//
//}


void StMotGo(){
	//angle_setpoint=angle-current_angle;
	StMotDir(angle_setpoint);
	pulse_setpoint=abs(angle_setpoint) * ANGLE_TO_STEPS;
	
}


void StMotPul(void){
	ST_MOT_PUL_PORT^=(1<<PORT_PUL1);
	ST_MOT_PUL_PORT^=(1<<PORT_PUL2);
	ST_MOT_PUL_PORT^=(1<<PORT_PUL3);
	ST_MOT_PUL_PORT^=(1<<PORT_PUL4);

}

void StMotDir(float direction){

	if (direction >= 0){
		ST_MOT_DIR_PORT|=(1<<PORT_DIR1);
		ST_MOT_DIR_PORT|=(1<<PORT_DIR2);
		ST_MOT_DIR_PORT|=(1<<PORT_DIR3);
		ST_MOT_DIR_PORT|=(1<<PORT_DIR4);
	}
	else{
		ST_MOT_DIR_PORT&=(0<<PORT_DIR1);
		ST_MOT_DIR_PORT&=(0<<PORT_DIR2);
		ST_MOT_DIR_PORT&=(0<<PORT_DIR3);
		ST_MOT_DIR_PORT&=(0<<PORT_DIR4);
	}
}

void SetAngle(float angle){

	//limiting the setpoint +/- 90 degrees

	if(angle<MIN_ANGLE) angle=MIN_ANGLE;
	if(angle>MAX_ANGLE) angle=MAX_ANGLE;
	if ((angle!=current_angle) & (operate_flag == 0))
	{
		set_angle = angle;
		angle_setpoint = angle - current_angle;
		StMotDir(angle_setpoint);
		pulse_setpoint=abs(angle_setpoint) * ANGLE_TO_STEPS;
		operate_flag = 1;
	}

}

float GetMotPos(void){
	float real_mot_pos;
	real_mot_pos=(512.0f-(float)AdcGetPos()[0])*0.262;
	return real_mot_pos;
	
}

//ISR(TIMER2_OVF_vect){
//TCNT1=65535-250; //timer period = 400 us
//if(operate_flag){
//StMotDir();
//if(pulse_count>=pulse_setpoint){
////if the motion is finished but setpoint hasn`t been reached yet
//pulse_count=0;
//float real_mot_pos;
//real_mot_pos=(512.0f-(float)AdcGetPos()[st_mot_chosen-1])*0.290323;
//if((real_mot_pos>=(angle_setpoint-POS_ERR)) && (real_mot_pos<=(angle_setpoint+POS_ERR))) operate_flag=0;
//else StMotCorrectPos();
//}
//else{
////if the motion hasn`t been finished yet
//pulse_count++;
//StMotPul();
//}
//}


//delta = time * f /pre_scaler
//########################################################################################################################

#ifdef SINGLE_MOT //one stepper motor mode

uint8_t operate_flag=0, direction_flag=0;
uint8_t st_mot_chosen=0;
uint16_t pulse_count=0, pulse_setpoint=0;
float angle_setpoint;

void StMotGo(float angle, uint8_t st_mot_num){
	//motors 3 & 4 should rotate in opposite direction to motors 1 & 23

	if((st_mot_num==3)||(st_mot_num==4)) angle_setpoint=-angle;
	//angle_setpoint stores user angle main setpoint
	else angle_setpoint=angle;
	//operate_flag is an indication of motor`s motion
	if(operate_flag==0) st_mot_chosen=st_mot_num;
	//limiting the setpoint +/- 90 degrees
	if(angle<MIN_ANGLE) angle=MIN_ANGLE;
	if(angle>MAX_ANGLE) angle=MAX_ANGLE;
	StMotCorrectPos();
}

void StMotCorrectPos(void){
	//reading real real shaft pos relative to 512
	float real_mot_pos;
	//~300 potentiometer degrees / 1024 = 0.29..
	real_mot_pos=(512.0f-(float)AdcGetPos()[st_mot_chosen-1])*0.290323;
	//position error for the shaft is POS_ERR
	if((real_mot_pos>=(angle_setpoint-POS_ERR)) && (real_mot_pos<=(angle_setpoint+POS_ERR))) operate_flag=0;
	else operate_flag=1;
	//angle_setpoint_delta is a difference between setpoint and real angle of the shaft
	float angle_setpoint_delta=angle_setpoint-real_mot_pos;
	//setting up the direction of rotation according to delta
	if(angle_setpoint_delta<0) direction_flag=1;
	else direction_flag=0;
	//annihilating negativeness
	angle_setpoint_delta=abs(angle_setpoint_delta);
	//3200 steps (LOOK FOR THE STEPPER MODE!) / 180 degrees
	pulse_setpoint=(uint16_t)(angle_setpoint_delta*18);
}


void StMotPul(void){
	#ifdef MODULE

	st_mot_i2c_message^=(1<<(st_mot_chosen+3));

	#endif
	#ifdef DIRECT

	ST_MOT_PUL_PORT^=(1<<(st_mot_chosen-1));

	#endif
}

void StMotDir(void){

	#ifdef DIRECT

	switch(st_mot_chosen){
		case 1:{
			dir_pin=0;
			break;
		}
		case 2:{
			dir_pin=3;
			break;
		}
		case 3:{
			dir_pin=4;
			break;
		}
		case 4:{
			dir_pin=5;
			break;
		}
	}
	if(direction_flag[st_mot_chosen-1]) ST_MOT_DIR_PORT|=(1<<dir_pin);
	else ST_MOT_DIR_PORT&=~(1<<dir_pin);

	#endif
}

ISR(TIMER2_OVF_vect){
	TCNT0=256-100;
	if(operate_flag){
		StMotDir();
		if(pulse_count>=pulse_setpoint){
			//if the motion is finished but setpoint hasn`t been reached yet
			pulse_count=0;
			float real_mot_pos;
			real_mot_pos=(512.0f-(float)AdcGetPos()[st_mot_chosen-1])*0.290323;
			if((real_mot_pos>=(angle_setpoint-POS_ERR)) && (real_mot_pos<=(angle_setpoint+POS_ERR))) operate_flag=0;
			else StMotCorrectPos();
		}
		else{
			//if the motion hasn`t been finished yet
			pulse_count++;
			StMotPul();
		}
	}

}

//test steppers
//ISR(TIMER1_OVF_vect){
//TCNT1=65535-100; //timer period = 400 us
//if (pulse_count < pulse_setpoint)
//{
//ST_MOT_PUL_PORT^=(1<<PORT_PUL1);
//ST_MOT_PUL_PORT^=(1<<PORT_PUL2) ;
//ST_MOT_PUL_PORT^=(1<<PORT_PUL3) ;
//ST_MOT_PUL_PORT^=(1<<PORT_PUL4) ;
//pulse_count++;
//}
//else{
////change dir
//ST_MOT_DIR_PORT^=(1<<PORT_DIR1);
//ST_MOT_DIR_PORT^=(1<<PORT_DIR2);
//ST_MOT_DIR_PORT^=(1<<PORT_DIR3);
//ST_MOT_DIR_PORT^=(1<<PORT_DIR4);
//pulse_count=0;
//
//}
//
////ST_MOT_PUL_PORT^=(1<<(st_mot_chosen-1));
//
//}

#endif

