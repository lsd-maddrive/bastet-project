#include "include/st_mot.h"
#include "math.h"
#ifdef ALL_MOT //4WD mode
uint8_t operate_flag[4]={0, 0, 0, 0};
uint8_t st_mot_chosen=0;
uint8_t	operate_master_flag=0;

uint16_t pulse_count[4]={0,0,0,0}, pulse_setpoint[4]={0,0,0,0};
float angle_setpoint[4]={0,0,0,0}, current_angle[4]={0,0,0,0}, set_angle[4]={0,0,0,0};
float real_mot_pos = 0;
uint16_t set_counter =0;
uint8_t len_wheel =34;
uint8_t wid_wheel =34;

//pot_koefs
float p_k[4]={0.268,0.268,0.268,0.268};
float info[3]={0, 0 ,0};




void StMotTim1Init(void){
	////timer in normal mode
	TCCR1B|=(1<<CS11) | (1<<CS10); //prescaler 64
	//TCCR1B|=(1<<CS12); //prescaler 256
	
	TCNT1=65535-100; //timer period = 400 us, delta = 100
	TIMSK1|=(1<<TOIE1);
}

void StMotInit(void){
	StMotTim1Init();
	
	ST_MOT_PUL_DDR|=(1<<DD_PUL1)|(1<<DD_PUL2)|(1<<DD_PUL3)|(1<<DD_PUL4);
	ST_MOT_DIR_DDR|=(1<<DD_DIR1)|(1<<DD_DIR2)|(1<<DD_DIR3)|(1<<DD_DIR4);
	//ST_MOT_PUL_DDR|=(1<<DD_PUL1);
	//ST_MOT_DIR_DDR|=(1<<DD_DIR1);

}

ISR(TIMER1_OVF_vect){
	TCNT1=65535-100; //timer period = 400 us
	for(int n = 0; n<4; n++){
		if (operate_flag[n])
		{
			if (pulse_count[n] < pulse_setpoint[n]){
				StMotPul(n);
				pulse_count[n]++;
				
			}
			else
			{
				float real_mot_pos;
				real_mot_pos = GetMotPos(n);
				StMotCorrectPos(n, real_mot_pos);
			}
		}
	}
}


void StMotCorrectPos(uint8_t n, float real_mot_pos){
	if((real_mot_pos>=(set_angle[n]-POS_ERR)) && (real_mot_pos<=(set_angle[n]+POS_ERR)))
	{
		operate_flag[n]=0;
		current_angle[n]=set_angle[n];
		pulse_count[n]=0;
		
	}
	else
	{
		float angle_setpoint_delta=set_angle[n]-real_mot_pos;
		StMotDir(angle_setpoint_delta, n);
		pulse_setpoint[n]=abs(angle_setpoint_delta) * ANGLE_TO_STEPS;
		operate_flag[n]=1;
		pulse_count[n]=0;
	}
	
}
float* GetInfo(void){
	//info[0] = set_angle;
	//info[1] = POS_ERR;
	////info[2] = operate_flag;
	//return info;
}



void StMotPul(uint8_t n){
	//PUL pin are 0 2 4 6
	uint8_t pinns[4]={PORT_PUL1,PORT_PUL2,PORT_PUL3,PORT_PUL4};
	ST_MOT_PUL_PORT^=(1<<pinns[n]);
}

void StMotDir(float direction, uint8_t n){
	uint8_t port_num[4]={PORT_DIR1,PORT_DIR2,PORT_DIR3,PORT_DIR4};
	if (direction >= 0){
		ST_MOT_DIR_PORT|=(1<<port_num[n]);
	}
	else{
		ST_MOT_DIR_PORT&=~(1<<port_num[n]);
	}
}

void SetAngle(float angle){
	float angle_l=atan((len_wheel*tan(0.01745*angle))/(len_wheel+0.5*wid_wheel*tan(0.01745*angle)));
	float angle_r=atan((len_wheel*tan(0.01745*angle))/(len_wheel-0.5*wid_wheel*tan(0.01745*angle)));
	float angles[4]={0, 57.3*angle_r, 57.3*angle_l, 0};
	if(angle<MIN_ANGLE) angle=MIN_ANGLE;
	if(angle>MAX_ANGLE) angle=MAX_ANGLE;
	operate_master_flag = operate_flag[0] | operate_flag[1] | operate_flag[2] | operate_flag[3];
	//operate_master_flag = operate_flag[0] | operate_flag[1];
	if ((angle!=current_angle[0]) & (operate_master_flag==0))
	{
		
		for (int i=0; i<4; i++)
		{
			// if (i==2) angle=-angle;
			set_angle[i] = angles[i];
			angle_setpoint[i] = angles[i] - current_angle[i];
			StMotDir(angle_setpoint[i], i);
			pulse_setpoint[i]=abs(angle_setpoint[i]) * ANGLE_TO_STEPS;
			operate_flag[i] = 1;
		}
		

		
		//for (int i=0; i<2; i++)
		//{
		//set_angle[i] = -angle;
		//angle_setpoint[i] = -angle - current_angle[i];
		//StMotDir(angle_setpoint[i], i);
		//pulse_setpoint[i]=abs(angle_setpoint[i]) * ANGLE_TO_STEPS;
		//operate_flag[i] = 1;
		//}
	}

}

float GetMotPos(uint8_t n){
	float real_mot_pos;
	real_mot_pos=(512.0f-(float)AdcGetPos()[n])*p_k[n];
	return -real_mot_pos;
	
}

#endif


