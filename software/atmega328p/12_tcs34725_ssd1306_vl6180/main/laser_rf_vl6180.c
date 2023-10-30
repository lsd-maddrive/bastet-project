#include "laser_rf_vl6180.h"

void vl6180_send_data(uint16_t reg_addr, uint8_t val){
	uint8_t reg_hbv=(reg_addr>>8), reg_lbv=reg_addr;
	i2c_start();
	i2c_transmit_byte((LASER_RF_ADDR<<1) | W);
	i2c_transmit_byte(reg_hbv);
	i2c_transmit_byte(reg_lbv);
	i2c_transmit_byte(val);
	i2c_stop();
}

uint8_t vl6180x_get_data(uint16_t reg_addr){
	uint8_t val, reg_hbv=(reg_addr>>8), reg_lbv=reg_addr;
	i2c_start();
	i2c_transmit_byte((LASER_RF_ADDR<<1) | W);
	i2c_transmit_byte(reg_hbv);
	i2c_transmit_byte(reg_lbv);
	i2c_start();
	i2c_transmit_byte((LASER_RF_ADDR<<1) | R);
	val = i2c_receive_byte(1);
	i2c_stop();
	return val;
}

void vl6180_change_addr(uint8_t new_addr){
	uint8_t reg_hbv=(I2C_SLAVE__DEVICE_ADDRESS>>8), reg_lbv=I2C_SLAVE__DEVICE_ADDRESS;
	i2c_start();
	i2c_transmit_byte((0x29<<1) | W); //default address
	i2c_transmit_byte(reg_hbv); //address register hbv
	i2c_transmit_byte(reg_lbv); //address register lbv
	i2c_transmit_byte(new_addr); //new address
	i2c_stop();
}

uint8_t vl6180x_init(uint8_t dev_addr){
	vl6180_change_addr(LASER_RF_ADDR);
	if((vl6180x_get_data(SYSTEM__FRESH_OUT_OF_RESET)) != 1)
	return(FAILURE_RESET);
	vl6180_send_data(SYSTEM__FRESH_OUT_OF_RESET, 0x00);
	//tuning up: register address, value to be put in that register
	vl6180_send_data(0x0207, 0x01);
	vl6180_send_data(0x0208, 0x01);
	vl6180_send_data(0x0096, 0x00);
	vl6180_send_data(0x0097, 0xfd);
	vl6180_send_data(0x00e3, 0x00);
	vl6180_send_data(0x00e4, 0x04);
	vl6180_send_data(0x00e5, 0x02);
	vl6180_send_data(0x00e6, 0x01);
	vl6180_send_data(0x00e7, 0x03);
	vl6180_send_data(0x00f5, 0x02);
	vl6180_send_data(0x00d9, 0x05);
	vl6180_send_data(0x00db, 0xce);
	vl6180_send_data(0x00dc, 0x03);
	vl6180_send_data(0x00dd, 0xf8);
	vl6180_send_data(0x009f, 0x00);
	vl6180_send_data(0x00a3, 0x3c);
	vl6180_send_data(0x00b7, 0x00);
	vl6180_send_data(0x00bb, 0x3c);
	vl6180_send_data(0x00b2, 0x09);
	vl6180_send_data(0x00ca, 0x09);
	vl6180_send_data(0x0198, 0x01);
	vl6180_send_data(0x01b0, 0x17);
	vl6180_send_data(0x01ad, 0x00);
	vl6180_send_data(0x00ff, 0x05);
	vl6180_send_data(0x0100, 0x05);
	vl6180_send_data(0x0199, 0x05);
	vl6180_send_data(0x01a6, 0x1b);
	vl6180_send_data(0x01ac, 0x3e);
	vl6180_send_data(0x01a7, 0x1f);
	vl6180_send_data(0x0030, 0x00);
	return(0);
}

uint8_t vl6180x_get_dist(){
	uint8_t result;
	vl6180_send_data(SYSTEM__INTERRUPT_CONFIG_GPIO, (1<<2)); //new sample
	vl6180_send_data(SYSRANGE__START, (1<<0)); //1-shot mode
	while((vl6180x_get_data(RESULT__INTERRUPT_STATUS_GPIO)) != (1<<2));
	result = vl6180x_get_data(RESULT__RANGE_VAL);
	vl6180_send_data(SYSTEM__INTERRUPT_CLEAR, ((1<<2)|(1<<1)|(1<<0)));
	return(result);
}

uint16_t vl6180x_get_amb_light(){
	uint16_t result;
	vl6180_send_data(SYSTEM__INTERRUPT_CONFIG_GPIO, (1<<5)); //new sample
	vl6180_send_data(SYSALS__INTEGRATION_PERIOD_HI, 0); //integration period is
	vl6180_send_data(SYSALS__INTEGRATION_PERIOD_LO, 0x63); //100 ms
	vl6180_send_data(SYSALS__ANALOGUE_GAIN, 0x46); //gain = 1
	vl6180_send_data(SYSALS__START, (1<<0));
	tim1_delay_ms(100);
	while((vl6180x_get_data(RESULT__INTERRUPT_STATUS_GPIO)) != (1<<5));
	result = vl6180x_get_data(RESULT__ALS_VAL_LO);
	result |= (vl6180x_get_data(RESULT__ALS_VAL_HI))<<8;
	vl6180_send_data(SYSTEM__INTERRUPT_CLEAR, ((1<<2)|(1<<1)|(1<<0)));
	return(result);
}