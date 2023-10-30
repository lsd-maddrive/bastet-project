/*
 * troyka_robot_chassis.c
 * Working with 4 stepper motors, 4 potentiometers, 4 DC motors and 3-DOF servo arm.
 *
 * Created: 11.03.2022 12:13:32
 * Author : Michael
 */ 

#define MAIN_ROUTINE 0
#define TEST1 1

#define PROGRAM_ROUTINE TEST1
#include "tests.h"
#include "utils.h"
#include "uart.h"
#include "i2c_basics.h"
#include "gpio_expander.h"
#include "adc.h"
#include "dc_mot.h"
#include "st_mot.h"



void test_fun(){
#if(PROGRAM_ROUTINE == TEST1)
	test1();
#endif

}

void InitAll(void);

int main(void){
	
#if(PROGRAM_ROUTINE != MAIN_ROUTINE)
	test_fun();
#else

	InitAll();
	while (1){
		float info=UartReceiveData();
		UartSendDec(info);
		UartTransmitByte('\t');
		if((info>=-90) && (info<=90)) StMotGo(info);
		//else if((info<=-100) || (info>=100)){
			//if(info<=-100) DcMotGo(info+100);
			//else if(info>=100) DcMotGo(info-100);
		//}
		uint16_t* st_mot_adc=AdcGetPos();
		for(uint8_t i=0; i<4; i++){
			UartSendDec(st_mot_adc[i]);
			UartTransmitByte('\t');
		}
		//uint16_t* dc_mot_enc=GetSpeed();
		//for(uint8_t i=0; i<4; i++){
			//UartSendDec(dc_mot_enc[i]);
			//UartTransmitByte('\t');
		//}
		
		// Вывод скорости в терминал
		// UartSendDec(GetSpeed());
		
		UartTransmitByte('\r');
		_delay_ms(20);
	}
#endif
}

void InitAll(void){
	UartInit();
	I2cInit();
	ExpInit(EXP_DEF_ADDR);
	AdcInit();
	DcMotInit();
	StMotInit();
	StMotGo(0);
	_delay_ms(100);
	sei();
}