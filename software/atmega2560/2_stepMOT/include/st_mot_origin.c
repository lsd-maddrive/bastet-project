#include "st_mot.h"

uint8_t st_mot_i2c_message=0;

void StMotTim0Init(void){
	//timer in normal mode
	TCCR2B=(1<<CS22); //prescaler 64
	TCNT2=256-100; //timer period = 400 us
	TIMSK2=(1<<TOIE2);
}

void StMotInit(void){
	StMotTim0Init();
	#ifdef MODULE
	
	I2cInit();

	#endif
	#ifdef DIRECT
	
	ST_MOT_PUL_DDR|=(1<<DD_PUL1)|(1<<DD_PUL2)|(1<<DD_PUL3)|(1<<DD_PUL4);
	ST_MOT_DIR_DDR|=(1<<DD_DIR1)|(1<<DD_DIR2)|(1<<DD_DIR3)|(1<<DD_DIR4);
	
	#endif
}

void StMotI2cSendData(uint8_t val){
	I2cStart();
	I2cTransmitByte((ST_MOT_ADDR<<1) | W);
	I2cTransmitByte(val);
	I2cStop();
}

#ifdef ALL_MOT //4WD mode

uint8_t operate_flag[4]={0,0,0,0}, direction_flag[4]={0,0,0,0};
uint16_t pulse_count[4]={0,0,0,0}, pulse_setpoint[4]={0,0,0,0};
float angle_setpoint[4]={0,0,0,0}, angle_setpoint_delta[4]={0,0,0,0}, real_mot_pos[4]={0,0,0,0}, angle_coeff=0;

void StMotGo(float angle){
	if(angle<MIN_ANGLE) angle=MIN_ANGLE;
	if(angle>MAX_ANGLE) angle=MAX_ANGLE;
	for(uint8_t i=1; i<=4; i++){
		if((i==1)||(i==2)) angle_setpoint[i-1]=-angle;
		else angle_setpoint[i-1]=angle;
		StMotCorrectPos(i);
	}
}

void StMotCorrectPos(uint8_t st_mot_chosen){
	switch(st_mot_chosen){
		case 1:{
			angle_coeff=0.285714; //180 / (0 deg pot - 180 deg pot)
			break;
		}
		case 2:{
			angle_coeff=0.284810; //every pot is different ):
			break;
		}
		case 3:{
			angle_coeff=0.286624;
			break;
		}
		case 4:{
			angle_coeff=0.286169;
			break;
		}
	}
	real_mot_pos[st_mot_chosen-1]=(512.0f-(float)AdcGetPos()[st_mot_chosen-1])*angle_coeff;	
	if((real_mot_pos[st_mot_chosen-1]>=angle_setpoint[st_mot_chosen-1]-POS_ERR) && (real_mot_pos[st_mot_chosen-1]<=angle_setpoint[st_mot_chosen-1]+POS_ERR))
		operate_flag[st_mot_chosen-1]=0;
	else
		operate_flag[st_mot_chosen-1]=1;
	angle_setpoint_delta[st_mot_chosen-1]=angle_setpoint[st_mot_chosen-1]-real_mot_pos[st_mot_chosen-1];
	if(angle_setpoint_delta[st_mot_chosen-1]<0)
		direction_flag[st_mot_chosen-1]=1;
	else
		direction_flag[st_mot_chosen-1]=0;
	angle_setpoint_delta[st_mot_chosen-1]=abs(angle_setpoint_delta[st_mot_chosen-1]);
	pulse_setpoint[st_mot_chosen-1]=(uint16_t)(angle_setpoint_delta[st_mot_chosen-1]*18);
}

void StMotPul(uint8_t st_mot_chosen){
	#ifdef MODULE
	
	st_mot_i2c_message^=(1<<(st_mot_chosen+3));	
	
	#endif
	#ifdef DIRECT
	
	ST_MOT_PUL_PORT^=(1<<(st_mot_chosen-1));	
		
	#endif
}

void StMotDir(uint8_t st_mot_chosen){
	int8_t dir_pin=-1;
	#ifdef MODULE
	
	if(direction_flag[st_mot_chosen-1]) st_mot_i2c_message|=(1<<(st_mot_chosen-1));
	else st_mot_i2c_message&=~(1<<(st_mot_chosen-1));
		
	#endif
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
	TCNT2=256-100;				
	for(uint8_t i=1; i<=4; i++){
		if(operate_flag[i-1]){
			StMotDir(i);
			if(pulse_count[i-1]>=pulse_setpoint[i-1]){
				//if the motion is finished but setpoint hasn`t been reached yet
				pulse_count[i-1]=0;
				real_mot_pos[i-1]=(512.0f-(float)AdcGetPos()[i-1])*angle_coeff;
				if((real_mot_pos[i-1]>=angle_setpoint[i-1]-POS_ERR) && (real_mot_pos[i-1]<=angle_setpoint[i-1]+POS_ERR))
					operate_flag[i-1]=0;
				else
					StMotCorrectPos(i);
			}
			else{
				//if the motion hasn`t been finished yet
				pulse_count[i-1]++;
				StMotPul(i);
			}
		}
	}
	#ifdef MODULE
	
	I2cAcquire();
	StMotI2cSendData(st_mot_i2c_message);
	I2cRelease();
	
	#endif
}

#endif

//########################################################################################################################

#ifdef SINGLE_MOT //one stepper motor mode
	
uint8_t operate_flag=0, direction_flag=0;
uint8_t st_mot_chosen=0;
uint16_t pulse_count=0, pulse_setpoint=0;
float angle_setpoint;

void StMotGo(float angle, uint8_t st_mot_num){
	//motors 3 & 4 should rotate in opposite direction to motors 1 & 2
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
	#ifdef MODULE
	
	if(direction_flag){
		st_mot_i2c_message|=(1<<(st_mot_chosen-1));
	}
	else{
		st_mot_i2c_message&=~(1<<(st_mot_chosen-1));
	}
	
	#endif
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
	#ifdef MODULE
	
	I2cAcquire();
	StMotI2cSendData(st_mot_i2c_message);
	I2cRelease();
	
	#endif
}

#endif