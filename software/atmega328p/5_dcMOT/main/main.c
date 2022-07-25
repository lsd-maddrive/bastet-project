/*
 * main.c
 *
 * Created: 29.12.2021 15:26:16
 * Author : Michael
 */ 

#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>

void motor_go(uint8_t dir, uint8_t speed)
{
	
	if(dir == 0)
	{
		PORTD |= (1<<7); //���������� ����������� ��������	
	}
	else
	{
		PORTD &= ~(1<<7);
	}
	OCR0A = speed; //���������� �������� ��������
	OCR0B = speed; //���������� �������� ��������
	OCR1A = speed; //���������� �������� ��������
	OCR1B = speed; //���������� �������� ��������

}

void init_pwm()
{
  //����� PD5(OC0B), PD6(OC0A) � ���, ����� PD7 - �����������
  DDRD |= (1<<5)|(1<<6)|(1<<7); 
  TCCR0A |= (1<<COM0A1);
  TCCR0A |= (1<<COM0B1);
  //����� PB1(OC1A), PB2(OC1B) � ���
  DDRB |= (1 << PORTB1) | (1 << PORTB2);
  TCCR1A |= (1<<COM1A1);
  TCCR1A |= (1<<COM1B1);
  
  /* TIMER0 - ��������� �������: ������� ���,
  ����������������� �����, ������������ �� 1 */
  TCCR0A |= (1<<WGM00) | (1<<WGM01);
  TCCR0B |= (1<<CS00);
  /* TIMER1 - ��������� �������: ������� ���,
  ����������������� �����, TOP = 0xFF, ������������ �� 1*/
  TCCR1A |= (1 << WGM10);
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1<<CS10);
}


int main(void)
{

  init_pwm();
  
  while(1)
  {
    for(uint8_t i=1; i<255; i++)
	{
		motor_go(0, i);
		_delay_ms(2);	
	}
	for(uint8_t i=1; i<255; i++)
	{
		motor_go(0, 256-i);
		_delay_ms(2);
	}
	for(uint8_t i=1; i<255; i++)
	{
		motor_go(1, i);
		_delay_ms(2);
	}
	for(uint8_t i=1; i<255; i++)
	{
		motor_go(1, 256-i);
		_delay_ms(2);
	}

  }
}


