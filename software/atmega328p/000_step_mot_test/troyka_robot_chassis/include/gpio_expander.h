/*
 * gpio_expander.h
 *
 * Created: 29.04.2022 13:40:03
 *  Author: Michael
 */ 


#ifndef GPIO_EXPANDER_H_
#define GPIO_EXPANDER_H_

#include "i2c_basics.h"

#define EXP_DEF_ADDR 0x2A

#define EXP_UID 0x00
    /*
      * command     (0x00)
      * argument    no
      * answer      u32
      
      Return 32 bit unic id stm32f030f4p6 (UID)
      (temperature and analog reference calibration values)
      */

#define EXP_RESET 0x01
    /*
      * command     (0x01)
      * argument    no
      * answer      no
      
      Reset chip 
      */
	
#define EXP_CHANGE_I2C_ADDR 0x02
    /*
      * command     (0x02)
      * argument    u8
      * answer      no
      Set new I2C address on chip. Restart I2C peripheral with new slave address.
      After power off or reset, device will start with old I2C address
      */

#define EXP_SAVE_I2C_ADDR 0x03
    /*
      * command     (0x03)
      * argument    no
      * answer      no
      Save current I2C address on flash, if it was changed by 
      CHANGE_I2C_ADDR, or CHANGE_I2C_ADDR_IF_UID_OK command
      After power off or reset, device will start with new I2C address
      */

#define EXP_PORT_MODE_INPUT 0x04
    /*
      * command     (0x04)
      * argument    u16
      * answer      no
      Set input mode on virtual port 0 pins. If argument is 
      0b0000000000000101, virtual pins 0 and 2 will be set on input mode
      */

#define EXP_PORT_MODE_PULLUP 0x05
    /*
      * command     (0x05)
      * argument    u16
      * answer      no
      Set input pullup mode on virtual port 0 pins. If argument is 
      0b0000000000000101, virtual pins 0 and 2 will be set on input pullup mode
      */

#define EXP_PORT_MODE_PULLDOWN 0x06
    /*
      * command     (0x06)
      * argument    u16
      * answer      no
      Set input pulldown mode on virtual port 0 pins. If argument is 
      0b0000000000000101, virtual pins 0 and 2 will be set on input pulldown mode
      */

#define EXP_PORT_MODE_OUTPUT 0x07
    /*
      * command     (0x07)
      * argument    u16
      * answer      no
      Set output mode on virtual port 0 pins. If argument is 
      0b0000000000000101, virtual pins 0 and 2 will be set on output mode with low value
      */

#define EXP_DIGITAL_READ 0x08
    /*
      * command     (0x08)
      * argument    no
      * answer      u16
      Return digital value of virtual port 0. Answer 
      0b0000000000000101 means virtual pins 0 and 2 is high, and all other pins is low.
      Not change pin mode
      */

#define EXP_DIGITAL_WRITE_HIGH 0x09
    /*
      * command     (0x09)
      * argument    u16
      * answer      no
      Set high digital value of virtual port 0 with change pin mode to output. If argument is 
      0b0000000000000101, virtual pins 0 and 2 will be output with high value.
      All other pins value is not change.
      Change pin mode to output
      */

 #define EXP_DIGITAL_WRITE_LOW 0x0A
    /*
      * command     (0x0A)
      * argument    u16
      * answer      no
      Set low digital value of virtual port 0 with change pin mode to output. If argument is 
      0b0000000000000101, virtual pins 0 and 2 will be output with low value.
      All other pins value is not change.
      Change pin mode to output
      */

#define EXP_ANALOG_WRITE 0x0B
    /*
      * command     (0x0B)
      * arguments   u8    - pin
                    u16   - value
      * answer      no
      Writes an analog value (PWM wave) to a pin.
      The pin will generate a steady square wave of the specified duty cycle
      Default frequency of the PWM signal 1 kHz
      Change pin mode to output
      */

#define EXP_ANALOG_READ 0x0C 
    /*
      * command     (0x0C)
      * arguments   u8    - pin
      * answer      u16
      Return analog values of pin (TODO - adc max value). ADC conversion is never stop,
      so value can be readed immediately
      Not change pin mode
      */

#define EXP_PWM_FREQ 0x0D
    /*
      * command     (0x0D)
      * arguments   u16   - value
      * answer      no
      Set the PWM frequency on all pins at the same time. The PWM filling factor does not change.
      */

#define EXP_ADC_SPEED 0x0E
    /*
      * command     (0x0E)
      * arguments   u8   - value in range 0..7
      * answer      no
      Set ADC conversion speed. Value must be in range 0..7.
      Default value - 6 (TODO - fix it. Need to filtering off and set 7 as default)
    
      0: Sampling time 1.5 ADC clock cycle
      1: Sampling time 7.5 ADC clock cycles
      2: Sampling time 13.5 ADC clock cycles
      3: Sampling time 28.5 ADC clock cycles
      4: Sampling time 41.5 ADC clock cycles
      5: Sampling time 55.5 ADC clock cycles
      6: Sampling time 71.5 ADC clock cycles
      7: Sampling time 239.5 ADC clock cycles
      */

