#include "st_mot.h"
#include "dc_motor.h"
#include "utils.h"
#include "uart.h"

void InitAll(void);

int main(void)
{
	InitAll();
	float data = 0;
	while (1)
	{
		//получаем данные с терминала
		data = UartReceiveData();
		// выставляем скорость вращения двигателя
		//if ((data >=-90) & (data <=90)) SetAngle(data);
		//else if (data == 1000) SetSpeed(0);
		//else SetSpeed(data);
		
		SetSpeed(data);
		UartTransmitByte('\t');
		UartSendDec(data);
		UartTransmitByte('\t');
					
		// выводим только что переданное значение
		//for (int i = 0; i < 4; i++){
		//UartSendDec(GetMotPos(i));
		//UartTransmitByte('\t');
		//}
		
		
		for (int i = 0; i < 4; i++){
			UartSendDec(data-GetSpeed()[i]);
			UartTransmitByte('\t');
		}
		
		UartTransmitByte('\r');
	}
}

void InitAll(void){
	UartInit();
	DDRF|=(1<<5);
	AdcInit();
	StMotInit();
	DcMotInit();
	//StMotGo(0);
	_delay_ms(100);
	sei();
}