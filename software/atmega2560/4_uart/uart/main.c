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
////����� �������� � ��������
//UartSendDec(GetSpeed());
//UartTransmitByte('\r');
//_delay_ms(20);
//}

void uart_init(void){
	UCSR0B |= (1<<RXEN0) | (1<<TXEN0);
	UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);
	//�������� �������� 9600 ���
	UBRR0H = 0;
	//��� �������� 9600 UBRR0L = 103 = 0x67
	UBRR0L = 0x67;
}

// ������� �������� �����
void uart_transmit(uint8_t byte){
	/* ������� ����������� ���� UDRE0, � ����� ������
	������� � UDR0 */
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = byte;
}

// ������� ����� �����
uint8_t uart_receive(void){
	/* ������� ����������� ���� RXC0, � ����� ��������
	������� �� UDR0 */
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
}

// ������� �������� ������, ����� ��� �����
void uart_transmit_str(uint8_t* string){
	/* ������� ������ ��������� � ������� ��������� *
	��������� � ����� �������� �������� ������������ ������*/
	while(*string)
	uart_transmit(*string++);
}

//uint8_t Digit(uint16_t d, uint8_t m){
	//uint8_t i = 5, a;
	//while(i){ //���� �� �������� �����
		//a = d%10; //�������� ��������� ������
		////������� �������� ������ - ������
		//if(i-- == m) break;
		//d /= 10; //��������� ����� � 10 ���
	//}
	//return(a);
//}
// ������� ����������� 16-������� ����� � �� � ��������
void uart_transmit_dec(uint16_t val){
	uint8_t i, j;
	// ���� ���������� �� ����� �� ����
	if(val!=0)
	{
		/* ��������� ���������� ������� �������� �
		������������� �����, �������� � ����� 64 ����� ���
		������� ���������� ������� � 00064 */
		j=1;
		while(!(Digit(val, j++)));
		// �� ������� ���������� ���� � ���������
		for(i=j-1; i<=5; i++) uart_transmit(Digit(val, i)+'0');
	}
	/* ���� �������� ���� � ������� ����, ����� ��������
	� ����� */
	else
	uart_transmit('0');
}

// ���� ����������� 16-������� ����� �� �� � ���������
uint16_t uart_receive_dec(void){
	uint16_t data = 0;
	uint8_t digit = 0;
	// ��������� ������������� �����
	do
	{
		digit = uart_receive();
		// ���������, ���� �� ������ ������� ����� ������
		if(digit == '\r') continue;
		if(digit == '\n') break;
		if(!((digit >= '0') && (digit <= '9'))) break;
		// ������� ASCII ���� ����
		digit -= '0';
		data *= 10;
		data += digit;
	}
	// �����������, ����� ������� ������� \r\n
	while(1);
	return data;
}

int main(void){
	InitAll();
	// �������� ��� ���������� �������� ����������������
	_delay_ms(50);
	uint8_t data, word[] = {"TEST"};
	// ���������� ���� � ��� ������ � ������� �1
	//DDRH |= (1<<6);
	// ����������� ���
	//TCCR2A = (1<<COM2B1) | (1<<WGM21) | (1<<WGM20);
	//TCCR2B = (1<<CS20);
	uart_init();
	// ������� � �������� ����� TEST
	uart_transmit_str(word);
	// ��������� � ������ ��������� ������
	uart_transmit('\r');
	while (1)
	{
		// �������� ������ � ��������� 0�255 � ���������� ���
		data = uart_receive_dec();
		// ���������� ������� �������� ����������
		DcMotGo(data);
		// ��������� ��������� ������� � ���������
		uart_transmit('\t');
		// ������� ������ ��� ���������� ��������
		uart_transmit_dec(data);
		uart_transmit('\r');
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