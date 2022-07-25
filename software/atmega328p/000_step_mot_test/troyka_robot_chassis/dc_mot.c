#include "dc_mot.h"

//uint16_t tim2_count=0, dc_mot_enc_count, enc_result;
uint16_t tim2_count=0, dc_mot_enc_count[4]={0,0,0,0}, enc_result[4]={0,0,0,0}; 
uint8_t prev_state=0, current_state=0; 

void Tim2DcMotInit(void){
	TCCR0A=(1<<COM0A1)|(1<<COM0B1)|(1<<WGM00); //phase correct PWM
	TCCR0B=(1<<CS02); //256 prescaler
	//TIMSK0=(1<<TOIE0);
}

void DcMotInit(void){
	Tim2DcMotInit();
	IntDcMotEcoderInit();
	DC_MOT_SPEED_DDR|=(1<<DC_MOT_SPEED_DDR_PIN);
	DC_MOT_DIR_DDR|=(1<<DC_MOT_DIR_DDR_PIN);
}

void IntDcMotEcoderInit(void){
	//PCICR=(1<<PCIE2);
	//PCMSK2=(1<<PCINT20)|(1<<PCINT21)|(1<<PCINT22)|(1<<PCINT23);
}

void DcMotGo(float speed){
	if(speed>=0) DC_MOT_FOR;
	else DC_MOT_REV;
	OCR0B=(uint8_t)((abs(speed)*255)/100);
}

uint16_t* GetSpeed(void){
	return(enc_result); 
}

ISR(TIMER0_OVF_vect){ //isr executes every 8 ms
	if(tim2_count<25) tim2_count++; //every 200 ms
	else{
		for(uint8_t i=0; i<4; i++){
			enc_result[i]=(dc_mot_enc_count[i]*5*60)/220; //rev per minute
			dc_mot_enc_count[i]=0;
		}
		tim2_count=0;
  	}
}

ISR(PCINT2_vect){
	current_state=PIND;
	if((prev_state&(1<<4))^(current_state&(1<<4))) dc_mot_enc_count[0]++;
	if((prev_state&(1<<5))^(current_state&(1<<5))) dc_mot_enc_count[1]++;
	if((prev_state&(1<<6))^(current_state&(1<<6))) dc_mot_enc_count[2]++;
	if((prev_state&(1<<7))^(current_state&(1<<7))) dc_mot_enc_count[3]++;
	prev_state=current_state;
}

//void IntDcMotEcoderInit(void){
	//EICRA=(1<<ISC00);
	//EIMSK=(1<<INT0);
//}

//uint16_t GetSpeed(void){
	//return((enc_result*10*60)/220);
//}

//void SetSpeed(uint16_t dc_mot_enc_count){
	//enc_result=dc_mot_enc_count;
//}

//ISR(TIMER2_OVF_vect){ //isr executes every 8 ms
	//if(tim2_count<12) tim2_count++; //every 100 ms
	//else{
		//SetSpeed(dc_mot_enc_count);
		//dc_mot_enc_count=0;
		//tim2_count=0;
	//}
//}

//ISR(INT0_vect){
	//dc_mot_enc_count++;
//}