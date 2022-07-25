/*
tcs34725 rgb sensor
connection via i2c
*/

#ifndef RGB_TCS34725_H_
#define RGB_TCS34725_H_

#include "i2c_basics.h"

//7-bit address of tcs34725
#define RGB_SENSOR_ADDR 0x29
//LED pin
#define LED_PIN PD6
//sensor registers
#define COMMAND_REGISTER 0x80
#define ENABLE 0x00
#define ATIME 0x01
#define WTIME 0x03
#define CONTROL 0x0F
#define ID 0x12
#define CLEAR_DATAL 0x14
#define CLEAR_DATAH 0x15
#define RED_DATAL 0x16
#define RED_DATAH 0x17
#define GREEN_DATAL 0x18
#define GREEN_DATAH 0x19
#define BLUE_DATAL 0x1A
#define BLUE_DATAH 0x1B
//crgb_hsv
#define RGB_CLEAR 0
#define RGB_RED 1
#define RGB_GREEN 2
#define RGB_BLUE 3
#define HSV_HUE 0
#define HSV_SATURATION 1
#define HSV_VALUE 2
//colors
#define BLACK 0
#define WHITE 1
#define RED 2
#define ORANGE 3
#define YELLOW 4
#define GREEN 5
#define CYAN 6
#define BLUE 7
#define MAGENTA 8
#define PINK 9
#define NO_COLOR 10

void tcs34725_send_data(uint8_t reg_addr, uint8_t val); //sending data to sensor via i2c
uint8_t tcs34725_get_data(uint8_t reg_addr); //getting data from sensor via i2c
uint8_t tcs34725_init(uint8_t dev_addr); //initialization of sensor
uint8_t tcs34725_get_color(void); //getting probable color (see defines @colors@)
float* tcs34725_get_rgb(uint8_t amb_light); //get raw rgb (+clear) data
float* tcs34725_calc_hsv(void); //calculate hsv components

#endif /* RGB_TCS34725_H_ */