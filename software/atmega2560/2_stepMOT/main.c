#include "st_mot.h"
#include "utils.h"
#include "uart.h"

void InitAll(void);

int main(void)
{
	InitAll();
	float angle = 0;
	while (1)
	{
		//�������� ������ � ���������
		angle = UartReceiveData();
		// ���������� �������� �������� ���������
		SetAngle(angle);
		// ��������� ��������� ������� � ���������
		UartTransmitByte('\t');
		// ������� ������ ��� ���������� ��������
		for (int i = 0; i < 3; i++){
			UartSendDec(GetInfo()[i]);
			UartTransmitByte('\t');
		}
		
		//UartSendDec(AdcGetPos()[0]);
		//UartTransmitByte('\t');
		//UartSendDec(GetMotPos());
		
		
		
		//UartTransmitByte('\t');
		//error = data - GetSpeed();
		//UartSendDec(error);
		
		UartTransmitByte('\r');
	}
}

void InitAll(void){
	UartInit();
	DDRB|=(1<<7);
	DDRF|=(1<<5);
	AdcInit();
	StMotInit();
	//StMotGo(0);
	_delay_ms(100);
	sei();
}