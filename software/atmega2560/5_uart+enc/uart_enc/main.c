#define MAIN_ROUTINE 0
#define TEST1 1
#define PROGRAM_ROUTINE TEST1

#include "dc_motor.h"
#include "utils.h"
#include "uart.h"

void InitAll(void);

int main(void){
	
	InitAll();
	_delay_ms(50);
	float data;
	//��������� ����� �������	
	while (1)
	{
		//// �������� ������ � ��������� 0�100 � ���������� ���
		data = UartReceiveDec();
		//// ���������� �������� �������� ���������
		DcMotGo(data);
		// ��������� ��������� ������� � ���������
		UartTransmitByte('\t');
		// ������� ������ ��� ���������� ��������
		//UartSendDec(data);
		UartSendDec(GetSpeed());
		UartTransmitByte('\r');
	}
}

void InitAll(void){
	UartInit();
	//I2cInit();
	//ExpInit(EXP_DEF_ADDR);
	//AdcInit();
	DcMotInit();
	DDRB|=(1<<7);
	//StMotInit();
	//StMotGo(0);
	//_delay_ms(100);
	sei();
}


// ����������� �������� �� ������� �����������

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