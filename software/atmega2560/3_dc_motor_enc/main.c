#define MAIN_ROUTINE 0
#define TEST1 1
#define PROGRAM_ROUTINE TEST1

#include "dc_motor.h"
#include "utils.h"

void InitAll(void);
uint8_t angle;

int main(void){
	InitAll();
	EICRA=(1<<ISC01);
	EIMSK=(1<<INT0);
	//настройка формы сигнала
	sei();
	
	while (1)
	{
		DcMotGo(angle);
	}
}

void InitAll(void){
	//UartInit();
	//I2cInit();
	//ExpInit(EXP_DEF_ADDR);
	//AdcInit();
	DcMotInit();
	DDRB|=(1<<7);
	//StMotInit();
	//StMotGo(0);
	//_delay_ms(100);
	//sei();
}

ISR (INT0_vect)
{
	PORTB^=(1<<7);
	angle+=1;
}

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