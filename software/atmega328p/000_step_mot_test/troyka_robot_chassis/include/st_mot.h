/*
 * st_mot.h
 *
 * Created: 11.03.2022 17:48:26
 *  Author: Michael
 */ 


#ifndef ST_MOT_H_
#define ST_MOT_H_

#include "i2c_basics.h"
#include "utils.h"
#include "adc.h"
#include "gpio_expander.h"

//i2c MODULE defined pinout
//PUL1-4, PUL2-5, PUL3-6, PUL4-7
//DIR1-0, DIR2-1, DIR3-2, DIR4-3
#define ST_MOT_ADDR 0x20

//DIRECT defined connection
#define ST_MOT_PUL_DDR DDRC
#define ST_MOT_PUL_PORT PORTC
#define ST_MOT_DIR_DDR DDRB
#define ST_MOT_DIR_PORT PORTB
//st_mot #1
#define DD_PUL1 DDC0
#define PORT_PUL1 PORTC0
#define DD_DIR1 DDB0
#define PORT_DIR1 PORTB0
//st_mot #2
#define DD_PUL2 DDC1
#define PORT_PUL2 PORTC1
#define DD_DIR2 DDB3
#define PORT_DIR2 PORTB3
//st_mot #3
#define DD_PUL3 DDC2
#define PORT_PUL3 PORTC2
#define DD_DIR3 DDB4
#define PORT_DIR3 PORTB4
//st_mot #4
#define DD_PUL4 DDC3
#define PORT_PUL4 PORTC3
#define DD_DIR4 DDB5
#define PORT_DIR4 PORTB5

//general settings
#define POS_CENTER 512
#define POS_ERR 2
#define MAX_ANGLE 90
#define MIN_ANGLE -90
#define MODE 16 //step divide by n
#define STEPS (360/(1.8/MODE))*2

#define ALL_MOT //SINGLE_MOT / ALL_MOT 
#define DIRECT //DIRECT / i2c MODULE

void StMotTim0Init(void);
void StMotInit(void);
void StMotI2cSendData(uint8_t val);

#ifdef SINGLE_MOT

void StMotGo(float angle, uint8_t st_mot_num);
void StMotPul(void);
void StMotDir(void);
void StMotCorrectPos(void);

#endif
#ifdef ALL_MOT

void StMotGo(float angle);
void StMotPul(uint8_t st_mot_chosen);
void StMotDir(uint8_t st_mot_chosen);
void StMotCorrectPos(uint8_t st_mot_chosen);

#endif

#endif /* ST_MOT_H_ */