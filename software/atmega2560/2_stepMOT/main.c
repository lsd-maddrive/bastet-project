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
		//�������� ������ � ��������� 0�100 � ���������� ���
		angle = UartReceiveData();
		// ���������� �������� �������� ���������
		SetAngle(angle);
		// ��������� ��������� ������� � ���������
		UartTransmitByte('\t');
		// ������� ������ ��� ���������� ��������
		UartSendDec(GetCount());
		
		//UartTransmitByte('\t');
		//error = data - GetSpeed();
		//UartSendDec(error);
		
		UartTransmitByte('\r');
		//PORTB^=(1<<7);
		//_delay_ms(500);
	}
}

void InitAll(void){
	UartInit();
	DDRB|=(1<<7);
	DDRF|=(1<<5);
	//AdcInit();
	StMotInit();
	//StMotGo(0);
	_delay_ms(100);
	sei();
}