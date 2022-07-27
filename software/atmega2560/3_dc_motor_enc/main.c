#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>

void motor_go(uint8_t dir, uint8_t speed)
{
	
	if(dir == 0)
	{
		PORTH |= (1<<4); //определяем направление вращения	
	}
	else
	{
		PORTH &= ~(1<<4);
	}
	OCR2A = speed; //определяем скорость вращения
	OCR2B = speed; //определяем скорость вращения
	OCR4A = speed; //определяем скорость вращения
	OCR4C = speed; //определяем скорость вращения

}

void init_pwm()
{
  //вывод PH6(OC2B), PB4(OC2A) – ШИМ, вывод PH4 - направление
  DDRH |= (1<<4) | (1<<6);
  DDRB |= (1<<4);
  TCCR2A |= (1<<COM2A1);
  TCCR2A |= (1<<COM2B1);
  //вывод PH3(OC4A), PH5(OC4C) – ШИМ
  DDRH |= (1 << PORTH3) | (1 << PORTH5);
  TCCR4A |= (1<<COM4A1);
  TCCR4A |= (1<<COM4C1);
  
  /* TIMER2 - настройка таймера: быстрый ШИМ,
  неинвертированный режим, предделитель на 1 */
  TCCR2A |= (1<<WGM20) | (1<<WGM21);
  TCCR2B |= (1<<CS20);
  
  /* TIMER4 - настройка таймера: быстрый ШИМ,
  неинвертированный режим, 8 bit, TOP = 0xFF, предделитель на 1*/
  TCCR4A |= (1 << WGM40);
  TCCR4B |= (1 << WGM42);
  TCCR4B |= (1<<CS40);
  
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
