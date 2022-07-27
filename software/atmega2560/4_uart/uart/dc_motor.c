#include "dc_motor.h"
#include <avr/io.h>

//uint16_t tim2_count=0, dc_mot_enc_count, enc_result;
uint16_t tim2_count=0, dc_mot_enc_count[4]={0,0,0,0}, enc_result[4]={0,0,0,0};
uint8_t prev_state=0, current_state=0;

void Tim2DcMotInit(void){
	//вывод PH6(OC2B), PB4(OC2A) – ШИМ, вывод PH4 - направление
	TCCR2A |= (1<<COM2A1);
	TCCR2A |= (1<<COM2B1);
	//вывод PH3(OC4A), PH5(OC4C) – ШИМ
	TCCR4A |= (1<<COM4A1);
	TCCR4A |= (1<<COM4C1);
	/* TIMER2 - настройка таймера: быстрый ШИМ,
	неинвертированный режим, предделитель на 1 */
	TCCR2A |= (1<<WGM20) | (1<<WGM21);
	TCCR2B |= (1<<CS20);
	/* TIMER4 - настройка таймера: быстрый ШИМ,
	неинвертированный режим, 8 bit, TOP = 0xFF, предделитель на 1*/
	TCCR4A |= (1 << WGM40);
	TCCR4B |= (1 << WGM42);
	TCCR4B |= (1<<CS40);
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

void IntDcMotEcoderInit(void){
	//PCICR=(1<<PCIE2);
	//PCMSK2=(1<<PCINT20)|(1<<PCINT21)|(1<<PCINT22)|(1<<PCINT23);
}

void DcMotGo(float speed){
	if(speed>=0) DC_MOT_FOR;
	else DC_MOT_REV;
	//OCR0B=(uint8_t)((abs(speed)*255)/100);
	OCR2A = speed;
	OCR2B = speed; 
	OCR4A = speed; 
	OCR4C = speed;
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


// Определение скорости по внешним прерываниям

// uint16_t speed = 0, dc_mot_enc_count = 0;

//void IntDcMotEcoderInit(void){
//EICRA=(1<<ISC00);
//EIMSK=(1<<INT0);
//}

//uint16_t GetSpeed(void){
//return(speed);
//}

//ISR(INT0_vect){
//dc_mot_enc_count++;
//}

//ISR(TIMER0_OVF_vect){ //isr executes every 8 ms
//if(tim2_count<25) tim2_count++; //every 200 ms
//else{
// speed = dc_mot_enc_count / something;
// dc_mot_enc_count = 0;
//}
//tim2_count=0;
//}
//}