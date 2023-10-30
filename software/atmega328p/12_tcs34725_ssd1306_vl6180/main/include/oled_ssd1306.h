 /*
 for oled display module ssd1306 
 128*64
 connection via i2c
 */

#ifndef OLED_SSD1306_H_
#define OLED_SSD1306_H_

#include "i2c_basics.h"

//7-bit addressof ssd1306
#define DISPLAY_ADDR 0x3C
//1. fundamental command table
#define SET_CONTRAST 0x81
#define RAM_ON 0xA4
#define RAM_OFF 0xA5
#define INVERT_OFF 0xA6
#define INVERT_ON 0xA7
#define DISPLAY_OFF 0xAE
#define DISPLAY_ON 0xAF
//3. addressing setting command
#define ADDR_MODE 0x20
#define SET_COLUMN_ADDR 0x21
#define SET_PAGE_ADDR 0x22
//4. hardware configuration command
#define SET_START_LINE 0x40
#define SET_REMAP_L_TO_R 0xA1
#define SET_MULTIPLEX_RATIO 0xA8
#define SET_REMAP_T_TO_D 0xC8
#define SET_DISPLAY_OFFSET 0xD3
#define SET_COM_PINS 0xDA
//5. timing & driving scheme setting command
#define SET_DISPLAY_CLOCK 0xD5
#define SET_PRECHARGE_PERIOD 0xD9
#define SET_VCOM_DESELECT 0xDB
//1. charge pump command
#define CHARGE_DCDC_PUMP 0x8D
//general use
#define WIDTH 128 //128 pix
#define HEIGHT 64 //64 pix
#define NO_DATA 0
#define FONT_SIZE 5

void ssd1306_cmd(uint8_t *data_buf, uint8_t length); //sending a command
void ssd1306_send_cmd_1(uint8_t command_addr); //only command with no value (e.g. DISPLAY ON)
void ssd1306_send_cmd_2(uint8_t command_addr, uint8_t value); //command with 1 value (e.g. SET_CONTRAST, 0x9F)
void ssd1306_send_cmd_3(uint8_t command_addr, uint8_t value1, uint8_t value2); //command with 2 values (e.g. SET_COLUMN_ADDR start_addr end_addr)
void ssd1306_send_data(uint8_t data); //sending data, not command, painting pixels on display
void ssd1306_clear(void); //clear display, cursor to 0,0 position
uint8_t ssd1306_init(uint8_t dev_addr); //initialization (see initialization function)
void ssd1306_set_cursor(uint8_t col_pos, uint8_t line_pos); //setting cursor to vertical segment (x_axis col 0...127) and to horizontal page (y_axis line 0...7)
void ssd1306_goto_line(uint8_t line); //go to designated line 0...7
void ssd1306_next_line(void); //go to line++
void ssd1306_img_buf(uint8_t *img_buff); //update whole screen image
void ssd1306_show_char(uint8_t data); //sending ascii codes of symbols to the screen (also '\r', '\n' etc.)
void ssd1306_show_str(uint8_t *str); //sending words, text to the screen
void ssd1306_show_hex(uint8_t val); //sending hex values to the screen
void ssd1306_show_dec(uint16_t val); //sending decimal values to the screen
void ssd1306_draw_pixel(int8_t x_coord, int8_t y_coord); //draw pixel on horizontal x coordinate and vertical y coordinate
void ssd1306_clear_pixel(int8_t x_coord, int8_t y_coord); //clear (make it blank) pixel on horizontal x coordinate and vertical y coordinate
void ssd1306_draw_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2); //drawing lines from (x1,y1) to (x2,y2)
//uint8_t ssd1306_scrsaver(uint8_t scrsvr_type);

#endif /* OLED_SSD1306_H_ */