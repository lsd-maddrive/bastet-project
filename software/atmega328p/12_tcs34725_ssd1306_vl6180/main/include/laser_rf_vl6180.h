/*
 for laser range finder vl6180x 
 0...100 mm
 connection via i2c
 */

#ifndef LASER_RF_VL6180_H_
#define LASER_RF_VL6180_H_

#include "i2c_basics.h"

//7-битный адрес устройства
#define LASER_RF_ADDR 0x2A //default 0x29
//регистры датчика
#define SYSTEM__INTERRUPT_CONFIG_GPIO 0x014
#define SYSTEM__INTERRUPT_CLEAR 0x015
#define SYSTEM__FRESH_OUT_OF_RESET 0x016
#define SYSRANGE__START 0x018
#define SYSALS__START 0x038
#define SYSALS__ANALOGUE_GAIN 0x03F
#define SYSALS__INTEGRATION_PERIOD_HI 0x040
#define SYSALS__INTEGRATION_PERIOD_LO 0x041
#define RESULT__INTERRUPT_STATUS_GPIO 0x04F
#define RESULT__ALS_VAL_HI 0x050
#define RESULT__ALS_VAL_LO 0x051
#define RESULT__RANGE_VAL 0x062
#define I2C_SLAVE__DEVICE_ADDRESS 0x212
#define FAILURE_RESET -1

void vl6180_send_data(uint16_t reg_addr, uint8_t val);
uint8_t vl6180x_get_data(uint16_t reg_addr);
void vl6180_change_addr(uint8_t new_addr);
uint8_t vl6180x_init(uint8_t dev_addr);
uint8_t vl6180x_get_dist(void); //distance in [mm]
uint16_t vl6180x_get_amb_light(void);

#endif