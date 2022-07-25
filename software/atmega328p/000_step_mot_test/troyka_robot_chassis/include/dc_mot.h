/*
 * dc_mot.h
 *
 * Created: 01.04.2022 15:03:49
 *  Author: Michael
 */ 


#ifndef DC_MOT_H_
#define DC_MOT_H_

#include "gpio_expander.h"
#include "utils.h"

#define DC_MOT_SPEED_DDR DDRD
#define DC_MOT_SPEED_PORT PORTD
#define DC_MOT_SPEED_DDR_PIN DDD3
#define DC_MOT_SPEED_PORT_PIN PORTD3

#define DC_MOT_DIR_DDR DDRB
#define DC_MOT_DIR_PORT PORTB
#define DC_MOT_DIR_DDR_PIN DDB4
#define DC_MOT_DIR_PORT_PIN PORTB4

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