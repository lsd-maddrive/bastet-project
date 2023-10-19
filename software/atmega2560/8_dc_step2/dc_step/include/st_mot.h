#ifndef ST_MOT_H_
#define ST_MOT_H_

#include "utils.h"
#include "adc.h"
//#include "gpio_expander.h"

//DIRECT defined connection
#define ST_MOT_PUL_DDR DDRA
#define ST_MOT_PUL_PORT PORTA
#define ST_MOT_DIR_DDR DDRA
#define ST_MOT_DIR_PORT PORTA
//st_mot #1
#define DD_PUL1 DDA2
#define PORT_PUL1 PA2
#define DD_DIR1 DDA3
#define PORT_DIR1 PA3
//st_mot #2
#define DD_PUL2 DDA6
#define PORT_PUL2 PA6
#define DD_DIR2 DDA7
#define PORT_DIR2 PA7
//st_mot #3
#define DD_PUL3 DDA4
#define PORT_PUL3 PA4
#define DD_DIR3 DDA5
#define PORT_DIR3 PA5
//st_mot #4
#define DD_PUL4 DDA0
#define PORT_PUL4 PA0
#define DD_DIR4 DDA1
#define PORT_DIR4 PA1

//general settings
#define POS_CENTER 512
#define POS_ERR 2
#define MAX_ANGLE 90
#define MIN_ANGLE -90
#define MODE 16 //step divide by n
#define STEPS (360/(1.8/MODE))*2
#define ANGLE_TO_STEPS STEPS/360

#define ALL_MOT //SINGLE_MOT / ALL_MOT
//#define DIRECT //DIRECT / i2c MODULE

void StMotTim1Init(void);
void StMotInit(void);

#ifdef SINGLE_MOT

float* GetInfo(void);
float GetMotPos(uint8_t n);
void StMotPul(uint8_t n);
void StMotDir(float direction, uint8_t n);
void StMotGo();
void SetAngle(float angle);
void StMotCorrectPos(uint8_t n, float real_mot_pos);

#endif
#ifdef ALL_MOT

//void StMotGo(float angle);
//void StMotPul(uint8_t st_mot_chosen);
//void StMotDir(uint8_t st_mot_chosen);
//void StMotCorrectPos(uint8_t st_mot_chosen);

float* GetInfo(void);
float GetMotPos(uint8_t n);
void StMotPul(uint8_t n);
void StMotDir(float direction, uint8_t n);
void StMotGo();
void SetAngle(float angle);
void StMotCorrectPos(uint8_t n, float real_mot_pos);

#endif

#endif /* ST_MOT_H_ */