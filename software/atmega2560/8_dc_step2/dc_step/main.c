#include "st_mot.h"
#include "dc_motor.h"
#include "utils.h"
#include "uart.h"

void InitAll(void);
void GoRobot(int16_t speed);
void Cmd(uint16_t speed);
void ShowAngles();
void ShowSpeeds();


int main(void)
{
	InitAll();
	float data = 0;
	while (1)
	{
		data = UartReceiveData();
		//if ((data >=-90) & (data <=90)) SetAngle(data);
		//else if (data == 1000) SetSpeed(0);
		//else SetSpeed(data);
		SetAngle(data);
		ShowAngles();
		//ShowSpeeds();


	}
}

void InitAll(void){
	UartInit();
	DDRB|=(1<<7);
	AdcInit();
	StMotInit();
	DcMotInit();
	//SetAngle(1);
	//SetSpeed(0);
	_delay_ms(100);
	sei();
}
void GoRobot(int16_t speed){
	SetSpeed(speed);
	_delay_ms(1000);
	SetSpeed(0);
	_delay_ms(500);
}

void ShowAngles(){
	UartTransmitByte('\t');
	for (int i = 0; i < 4; i++){
		UartSendDec(GetMotPos(i));
		UartTransmitByte('\t');
	}
	UartTransmitByte('\r');
	
}
void ShowSpeeds(){

	UartTransmitByte('\t');
	for (int i = 0; i < 4; i++){
		UartSendDec(GetSpeed()[i]);
		UartTransmitByte('\t');
	}
	UartTransmitByte('\r');
}

void Cmd(uint16_t speed){
	uint16_t robot_speed = speed;
	
	_delay_ms(1000);
	SetAngle(1);
	_delay_ms(1000);
	GoRobot(robot_speed);
	_delay_ms(1000);
	SetAngle(90);
	_delay_ms(1000);
	GoRobot(robot_speed);
	_delay_ms(1000);
	SetAngle(0);
	_delay_ms(1000);
	GoRobot(-robot_speed);
	_delay_ms(1000);
	
	SetAngle(-90);
	_delay_ms(1000);
	GoRobot(robot_speed);
	_delay_ms(1000);

}
