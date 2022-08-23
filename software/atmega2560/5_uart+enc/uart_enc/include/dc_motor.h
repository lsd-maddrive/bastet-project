#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

//#include "gpio_expander.h"
#include "utils.h"

//MOTOR 1
#define DC_MOT1_SPEED_DDR DDRH
#define DC_MOT1_SPEED_PORT PORTH
#define DC_MOT1_SPEED_DDR_PIN PH5
#define DC_MOT1_SPEED_PORT_PIN PORTH5

//MOTOR 2
#define DC_MOT2_SPEED_DDR DDRH
#define DC_MOT2_SPEED_PORT PORTH
#define DC_MOT2_SPEED_DDR_PIN PH3
#define DC_MOT2_SPEED_PORT_PIN PORTH3

//MOTOR 3
#define DC_MOT3_SPEED_DDR DDRB
#define DC_MOT3_SPEED_PORT PORTB
#define DC_MOT3_SPEED_DDR_PIN DDB4
#define DC_MOT3_SPEED_PORT_PIN PORTB4

//MOTOR 4
#define DC_MOT4_SPEED_DDR DDRH
#define DC_MOT4_SPEED_PORT PORTH
#define DC_MOT4_SPEED_DDR_PIN PH6
#define DC_MOT4_SPEED_PORT_PIN PORTH6

//DIRECTION
#define DC_MOT_DIR_DDR DDRH
#define DC_MOT_DIR_PORT PORTH
#define DC_MOT_DIR_DDR_PIN PH4
#define DC_MOT_DIR_PORT_PIN PORTH4

#define MOVE_FOR 1
#define MOVE_REV 0
#define DC_MOT_REV DC_MOT_DIR_PORT|=(1<<DC_MOT_DIR_PORT_PIN)
#define DC_MOT_FOR DC_MOT_DIR_PORT&=~(1<<DC_MOT_DIR_PORT_PIN)

void Tim2DcMotInit(void);
void DcMotInit(void);
void IntDcMotEcoderInit(void); //110 points per 1 rev
void DcMotGo(float speed);
uint16_t* GetSpeed(void);
void SetSpeed(uint16_t dc_mot_enc_count, uint8_t i);
//uint16_t GetSpeed(void);
//void SetSpeed(uint16_t dc_mot_enc_count);

#endif /* DC_MOT_H_ */