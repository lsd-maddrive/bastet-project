#include "rgb_tcs34725.h"

float crgb[4]; //clear red green blue
float hsv[3]; //hue saturation value

void tcs34725_send_data(uint8_t reg_addr, uint8_t val){
	i2c_start();
	i2c_transmit_byte((RGB_SENSOR_ADDR<<1) | W);
	i2c_transmit_byte(COMMAND_REGISTER | reg_addr);
	i2c_transmit_byte(val);
	i2c_stop();
}

uint8_t tcs34725_get_data(uint8_t reg_addr){
	uint8_t val;
	i2c_start();
	i2c_transmit_byte((RGB_SENSOR_ADDR<<1) | W);
	i2c_transmit_byte(COMMAND_REGISTER | reg_addr);
	i2c_start();
	i2c_transmit_byte((RGB_SENSOR_ADDR<<1) | R);
	val = i2c_receive_byte(1);
	i2c_stop();
	return(val);
}

uint8_t tcs34725_init(uint8_t dev_addr){
	tcs34725_send_data(ENABLE, (1<<0)); //turn sensor on
	tcs34725_send_data(ATIME, 0xF6); //time for RGBC processing = 24 ms
	tim1_delay_ms(3);
	OCR0A = 1;
	tim1_delay_ms(100);
	OCR0A = 0;
	return(0);
}

float* tcs34725_get_rgb(uint8_t amb_light){
	uint16_t raw_clear[3]={0,0,0}, raw_red[3]={0,0,0}, raw_green[3]={0,0,0}, raw_blue[3]={0,0,0};
	tcs34725_send_data(ENABLE, (1<<0)|(1<<1)); //power supply on and prepare for color recognition
	//making three measurements with different ambient light (LED on the sensor`s pcb)
	cli();
	for(uint8_t i=0; i<3; i++){
		if(amb_light >= 25) DDRD&=~(1<<LED_PIN); //if it emits light, then LED is not needed
		else DDRD|=(1<<LED_PIN); //if it has no lights emitted, then bright it up with LED
		OCR0A = 50+(i*100); //changing the LED`s brightness 50, 150, 250
		raw_clear[i] = tcs34725_get_data(CLEAR_DATAL);
		raw_clear[i] |= (tcs34725_get_data(CLEAR_DATAH)<<8);
		raw_red[i] = tcs34725_get_data(RED_DATAL);
		raw_red[i] |= (tcs34725_get_data(RED_DATAH)<<8);
		raw_green[i] = tcs34725_get_data(GREEN_DATAL);
		raw_green[i] |= (tcs34725_get_data(GREEN_DATAH)<<8);
		raw_blue[i] = tcs34725_get_data(BLUE_DATAL);
		raw_blue[i] |= (tcs34725_get_data(BLUE_DATAH)<<8);
		OCR0A = 0;
		tim1_delay_ms(30); //delay = (256-0xF6)*2.4 = 24мс -> 30мс (0xF6 <- ATIME)
	}
	tcs34725_send_data(ENABLE, 0x00);
	sei();
	//average on 3 measurements
	crgb[RGB_CLEAR] = (raw_clear[0]+raw_clear[1]+raw_clear[2])/3;
	crgb[RGB_RED] = (raw_red[0]+raw_red[1]+raw_red[2])/3;
	crgb[RGB_GREEN] = (raw_green[0]+raw_green[1]+raw_green[2])/3;
	crgb[RGB_BLUE] = (raw_blue[0]+raw_blue[1]+raw_blue[2])/3;	
	return(crgb);
}

float* tcs34725_calc_hsv(void){
	float _red = crgb[RGB_RED]/crgb[RGB_CLEAR]*255;
	float _green = crgb[RGB_GREEN]/crgb[RGB_CLEAR]*255;
	float _blue = crgb[RGB_BLUE]/crgb[RGB_CLEAR]*255;
	float C_max = find_max_out_of_3(_red, _green, _blue);
	float C_min = find_min_out_of_3(_red, _green, _blue);
	float delta = C_max-C_min;
	
	//calculation Hue (formula from the internet)
	if(C_max == C_min) hsv[HSV_HUE] = 0;
	else if (C_max == _red) hsv[HSV_HUE] = fmod((60*((_green-_blue)/delta)+360), 360.0);
	else if (C_max == _green) hsv[HSV_HUE] = fmod((60*((_blue-_red)/delta)+120), 360.0);
	else if (C_max == _blue) hsv[HSV_HUE] = fmod((60*((_red-_green)/delta)+240), 360.0);
	
	//calculation Saturation
	if(C_max == 0) hsv[HSV_SATURATION] = 0;
	else{
		hsv[HSV_SATURATION] = (delta/C_max)*100;
		if(hsv[HSV_SATURATION]>100) hsv[HSV_SATURATION]=100;
	}
	
	//calculation Value
	hsv[HSV_VALUE] = C_max*100;
	if(hsv[HSV_VALUE]>100) hsv[HSV_VALUE]=100;

	return(hsv);
}

uint8_t tcs34725_get_color(void){
	if(hsv[HSV_VALUE]<=4) return(BLACK);
	if(hsv[HSV_HUE]>=351) return(RED);
	if(hsv[HSV_HUE]>=311) return(PINK);
	if(hsv[HSV_HUE]>=241) return(MAGENTA);
	if(hsv[HSV_HUE]>=191) return(BLUE);
	if(hsv[HSV_HUE]>=135) return(CYAN);
	if(hsv[HSV_HUE]>=65) return(GREEN);
	if(hsv[HSV_HUE]>=21) return(YELLOW);
	if(hsv[HSV_HUE]>=5) return(ORANGE);
	return(RED);
}