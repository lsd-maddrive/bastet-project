/*
code includes:
rgb sensor tcs34725;
oled display ssd1306 128x64 px;
laser rangefinder vl6180x;
buzzer.
also code works with: uart, i2c, watchdog and has no connection with delay.h.
 */ 

#include "utils.h"
#include "uart.h"
#include "i2c_basics.h"
#include "rgb_tcs34725.h"
#include "oled_ssd1306.h"
#include "laser_rf_vl6180.h"
#include "buzzer.h"
#include "watchdog.h"

void ssd1306_color_info(void);
void ssd1306_scrsvr(void);
void uart_color_info(void);
void init_all(void);

uint8_t colors[][11]={"Black  ", "White  ", "Red    ", "Orange ", "Yellow ", "Green  ", "Cyan   ", "Blue   ", "Magenta", "Pink   ", "N/D"};
uint8_t title_word[]={"* Color detector *"}, rgb_word[]={"RGB: "}, hex_word[]={"0x"}, hsv_word[]={"HSV:   "}, fin_color_word[]={"Prb. color: "}, amb_light_word[]={"Amb. light: "};
uint8_t dist_word[]={"Dist. [mm]: "}, far_word[]={"move sensor closer"}, close_word[]={"move sensor away  "};
float *crgb_res, *hsv_res; 
uint8_t color_res=0, dist_res=0, amb_light_res=0;
uint8_t mode_flag=0, measure_flag=0, measure_repeat_flag=0, error_flag=0, error_repeat_flag=0, update_type=4;
uint8_t wdt_count=0;

int main(void){
	tim1_delay_ms(250);
	init_all();
	sei();
	while (1){
		switch(mode_flag){
			case(1):{
				mode_flag=0;
				dist_res=vl6180x_get_dist();
				if((dist_res>=80) || (dist_res<=20)){
					measure_flag=0;
					measure_repeat_flag=0;
					error_flag=1; 
					amb_light_res=0;
				}
				else{
					error_flag=0;
					error_repeat_flag=0;
					measure_flag=1;
					amb_light_res=vl6180x_get_amb_light();
					crgb_res=tcs34725_get_rgb(amb_light_res);
					hsv_res=tcs34725_calc_hsv();
					color_res=tcs34725_get_color();
				}
				uart_color_info();
				ssd1306_color_info();
				break;
			}
			case(2):{
				mode_flag=0;
				measure_flag=0;
				measure_repeat_flag=0;
				error_flag=0;
				error_repeat_flag=0;
				ssd1306_scrsvr();
				break;
			}
		}
	}
}

