#include "gpio_expander.h"

void ExpWriteCmd(uint8_t slave_addr, uint8_t cmd){
	I2cStart();
	I2cTransmitByte((slave_addr<<1) | W);
	I2cTransmitByte(cmd);
	I2cStop();
}

void ExpWriteCmdPin(uint8_t slave_addr, uint8_t cmd, uint8_t pin){
	I2cStart();
	I2cTransmitByte((slave_addr<<1) | W);
	I2cTransmitByte(cmd);
	I2cTransmitByte(pin);
	I2cStop();
}

void ExpWriteCmd8BitVal(uint8_t slave_addr, uint8_t cmd, uint8_t val){
	I2cStart();
	I2cTransmitByte((slave_addr<<1) | W);
	I2cTransmitByte(cmd);
	I2cTransmitByte(val);
	I2cStop();
}

void ExpWriteCmd16BitVal(uint8_t slave_addr, uint8_t cmd, uint16_t val){
	uint8_t msb_val=(val>>8)&0xFF, lsb_val=val&0xFF;
	I2cStart();
	I2cTransmitByte((slave_addr<<1) | W);
	I2cTransmitByte(cmd);
	I2cTransmitByte(msb_val);
	I2cTransmitByte(lsb_val);
	I2cStop();
}

void ExpWriteCmdPin16BitVal(uint8_t slave_addr, uint8_t cmd, uint8_t pin, uint16_t val){
	uint8_t msb_val=(val>>8)&0xFF, lsb_val=val&0xFF;
	I2cStart();
	I2cTransmitByte((slave_addr<<1) | W);
	I2cTransmitByte(cmd);
	I2cTransmitByte(pin);
	I2cTransmitByte(msb_val);
	I2cTransmitByte(lsb_val);
	I2cStop();
}

uint16_t ExpReadCmd(uint8_t slave_addr, uint8_t cmd){
	uint16_t result=0;
	I2cStart();
	I2cTransmitByte((slave_addr<<1) | W);
	I2cTransmitByte(cmd);
	I2cStart();
	I2cTransmitByte((slave_addr<<1) | R);
	result=I2cReceiveByte(0);
	result<<=8;
	result|=I2cReceiveByte(1);
	I2cStop();
	return(result);
}

int16_t ExpRead16Bit(uint8_t slave_addr){
	int16_t result=-1;
	I2cStart();
	I2cTransmitByte((slave_addr<<1) | R);
	result=I2cReceiveByte(0);
	result<<=8;
	result|=I2cReceiveByte(1);
	I2cStop();
	return(result);
}

void ExpReset(uint8_t slave_addr){
	ExpWriteCmd(slave_addr, EXP_RESET);
}

void ExpInit(uint8_t slave_addr){
	ExpReset(slave_addr);
}

void ExpPinMode(uint8_t slave_addr, uint8_t pin, uint8_t mode){
	uint16_t chosen_pin=(1<<pin);
	ExpPinModePort(slave_addr, chosen_pin, mode);
}

void ExpPinModePort(uint8_t slave_addr, uint16_t chosen_pin, uint8_t mode){
	if (mode==INPUT){
		ExpWriteCmd16BitVal(slave_addr, EXP_PORT_MODE_INPUT, chosen_pin);
	}else if (mode==OUTPUT){
		ExpWriteCmd16BitVal(slave_addr, EXP_PORT_MODE_OUTPUT, chosen_pin);
	}else if (mode==INPUT_PULLUP){
		ExpWriteCmd16BitVal(slave_addr, EXP_PORT_MODE_PULLUP, chosen_pin);
	}else if (mode==INPUT_PULLDOWN){
		ExpWriteCmd16BitVal(slave_addr, EXP_PORT_MODE_PULLDOWN, chosen_pin);
	}
}

void ExpDigitalWrite(uint8_t slave_addr, uint8_t pin, uint8_t val){
	ExpPinMode(slave_addr, pin, OUTPUT);
	uint16_t chosen_pin=(1<<pin);
	if(val) ExpWriteCmd16BitVal(slave_addr, EXP_DIGITAL_WRITE_HIGH, chosen_pin);
	else ExpWriteCmd16BitVal(slave_addr, EXP_DIGITAL_WRITE_LOW, chosen_pin);
}

uint8_t ExpDigitalRead(uint8_t slave_addr, uint8_t pin){
	ExpPinMode(slave_addr, pin, INPUT_PULLUP);
	uint16_t result=ExpReadCmd(slave_addr, EXP_DIGITAL_READ);
	if(result>=0) (result=(result&(1<<pin)) ? 1 : 0);
	return(result);	
}

uint16_t ExpMapRes(uint16_t value, uint8_t from, uint8_t to){
	if (from==to) return(value);
	if (from>to) return(value>>(from-to));
	return(value<<(to-from));
}

void ExpSetPwmFreq(uint8_t slave_addr, uint16_t freq){
	ExpWriteCmd16BitVal(EXP_DEF_ADDR, EXP_PWM_FREQ, freq);
}

void ExpAnalogWrite(uint8_t slave_addr, uint8_t pin, uint16_t pulse_width){
	ExpPinMode(EXP_DEF_ADDR, pin, OUTPUT);
	ExpSetPwmFreq(EXP_DEF_ADDR, 123); //pwm freq
	uint16_t val=ExpMapRes(pulse_width, 8, 16); //second value - pwm resolution
	ExpWriteCmdPin16BitVal(slave_addr, EXP_ANALOG_WRITE, pin, val);
}

int16_t ExpAnalogRead(uint8_t slave_addr, uint8_t pin){
	//ExpPinMode(EXP_DEF_ADDR, pin, INPUT);
	ExpWriteCmdPin(EXP_DEF_ADDR, EXP_ANALOG_READ, pin);
	_delay_us(10);
	int16_t result=ExpRead16Bit(EXP_DEF_ADDR);
	if (result>=0) result=ExpMapRes(result, 12, 10); //third value - adc resolution
	return result;
}