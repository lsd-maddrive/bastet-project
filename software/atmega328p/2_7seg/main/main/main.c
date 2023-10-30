/*
 * main.c
 *
 * Created: 27.12.2021 15:35:40
 * Author : Michael
 */ 

#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>

void SPI_init(void); 
unsigned char Digit(uint16_t d, uint8_t m);
void showMe(int16_t spiData);

int main(void)
{
	SPI_init();
	_delay_ms(200);	
	//����� AbEd
	SPDR=0b00010001; //A
	while(!(SPSR&(1<<SPIF)));
	SPDR=0b00000111; //b
	while(!(SPSR&(1<<SPIF)));
	SPDR=0b00001101; //E
	while(!(SPSR&(1<<SPIF)));
	SPDR=0b01000011; //d
	while(!(SPSR&(1<<SPIF)));
	_delay_ms(1000);
	while (1)
    {
		for(int8_t i=-99; i<=99; i++)
		{
			showMe(i);
			_delay_ms(125);
		}
		for(int8_t i=99; i>=-99; i--)
		{
			showMe(i);
			_delay_ms(125);
		}
    }
}

void SPI_init(void) //������������� ���������� ����� SPI
{
	DDRB |= (1<<2)|(1<<3)|(1<<5); //������ SS, MOSI, SCK � �� �����
	//muu - ������ ����
	DDRD |= (1<<0); //����� CS � �� �����
	//�������� ���������� SPI, ����� 0, ������������ �� 128
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1)|(1<<SPR0);
	PORTB &= ~(1<<3);
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
	PORTD &= ~(1<<0); //����������� ��������� �����
	_delay_us(20);
	PORTD |= (1<<0);
	_delay_us(20);
	PORTD &= ~(1<<0);
}
