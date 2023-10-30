#include "i2c_basics.h"

void i2c_init(void){
	TWBR=12; //400 kHz
}

void i2c_start(void){
	TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while(!(TWCR&(1<<TWINT)));
}

void i2c_stop(void){
	TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}

void i2c_transmit_byte(uint8_t data){
	TWDR=data;
	TWCR=(1<<TWINT)|(1<<TWEN);
	while(!(TWCR&(1<<TWINT)));
}

uint8_t i2c_receive_byte(uint8_t is_last_byte){
	if(is_last_byte) TWCR=(1<<TWINT)|(1<<TWEN);
	else TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while(!(TWCR&(1<<TWINT)));
	return(TWDR);
}