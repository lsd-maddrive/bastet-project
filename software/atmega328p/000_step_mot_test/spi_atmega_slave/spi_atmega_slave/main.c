/*
 * spi_atmega_slave.c
 *
 * Created: 14.03.2022 19:01:56
 * Author : Michael
 */ 

#include "utils.h"
#include "spi.h"

int main(void)
{
	spi_slave_init();
    DDRD|=(1<<7);
	while (1) 
    {
		uint8_t var=spi_slave_receive();
		if(var!=1) PORTD=1<<7;
		else PORTD=0x00;
    }
}

