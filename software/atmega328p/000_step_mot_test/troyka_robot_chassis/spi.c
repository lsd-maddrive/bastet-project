#include "spi.h"

void SpiMasterInit(void) //инициализация аппаратной части SPI
{
	DDRB|=(1<<MOSI)|(1<<SCK);
	//включить аппаратный SPI, режим 0, предделитель на 4
	SPCR=(1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

void SpiMasterTransmit(uint8_t byte){
	SPDR=byte;
	while(!(SPSR&(1<<SPIF)));
}

uint8_t SpiMasterReceive(void){
	while(!(SPSR&(1<<SPIF)));
	return SPDR;
}