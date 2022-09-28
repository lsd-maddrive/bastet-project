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
#include "uart.h"
#include "dc_motor.h"

void InitAll(void);

//int main(void){
//
//InitAll();
//
//while (1){
//float info=UartReceiveData();
//UartSendDec(info);
//UartTransmitByte('\t');
//uint16_t* dc_mot_enc=GetSpeed();
//for(uint8_t i=0; i<4; i++){
//UartSendDec(dc_mot_enc[i]);
//UartTransmitByte('\t');
//}
//
////¬ывод скорости в терминал
//UartSendDec(GetSpeed());
//UartTransmitByte('\r');
//_delay_ms(20);
//}

int main(void){
	InitAll();
	// задержка дл€ завершени€ процесса программировани€
	DDRB|=(1 << PB7);
	_delay_ms(50);
	float data;
	while (1)
	{
		// получаем данные с терминала 0Е100 Ц скважность Ў»ћ
		data = UartReceiveDec();
		// выставл€ем скорость вращени€ двигател€
		DcMotGo(data);
		if (data > 50) PORTB^=(1 << PB7);
		// табул€ци€ положени€ курсора в терминале
		UartTransmitByte('\t');
		// выводим только что переданное значение
		UartSendDec(data);
		UartTransmitByte('\r');
	}
	
}

void InitAll(void){
	UartInit();
	//I2cInit();
	//ExpInit(EXP_DEF_ADDR);
	//AdcInit();
	DcMotInit();
	//StMotInit();
	//StMotGo(0);
	//_delay_ms(100);
	//sei();
}