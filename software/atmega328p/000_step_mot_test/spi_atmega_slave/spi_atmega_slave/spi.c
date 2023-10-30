#include "spi.h"

void spi_slave_init(void) //инициализация аппаратной части SPI
{
	DDRB|=(1<<MISO); //miso
	SPCR=(1<<SPE);
}

void spi_slave_transmit(uint8_t byte){
	SPDR=byte;
	while(!(SPSR&(1<<SPIF)));
}

uint8_t spi_slave_receive(void){
	while(!(SPSR&(1<<SPIF)));
	return SPDR;
}