#define EXP_MASTER_READED_UID 0x0F
    /*
      * command     (0x0F)
      * arguments   u32   - UID
      * answer      no
      When many I2Cadio devices have the same I2C address, I2C master can read UID of devices
      with this address (UID command). Only one device can send correct UID 
      (smallest UID. See I2C arbitration). To set new addres on that device, I2C master must
      send readed UID to I2C slaves with command MASTER_READED_UID. If UID is belongs to slave,
      that slave device can change i2c address with the command CHANGE_I2C_ADDR_IF_UID_OK
      */

#define EXP_CHANGE_I2C_ADDR_IF_UID_OK 0x10
    /*
      * command     (0x10)
      * arguments   u8   - new I2C address
      * answer      no
      Set new I2C address on slave device, if slave recieve his UID on MASTER_READED_UID command
      */

#define EXP_SAY_SLOT 0x11
    /*
      * command     (0x11)
      * arguments   no
      * answer      u32   - "slot"
      Command to identify of I2Cadio device on I2C address. If slave answer is "slot", then we can addressing it
      with UID (see MASTER_READED_UID)
      */

    // 0x20 - Advanced ADC functions

#define EXP_ADC_LOWPASS_FILTER_ON 0x20
    /*
      * command     (0x20)
      * arguments   no
      * answer      no
      
      turning on ADC low pass filter. Default state
      */

#define EXP_ADC_LOWPASS_FILTER_OFF 0x21
    /*
      * command     (0x21)
      * arguments   no
      * answer      no
      
      turning off ADC low pass filter.
      */

#define EXP_ADC_AS_DIGITAL_PORT_SET_TRESHOLD 0x22
    /*
      * command     (0x22)
      * arguments   u16   - treshold
      * answer      no
      
      Set treshold for ADC_AS_DIGITAL_PORT_READ
      */

#define EXP_ADC_AS_DIGITAL_PORT_READ 0x23
    /*
      * command     (0x23)
      * arguments   no
      * answer      u16
      
      Return digital value of virtual port 0 from pins analog value. Answer 
      0b0000000000000101 means analog value on virtual pins 0 and 2 is equal or larger than treshold
      Not change pin mode
      */

#define EXP_ENCODER_SET_PINS 0x30
    /*
      * command     (0x30)
      * arguments   u8 - encoder nummber, u8 - encoder A<<4|B pin number
      * answer      
      
      Add encoder on A and B pins, packed on u8.
      4 encoders max
      */

#define EXP_ENCODER_GET_DIFF_VALUE 0x31
    /*
      * command     (0x31)
      * arguments   u8 - encoder number
      * answer      int8_t - diff value
      
      Return difference steps after last encoder read.
      4 encoders max
      */

    // 0x40 -Advanced PWM functions

#define EXP_PWM_ANALOG_WRITE_U8 0x40 // 1b in
    // 0x60 -Advanced Digital functions
    // 0x80 -Software interfaces

    // ... 8 groups, 32 commands each

    // etc - start at 0xE0
	
#define EXP_ETC_VERSION 0xE0 // command, 4b answer

#define EXP_ETC_ACT_LED_ENABLE 0xE1 // command
    
#define EXP_ETC_ACT_LED_DISABLE 0xE2 // command
    
#define EXP_ETC_ACT_LED_BLINK_WITH_COUNTER 0xE3 // 1b in

#define EXP_ETC_NUM_DIGITAL_PINS 0xE4

#define EXP_ETC_NUM_ANALOG_INPUTS 0xE5

#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2
#define INPUT_PULLDOWN 3

void ExpWriteCmd(uint8_t slave_addr, uint8_t cmd);
void ExpWriteCmdPin(uint8_t slave_addr, uint8_t cmd, uint8_t pin);
void ExpWriteCmd8BitVal(uint8_t slave_addr, uint8_t cmd, uint8_t val);
void ExpWriteCmd16BitVal(uint8_t slave_addr, uint8_t cmd, uint16_t val);
void ExpWriteCmdPin16BitVal(uint8_t slave_addr, uint8_t cmd, uint8_t pin, uint16_t val);
uint16_t ExpReadCmd(uint8_t slave_addr, uint8_t cmd);
int16_t ExpRead16Bit(uint8_t slave_addr);
void ExpReset(uint8_t slave_addr);
void ExpInit(uint8_t slave_addr);
void ExpPinMode(uint8_t slave_addr, uint8_t pin, uint8_t mode);
void ExpPinModePort(uint8_t slave_addr, uint16_t chosen_pin, uint8_t mode);
void ExpDigitalWrite(uint8_t slave_addr, uint8_t pin, uint8_t val);
uint8_t ExpDigitalRead(uint8_t slave_addr, uint8_t pin);
uint16_t ExpMapRes(uint16_t value, uint8_t from, uint8_t to);
void ExpSetPwmFreq(uint8_t slave_addr, uint16_t freq);
void ExpAnalogWrite(uint8_t slave_addr, uint8_t pin, uint16_t pulse_width);
int16_t ExpAnalogRead(uint8_t slave_addr, uint8_t pin);

#endif /* GPIO_EXPANDER_H_ */