/*
 * main.c
 *
 * Created: 29.12.2021 13:57:20
 * Author : Michael
 */ 

#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>

void st_mot_go(uint8_t dir, uint8_t speed, uint8_t steps);
void my_delay_ms(uint8_t value);

int main(void)
{
    DDRD = 0xF0; //Ўƒ подключаетс€ к PD4ЕPD7
	while(1)
	{
		uint8_t mode[] = {0b0011, 0b1100, 0b0010, 0b0100};
		{
			//sizeof() возвращает размер переменной в байтах
			for(uint8_t i = 0; i < sizeof(mode); i++)
			{
				PORTD = mode[i]<<4;
				_delay_ms(1);
			}
		}
	}
	//while (1) 
    //{
		//for(uint8_t i=1; i<=10; i++)
			//st_mot_go(1, i, 15);
    //}
}

void st_mot_go(uint8_t dir, uint8_t speed, uint8_t steps)
{
	uint8_t i, j;
	//двухфазный полношаговый режим
	uint8_t mode[] = {0b1111, 0b1110, 0b0110, 0b0111};
	DDRD = 0xF0; //Ўƒ подключаетс€ к PD4ЕPD7
	while (1)
	{
		for(j = 0; j < steps; j++)
		{
			//sizeof() возвращает размер переменной в байтах
			for(i = 0; i < sizeof(mode); i++)
			{
				if(dir == 0) PORTD = (mode[i]<<4);
				else PORTD = (mode[(sizeof(mode)-1)-i]<<4);
				/* собственноручно написанна€ функци€ задержки с переменным входным значением (speed = 1Е10, 1 Ц медленно, 10 Ц быстро) */
				my_delay_ms(11-speed);
			}
		}
	}
}

void my_delay_ms(uint8_t value)
{
	for(uint8_t i = 0; i < value; i++)
		_delay_ms(1);
}


