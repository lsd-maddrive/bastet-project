/*
 * led_test.c
 *
 * Created: 03.03.2022 13:42:44
 * Author : Michael
 */ 

#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>


int main(void)
{
    DDRB|=(1<<5);
    while (1) 
    {
		PORTB^=(1<<5);
		_delay_ms(500);
    }
}