void ssd1306_color_info(void){
	if(error_flag!=0){
		if(error_repeat_flag==error_flag){
			update_type=0; //is not first dist error
		}
		else{
			update_type=1; //is first dist error
			error_repeat_flag=error_flag;			
		}
	}
	else{
		if(measure_repeat_flag==measure_flag){
			update_type=2; //is not first measurement
		}
		else{
			update_type=3; //is first measurement
			measure_repeat_flag=measure_flag;
		}
	}
	switch(update_type){
		case 0:{
			ssd1306_set_cursor(1,3);
			if(dist_res<=20){
				ssd1306_show_str(close_word);
				OCR1A=220;
			}
			else{
				ssd1306_show_str(far_word);
				OCR1A=80;
			}				
			ssd1306_set_cursor(1+((FONT_SIZE+1)*(sizeof(fin_color_word)-1)),5);
			ssd1306_show_str(colors[NO_COLOR]);
			ssd1306_set_cursor(1+((FONT_SIZE+1)*(sizeof(dist_word)-1)),7);
			for(uint8_t i=0; i<3; i++) ssd1306_show_char(' ');
			ssd1306_set_cursor(1+((FONT_SIZE+1)*(sizeof(dist_word)-1)),7);
			if(dist_res<=80) ssd1306_show_dec(dist_res);
			else{
				ssd1306_show_char(0x81);
				ssd1306_set_cursor(1+((FONT_SIZE+1)*(sizeof(dist_word)-1))+(FONT_SIZE-1),7);
				ssd1306_show_char(0x81);
			}
			OCR1A=0;
			break;
		}
		case 1:{
			ssd1306_clear();
			ssd1306_set_cursor(64-((FONT_SIZE+1)*(sizeof(title_word)-1)/2),0);
			ssd1306_show_str(title_word);
			ssd1306_set_cursor(1,3);
			if(dist_res<=20){
				ssd1306_show_str(close_word);
				OCR1A=220;
			}
			else{
				ssd1306_show_str(far_word);
				OCR1A=80;
			}
			ssd1306_set_cursor(1,5);
			ssd1306_show_str(fin_color_word);
			ssd1306_show_str(colors[NO_COLOR]);
			ssd1306_set_cursor(1,7);
			ssd1306_show_str(dist_word);
			if(dist_res<=80) ssd1306_show_dec(dist_res);
			else{
				ssd1306_show_char(0x81);
				ssd1306_set_cursor(1+((FONT_SIZE+1)*(sizeof(dist_word)-1))+(FONT_SIZE-1),7);
				ssd1306_show_char(0x81);
			}
			OCR1A=0;
			break;
		}
		case 2:{
			ssd1306_set_cursor(1+((FONT_SIZE+1)*(sizeof(rgb_word)-1)),2);
			ssd1306_show_str(hex_word);
			ssd1306_show_hex(crgb_res[1]);
			ssd1306_show_hex(crgb_res[2]);
			ssd1306_show_hex(crgb_res[3]);
			ssd1306_set_cursor(1+((FONT_SIZE+1)*(sizeof(hsv_word)-1)),3);
			for(uint8_t i=0; i<11; i++) ssd1306_show_char(' ');
			ssd1306_set_cursor(1+((FONT_SIZE+1)*(sizeof(hsv_word)-1)),3);
			ssd1306_show_dec(hsv_res[0]);
			ssd1306_show_char(',');
			ssd1306_show_dec(hsv_res[1]);
			ssd1306_show_char(',');
			ssd1306_show_dec(hsv_res[2]);
			ssd1306_set_cursor(1+((FONT_SIZE+1)*(sizeof(fin_color_word)-1)),5);
			ssd1306_show_str(colors[color_res]);
			ssd1306_set_cursor(1+((FONT_SIZE+1)*(sizeof(dist_word)-1)),7);
			ssd1306_show_dec(dist_res);
			break;
		}
		case 3:{
			ssd1306_clear();
			ssd1306_set_cursor(64-((FONT_SIZE+1)*(sizeof(title_word)-1)/2),0);
			ssd1306_show_str(title_word);
			ssd1306_set_cursor(1,2);
			ssd1306_show_str(rgb_word);
			ssd1306_show_str(hex_word);
			ssd1306_show_hex(crgb_res[1]);
			ssd1306_show_hex(crgb_res[2]);
			ssd1306_show_hex(crgb_res[3]);
			ssd1306_set_cursor(1,3);
			ssd1306_show_str(hsv_word);
			ssd1306_show_dec(hsv_res[0]);
			ssd1306_show_char(',');
			ssd1306_show_dec(hsv_res[1]);
			ssd1306_show_char(',');
			ssd1306_show_dec(hsv_res[2]);
			ssd1306_set_cursor(1,5);
			ssd1306_show_str(fin_color_word);
			ssd1306_show_str(colors[color_res]);
			ssd1306_set_cursor(1,7);
			ssd1306_show_str(dist_word);
			ssd1306_show_dec(dist_res);
			break;
		}
	}
}

