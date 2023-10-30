/*
 * main.c
 *
 * Created: 29.12.2021 18:41:57
 * Author : Michael
 */ 

#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>

void SPI_init(void);
unsigned char Digit(uint16_t d, uint8_t m);
void showMe(int16_t spiData);

uint8_t readADC(uint8_t channel);
void motor_go(uint8_t dir, uint8_t speed);

int main(void)			
{
  //8 ������ ���������� ������ ��������� �������������� ���
  uint8_t result=0;
  //�������� ��� � ��������� ������������ �� 4
  ADCSRA |= (1<<ADEN) | (1<<ADPS1);
  //����� 5 � ���, ����� 4 - �����������
  DDRD |= (1<<4)|(1<<5); 
  /* ��������� �������: ��� � ������� ����������,
  ����������������� �����, ������������ �� 1 */
  TCCR0A = (1<<COM0B1) | (1<<WGM00) | (1<<WGM01);
  TCCR0B = (1<<CS00); 
  SPI_init();
  _delay_ms(200);
  while(1)
  {
	result = readADC(0);
	//motor_go(0, result);
	showMe(result);
  }
}
uint8_t readADC(uint8_t channel)
{
  /* ������� � AVCC; ����� ������������, ����� ������
  ��� - channel (������� ����������) */
  ADMUX |= (1<<REFS0) | (1<<ADLAR) | channel;
  _delay_us(50);
  //������ �������������� ����������� ������� � �����
  ADCSRA |= (1<<ADSC);
  /* ���� ��� ADIF = 0 � �������������� �� ��������,
  ���� ��� ADIF = 1 � �������������� �������� */
  while(!(ADCSRA & (1<<ADIF)));
  //�������� ADIF � �������� � ���� 1
  ADCSRA |= (1<<ADIF);
  //��������� ������ ������������, ������ ������ ADCH
  return ADCH;
}

void motor_go(uint8_t dir, uint8_t speed)
{
	if(dir == 0) //���������� ����������� ��������
		PORTD |= (1<<4);
	else
		PORTD &= ~(1<<4);
	OCR0B = speed; //���������� �������� ��������
}

void SPI_init(void) //������������� ���������� ����� SPI
{
	DDRB |= (1<<0)|(1<<2)|(1<<3)|(1<<5); //������ CS, SS, MOSI, SCK � �� �����
	//�������� ���������� SPI, ����� 0, ������������ �� 128
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1)|(1<<SPR0);
}

/*������� ��� ��������� �� ������������� ����� d ������� m
��������, Digit(14, 3) = 4; ��� Digit(591, 1) = 5 */
unsigned char Digit(uint16_t d, uint8_t m)
{
	uint8_t i = 3, a;
	while(i){ //���� �� �������� �����
		a = d%10; //�������� ��������� ������
		if(i-- == m) break; //������� �������� ������ - ������
		d /= 10; //��������� ����� � 10 ���
	}
	return(a);
}

void showMe(int16_t spiData)
{
	int8_t i;
	uint8_t dig[] = {0,0,0}, j; //������� ����� ���� � dig[]
	uint8_t mas[] = {0x81,0xF3,0x49,0x61,0x33,0x25,0x05,0xF1,0x01,0x21,0xFF,0x7F}; //�0�,..., �9�, ������� ��������, �-�
	uint16_t res; //����� ��������������� ����� � res
	if(spiData < 0) res = -spiData; //������� ���������������
	else res = spiData;
	for(i=1; i<=3; i++) //��������� ����� �� �����
	dig[i-1] = Digit(res,i);
	if(res != 0) //���� ������� �� ���� � ������� �����
	{
		j=0;
		while(dig[j] == 0) //������� ���� (�� 0428, � _428)
		j++;
		/* ������� ���������� ����, ����� - �����, ����� � �������, � ����� ����� � ������� */
		if(spiData < 0)
		{
			SPDR=mas[11]; //������������� ����� - ���� �����
			while(!(SPSR&(1<<SPIF)));
		}
		else
		{
			SPDR=mas[10]; //������������� ����� - ������
			while(!(SPSR&(1<<SPIF)));
		}
		for(i=0; i<=2; i++)
		{
			if(i<j) SPDR=mas[10]; //������ �������� ����� - �����
			else SPDR=mas[dig[i]];
			while(!(SPSR&(1<<SPIF))); //�������� �������� SPDR
		}
	}
	else
	{
		for(i=0; i<=2; i++)
		{
			SPDR=mas[10];
			while(!(SPSR&(1<<SPIF)));
		}
		SPDR=mas[0]; //���� res == 0, ������� �� ����� ___0
		while(!(SPSR&(1<<SPIF)));
	}
	PORTB &= ~(1<<0); //����������� ��������� �����
	_delay_us(20);
	PORTB |= (1<<0);
	_delay_us(20);
	PORTB &= ~(1<<0);
}