void ssd1306_scrsvr(void){
	uint8_t hor[WIDTH/5], ver[HEIGHT/5], flag=0;
	for(uint16_t i=0; i<(WIDTH*HEIGHT/25); i+=2){
		hor[i]=rand()%WIDTH+1;
		ver[i]=rand()%HEIGHT+1;
		if(flag==0) ssd1306_draw_pixel(hor[i],ver[i]);
		else ssd1306_clear_pixel(hor[i],ver[i]);
		if(i>=(WIDTH*HEIGHT/25)-1){
			i=0;
			if(flag==0) flag=1;
			else flag=0;
		}
		if(mode_flag==1) break;
	}
}

void uart_color_info(void){
	if(error_flag==0){
		uart_send_str(rgb_word);
		uart_send_str(hex_word);
		uart_send_hex(crgb_res[1]);
		uart_send_hex(crgb_res[2]);
		uart_send_hex(crgb_res[3]);
		uart_transmit_byte('\r');
		uart_send_str(hsv_word);
		uart_send_dec(hsv_res[0]);
		uart_transmit_byte('°');
		uart_transmit_byte(',');
		uart_send_dec(hsv_res[1]);
		uart_transmit_byte('%');
		uart_transmit_byte(',');
		uart_send_dec(hsv_res[2]);
		uart_transmit_byte('%');
		uart_transmit_byte('\r');
		uart_send_str(fin_color_word);
		uart_send_str(colors[color_res]);
	}
	else uart_send_str(colors[NO_COLOR]);
	uart_transmit_byte('\r');
	uart_send_str(amb_light_word);
	uart_send_dec(amb_light_res);
	uart_transmit_byte('\r');
}

void init_all(void){
	TCCR0A=(1<<COM0A1)|(1<<WGM00); //tcs34725 led (A-channel)
	TCCR0B=(1<<CS00); //prescaler - 1
	EIMSK=(1<<INT1)|(1<<INT0); //oled module buttons INT0-button_1, INT1-button_2
	EICRA=(1<<ISC11)|(1<<ISC01);
	DDRB|=(1<<5); //sck led
	watchdog_init();
	uart_init();
	i2c_init();
	vl6180x_init(LASER_RF_ADDR);
	tcs34725_init(RGB_SENSOR_ADDR);
	ssd1306_init(DISPLAY_ADDR);
	uint8_t oled_rdy[]={"OLED: rdy"};
	ssd1306_set_cursor(64-((FONT_SIZE+1)*(sizeof(oled_rdy)-1)/2),1);
	ssd1306_show_str(oled_rdy);
	uint8_t tcs_rdy[]={"TCS: rdy 0x"};
	ssd1306_set_cursor(64-((FONT_SIZE+1)*(sizeof(tcs_rdy)+1)/2),3);
	ssd1306_show_str(tcs_rdy);
	ssd1306_show_hex(tcs34725_get_data(ID)); //slave address of rgb sensor
	uint8_t vl_rdy[]={"VL: rdy 0x"};
	ssd1306_set_cursor(64-((FONT_SIZE+1)*(sizeof(vl_rdy)+1)/2),5);
	ssd1306_show_str(vl_rdy);
	ssd1306_show_hex(vl6180x_get_data(I2C_SLAVE__DEVICE_ADDRESS)); //slave address of laser range finder
	buzzer_init();
}

ISR(INT0_vect){
	if((PIND & (1<<2)) == 0){
		tim1_delay_ms(25);
		if((PIND & (1<<2)) == 0){
			mode_flag=1;
			wdt_count=0;
		}
	}
}

ISR(INT1_vect){
	if((PIND & (1<<3)) == 0){
		tim1_delay_ms(25);
		if((PIND & (1<<3)) == 0){
			wdt_count=0;
		}
	}
}

ISR(WDT_vect){ //period - 1s
	WDTCSR|=(1<<WDIE);
	PORTB^=(1<<5);
	wdt_count++;
	if(wdt_count>=45){ //in seconds - screensaver ON
		wdt_count=0;
		mode_flag=2;
	}
}