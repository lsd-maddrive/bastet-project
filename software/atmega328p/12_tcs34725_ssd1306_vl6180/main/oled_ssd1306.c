#include "oled_ssd1306.h"

uint8_t ssd1306_line_pos, ssd1306_col_pos;

const uint8_t FontTable[][FONT_SIZE] PROGMEM = {
	{0x00, 0x00, 0x00, 0x00, 0x00},   // space 00   20 hex ASCII
	{0x00, 0x00, 0x2f, 0x00, 0x00},   // !     01   21
	{0x00, 0x07, 0x00, 0x07, 0x00},   // "     02   22
	{0x14, 0x7f, 0x14, 0x7f, 0x14},   // #     03   23
	{0x24, 0x2a, 0x7f, 0x2a, 0x12},   // $     04   24
	{0x23, 0x13, 0x08, 0x64, 0x62},   // %     05   25
	{0x36, 0x49, 0x55, 0x22, 0x50},   // &     06   26
	{0x00, 0x05, 0x03, 0x00, 0x00},   // '     07   27
	{0x00, 0x1c, 0x22, 0x41, 0x00},   // (     08   28
	{0x00, 0x41, 0x22, 0x1c, 0x00},   // )     09   29
	{0x14, 0x08, 0x3E, 0x08, 0x14},   // *     10   2a
	{0x08, 0x08, 0x3E, 0x08, 0x08},   // +     11   2b
	{0x00, 0x00, 0xA0, 0x60, 0x00},   // ,     12   2c
	{0x08, 0x08, 0x08, 0x08, 0x08},   // -     13   2d
	{0x00, 0x60, 0x60, 0x00, 0x00},   // .     14   2e
	{0x20, 0x10, 0x08, 0x04, 0x02},   // /     15   2f
	{0x3E, 0x51, 0x49, 0x45, 0x3E},   // 0     16   30
	{0x00, 0x42, 0x7F, 0x40, 0x00},   // 1     17   31
	{0x42, 0x61, 0x51, 0x49, 0x46},   // 2     18   32
	{0x21, 0x41, 0x45, 0x4B, 0x31},   // 3     19   33
	{0x18, 0x14, 0x12, 0x7F, 0x10},   // 4     20   34
	{0x27, 0x45, 0x45, 0x45, 0x39},   // 5     21   35
	{0x3C, 0x4A, 0x49, 0x49, 0x30},   // 6     22   36
	{0x01, 0x71, 0x09, 0x05, 0x03},   // 7     23   37
	{0x36, 0x49, 0x49, 0x49, 0x36},   // 8     24   38
	{0x06, 0x49, 0x49, 0x29, 0x1E},   // 9     25   39
	{0x00, 0x36, 0x36, 0x00, 0x00},   // :     26   3a
	{0x00, 0x56, 0x36, 0x00, 0x00},   // ;     27   3b
	{0x08, 0x14, 0x22, 0x41, 0x00},   // <     28   3c
	{0x14, 0x14, 0x14, 0x14, 0x14},   // =     29   3d
	{0x00, 0x41, 0x22, 0x14, 0x08},   // >     30   3e
	{0x02, 0x01, 0x51, 0x09, 0x06},   // ?     31   3f
	{0x32, 0x49, 0x59, 0x51, 0x3E},   // @     32   40
	{0x7C, 0x12, 0x11, 0x12, 0x7C},   // A     33   41
	{0x7F, 0x49, 0x49, 0x49, 0x36},   // B     34   42
	{0x3E, 0x41, 0x41, 0x41, 0x22},   // C     35   43
	{0x7F, 0x41, 0x41, 0x22, 0x1C},   // D     36   44
	{0x7F, 0x49, 0x49, 0x49, 0x41},   // E     37   45
	{0x7F, 0x09, 0x09, 0x09, 0x01},   // F     38   46
	{0x3E, 0x41, 0x49, 0x49, 0x7A},   // G     39   47
	{0x7F, 0x08, 0x08, 0x08, 0x7F},   // H     40   48
	{0x00, 0x41, 0x7F, 0x41, 0x00},   // I     41   49
	{0x20, 0x40, 0x41, 0x3F, 0x01},   // J     42   4a
	{0x7F, 0x08, 0x14, 0x22, 0x41},   // K     43   4b
	{0x7F, 0x40, 0x40, 0x40, 0x40},   // L     44   4c
	{0x7F, 0x02, 0x0C, 0x02, 0x7F},   // M     45   4d
	{0x7F, 0x04, 0x08, 0x10, 0x7F},   // N     46   4e
	{0x3E, 0x41, 0x41, 0x41, 0x3E},   // O     47   4f
	{0x7F, 0x09, 0x09, 0x09, 0x06},   // P     48   50
	{0x3E, 0x41, 0x51, 0x21, 0x5E},   // Q     49   51
	{0x7F, 0x09, 0x19, 0x29, 0x46},   // R     50   52
	{0x46, 0x49, 0x49, 0x49, 0x31},   // S     51   53
	{0x01, 0x01, 0x7F, 0x01, 0x01},   // T     52   54
	{0x3F, 0x40, 0x40, 0x40, 0x3F},   // U     53   55
	{0x1F, 0x20, 0x40, 0x20, 0x1F},   // V     54   56
	{0x3F, 0x40, 0x38, 0x40, 0x3F},   // W     55   57
	{0x63, 0x14, 0x08, 0x14, 0x63},   // X     56   58
	{0x07, 0x08, 0x70, 0x08, 0x07},   // Y     57   59
	{0x61, 0x51, 0x49, 0x45, 0x43},   // Z     58   5a
	{0x00, 0x7F, 0x41, 0x41, 0x00},   // [     59   5b
	{0x55, 0xAA, 0x55, 0xAA, 0x55},   //       60   5c Backslash (Checker pattern)
	{0x00, 0x41, 0x41, 0x7F, 0x00},   // ]     61   5d
	{0x04, 0x02, 0x01, 0x02, 0x04},   // ^     62   5e
	{0x40, 0x40, 0x40, 0x40, 0x40},   // _     63   5f underscore
	{0x00, 0x03, 0x05, 0x00, 0x00},   // `     64   60 apostrophe
	{0x20, 0x54, 0x54, 0x54, 0x78},   // a     65   61
	{0x7F, 0x48, 0x44, 0x44, 0x38},   // b     66   62
	{0x38, 0x44, 0x44, 0x44, 0x20},   // c     67   63
	{0x38, 0x44, 0x44, 0x48, 0x7F},   // d     68   64
	{0x38, 0x54, 0x54, 0x54, 0x18},   // e     69   65
	{0x08, 0x7E, 0x09, 0x01, 0x02},   // f     70   66
	{0x18, 0xA4, 0xA4, 0xA4, 0x7C},   // g     71   67
	{0x7F, 0x08, 0x04, 0x04, 0x78},   // h     72   68
	{0x00, 0x44, 0x7D, 0x40, 0x00},   // i     73   69
	{0x40, 0x80, 0x84, 0x7D, 0x00},   // j     74   6a
	{0x7F, 0x10, 0x28, 0x44, 0x00},   // k     75   6b
	{0x00, 0x41, 0x7F, 0x40, 0x00},   // l     76   6c
	{0x7C, 0x04, 0x18, 0x04, 0x78},   // m     77   6d
	{0x7C, 0x08, 0x04, 0x04, 0x78},   // n     78   6e
	{0x38, 0x44, 0x44, 0x44, 0x38},   // o     79   6f
	{0xFC, 0x24, 0x24, 0x24, 0x18},   // p     80   70
	{0x18, 0x24, 0x24, 0x18, 0xFC},   // q     81   71
	{0x7C, 0x08, 0x04, 0x04, 0x08},   // r     82   72
	{0x48, 0x54, 0x54, 0x54, 0x20},   // s     83   73
	{0x04, 0x3F, 0x44, 0x40, 0x20},   // t     84   74
	{0x3C, 0x40, 0x40, 0x20, 0x7C},   // u     85   75
	{0x1C, 0x20, 0x40, 0x20, 0x1C},   // v     86   76
	{0x3C, 0x40, 0x30, 0x40, 0x3C},   // w     87   77
	{0x44, 0x28, 0x10, 0x28, 0x44},   // x     88   78
	{0x1C, 0xA0, 0xA0, 0xA0, 0x7C},   // y     89   79
	{0x44, 0x64, 0x54, 0x4C, 0x44},   // z     90   7a
	{0x00, 0x10, 0x7C, 0x82, 0x00},   // brace 91   7b
	{0x00, 0x00, 0xFF, 0x00, 0x00},   // |     92   7c
	{0x00, 0x82, 0x7C, 0x10, 0x00},   // brace 93   7d
	{0x00, 0x06, 0x09, 0x09, 0x06},   // ~     94   7e
	{0x3C, 0x7E, 0x7E, 0x7E, 0x3C},	  //oval   95   7f
	{0x00, 0x30, 0x48, 0x48, 0x30},	  //celc   96   80
	{0x3C, 0x42, 0x42, 0x42, 0x3C}    //infinity/2  81
};

void ssd1306_cmd(uint8_t *data_buf, uint8_t length){
	i2c_start();
	i2c_transmit_byte((DISPLAY_ADDR<<1) | W);
	i2c_transmit_byte(0x00); //command signature (D/C# in i2c diagram = 0)
	for(uint8_t i=0; i<length; i++) i2c_transmit_byte(data_buf[i]);
	i2c_stop();
}

void ssd1306_send_cmd_1(uint8_t command_addr){
	uint8_t data_buf[] = {command_addr};
	ssd1306_cmd(data_buf, 1);
}

void ssd1306_send_cmd_2(uint8_t command_addr, uint8_t value){
	uint8_t data_buf[] = {command_addr, value};
	ssd1306_cmd(data_buf, 2);
}

void ssd1306_send_cmd_3(uint8_t command_addr, uint8_t value1, uint8_t value2){
	uint8_t data_buf[] = {command_addr, value1, value2};
	ssd1306_cmd(data_buf, 3);
}

void ssd1306_send_data(uint8_t data){
	i2c_start();
	i2c_transmit_byte((DISPLAY_ADDR<<1) | W);
	i2c_transmit_byte(0x40); //data signature (D/C# in i2c diagram = 1)
	i2c_transmit_byte(data);
	i2c_stop();
}

uint8_t ssd1306_init(uint8_t dev_addr){
	//turn on ssd1306
	ssd1306_send_cmd_1(DISPLAY_OFF);
	//update frequency by default
	ssd1306_send_cmd_2(SET_DISPLAY_CLOCK, 0x80);
	//multiplex coefficient of COM pins by default
	ssd1306_send_cmd_2(SET_MULTIPLEX_RATIO, 0x3F);
	//set vertical shift by COM from 0d~63d 
	ssd1306_send_cmd_2(SET_DISPLAY_OFFSET, 0x00);
	//set display RAM display start line register
	ssd1306_send_cmd_1(SET_START_LINE | 0x00);
	//power supply scheme (0x14 - internal DC-DC on, 0x10 - off)
	ssd1306_send_cmd_2(CHARGE_DCDC_PUMP, 0x14);
	//addressing mode (0x00-horizontal,	0x01-vertical, 0x10-page)
	ssd1306_send_cmd_2(ADDR_MODE, 0x00);
	//set segment remap (left-to-right)
	ssd1306_send_cmd_1(SET_REMAP_L_TO_R);
	//set page remap (to-to-bottom)
	ssd1306_send_cmd_1(SET_REMAP_T_TO_D);
	//set COM pins hardware configuration by default
	ssd1306_send_cmd_2(SET_COM_PINS, 0x12);
	//set contrast (0x00...0xFF)
	ssd1306_send_cmd_2(SET_CONTRAST, 0xCF);	
	//set DC/DC scheme (0xF1 - Vcc is taken from DC/DC converter
	//0x22 - Vcc is taken from external source)
	ssd1306_send_cmd_2(SET_PRECHARGE_PERIOD, 0xF1);
	/*set power supply of LEDs VcomH in a different value than default 0x30:
	probable values: 0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70*/
	ssd1306_send_cmd_2(SET_VCOM_DESELECT, 0x40); //oled will be brighter
	//RAM content diplay is permitted
	ssd1306_send_cmd_1(RAM_ON);
	//inversion is off
	ssd1306_send_cmd_1(INVERT_OFF);
	//display is on
	ssd1306_send_cmd_1(DISPLAY_ON);
	//clearing display
	ssd1306_clear();
	return(0);
}

void ssd1306_clear(){
	ssd1306_send_cmd_3(SET_COLUMN_ADDR, 0, 127);
	ssd1306_send_cmd_3(SET_PAGE_ADDR, 0, 7);
	//128 bytes (8 bits vertical) per row * 8 char rows = 1024
	//Send 64 TWI messages {S:SLAw:0x40:16 bytes of 0x00:P}  1024 data bytes total
	for(uint8_t lineCnt=0; lineCnt<8; lineCnt++){  //(LCDWIDTH*LCDHEIGHT/8)
		for(uint8_t index=0; index<128; index++) ssd1306_send_data(0x00);
	}
	ssd1306_set_cursor(0,0);
}

void ssd1306_set_cursor(uint8_t col_pos, uint8_t line_pos){
	if((line_pos <= 7) && (col_pos <= 127)){
		ssd1306_line_pos = line_pos; // global var: current line number
		ssd1306_col_pos = col_pos; // global var: current cursor position
		ssd1306_send_cmd_3(SET_COLUMN_ADDR, col_pos, 127);
		ssd1306_send_cmd_3(SET_PAGE_ADDR, line_pos, 7);
	}
}

void ssd1306_goto_line(uint8_t line){
	if(line <= 0x07){
		ssd1306_line_pos = line;
		ssd1306_set_cursor(0, ssd1306_line_pos);
	}
}

void ssd1306_next_line(void){ // Increment and roll-over the current line number.
	ssd1306_line_pos++; // global var for current line number
	ssd1306_line_pos = ssd1306_line_pos & 0x07; // roll-over back to top after 8 lines
	ssd1306_set_cursor(0, ssd1306_line_pos);
}

void ssd1306_img_buf(uint8_t *img_buff){
	ssd1306_set_cursor(0,0);
	for(uint16_t i=0; i<=1023; i++){
		ssd1306_send_data(img_buff[i]);
	}
}

void ssd1306_show_char(uint8_t data){
	uint8_t charColumnBitMap, charBitMapIndex = 0;

	if(((ssd1306_col_pos + FONT_SIZE) >= 128) || (data=='\n')){ // font size=5
		//If the line has no more room for complete chars OR NewLine command,
		//then move the cursor to next line
		ssd1306_next_line();
	}
	if(data != '\n'){
		data = data - 0x20; // As the lookup table starts from Space(0x20)
		while(1){
			charColumnBitMap = pgm_read_byte(&FontTable[data][charBitMapIndex]);
			ssd1306_send_data(charColumnBitMap);
			ssd1306_col_pos++;
			charBitMapIndex++;
			if(charBitMapIndex == FONT_SIZE) {   // Exit the loop after sending five bytes
				ssd1306_send_data(0x00);
				ssd1306_col_pos++;
				break;
			}    
		}  
	}
}

void ssd1306_show_str(uint8_t *str){
	while(*str) ssd1306_show_char(*str++);
}

void ssd1306_show_hex(uint8_t val){
	uint8_t msb = (val & 0xF0) >> 4, lsb = val & 0x0F;
	msb += msb > 9 ? 'A' - 10 : '0';
	lsb += lsb > 9 ? 'A' - 10 : '0';
	ssd1306_show_char(msb);
	ssd1306_show_char(lsb);
}

void ssd1306_show_dec(uint16_t val){
	uint8_t i, j;
	if(val!=0){
		j=1;
		while(!(Digit(val, j++)));
		for(i=j-1; i<=5; i++) ssd1306_show_char(Digit(val, i)+'0');
	}
	else ssd1306_show_char('0');
}

void ssd1306_draw_pixel(int8_t x_coord, int8_t y_coord){
	//ssd1306_send_cmd_2(ADDR_MODE, 0x10);
	if((x_coord <= 127) && (y_coord <= 63)){
		y_coord = (y_coord == 0) ? 0 : y_coord;
		ssd1306_send_cmd_1(0xB0+(y_coord/8));
		while((y_coord/8)!=0) y_coord-=8;
		ssd1306_send_cmd_1(0x00|(x_coord&0x0F));
		ssd1306_send_cmd_1(0x10|((x_coord&0xF0)>>4));
		ssd1306_send_data(1<<(y_coord%8));
	}
}

void ssd1306_clear_pixel(int8_t x_coord, int8_t y_coord){
	//ssd1306_send_cmd_2(ADDR_MODE, 0x10);
	if((x_coord <= 127) && (y_coord <= 63)){
		y_coord = (y_coord == 0) ? 0 : y_coord;
		ssd1306_send_cmd_1(0xB0+(y_coord/8));
		while((y_coord/8)!=0) y_coord-=8;
		ssd1306_send_cmd_1(0x00|(x_coord&0x0F));
		ssd1306_send_cmd_1(0x10|((x_coord&0xF0)>>4));
		ssd1306_send_data((1<<y_coord)&(~(1<<y_coord)));
	}
}

void ssd1306_draw_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2){
	int16_t dx=x2-x1;
	int16_t dy=y2-y1;
	//drawing a line according to linear formula (y-y1)/(y2-y1) = (x-x1)/(x2-x1)
	//understanding where distance is greater - on x axis or y axis
	//on that axis we running a loop, to search for points on another axis
	if (abs(dx) > abs(dy)){
		if (x1 < x2){
			for(int16_t x=x1; x<=x2; x++){
				ssd1306_draw_pixel(x,((x-x1)*dy/dx+y1));
			}
		} 
		else{
			for(int16_t x=x1; x>=x2; x--){
				ssd1306_draw_pixel(x,((x-x1)*dy/dx+y1));
			}
		}
	} 
	else{
		if (y1 < y2){
			for(int16_t y=y1; y<=y2; y++){
				ssd1306_draw_pixel(((y-y1)*dx/dy+x1), y);
			}
		} 
		else{
			for(int16_t y=y1; y>=y2; y--){
				ssd1306_draw_pixel(((y-y1)*dx/dy+x1), y);
			}
		}
	}
}

//uint8_t ssd1306_scrsaver(uint8_t scrsvr_type){
	//ssd1306_clear();
	////ssd1306_set_cursor(61-(FONT_SIZE*2), 3);
	////ssd1306_show_str("TEST");
	//switch(scrsvr_type){
		//case (1):{ //vertical "snake"
			//for(uint8_t j=0; j<=122; j+=FONT_SIZE){
				//if(contrast_flag==0){
					//contrast_flag=1;
					//ssd1306_send_cmd_1(INVERT_ON);
				//}
				//else{
					//contrast_flag=0;
					//ssd1306_send_cmd_1(INVERT_OFF);
				//}
				//if(j<=61){
					//for(int8_t i=0; i<=7; i++){
						//ssd1306_set_cursor(j+1, i);
						//ssd1306_show_char('*');
						//ssd1306_set_cursor(122-j, 7-i);
						//ssd1306_show_char('*');
						//_delay_ms(100);
						//if((EIFR&(1<<0)) != 0) return(0);
					//}
					//j+=FONT_SIZE+3;
					//for(uint8_t i=0; i<=7; i++){
						//ssd1306_set_cursor(j+1, 7-i);
						//ssd1306_show_char('*');
						//ssd1306_set_cursor(122-j, i);
						//ssd1306_show_char('*');
						//_delay_ms(100);
						//if((EIFR&(1<<0)) != 0) return(0);
					//}
					//j+=(FONT_SIZE+3)-FONT_SIZE;
				//}
				//if(contrast_flag==0){
					//contrast_flag=1;
					//ssd1306_send_cmd_1(INVERT_ON);
				//}
				//else{
					//contrast_flag=0;
					//ssd1306_send_cmd_1(INVERT_OFF);
				//}
				//if(j>61){
					//for(int8_t i=0; i<=7; i++){
						//ssd1306_set_cursor(j+1, i);
						//ssd1306_show_char(' ');
						//ssd1306_set_cursor(122-j-1, 7-i);
						//ssd1306_show_char(' ');
						//_delay_ms(100);
						//if((EIFR&(1<<0)) != 0) return(0);
					//}
					//j+=FONT_SIZE+3;
					//for(uint8_t i=0; i<=7; i++){
						//ssd1306_set_cursor(j+1, 7-i);
						//ssd1306_show_char(' ');
						//ssd1306_set_cursor(122-j-1, i);
						//ssd1306_show_char(' ');
						//_delay_ms(100);
						//if((EIFR&(1<<0)) != 0) return(0);
					//}
					//j+=(FONT_SIZE+3)-FONT_SIZE;
				//}
			//}
			//break;
		//}
		//case (2):{ //horizontal "snake"
			//for(uint8_t i=0; i<=7; i++){
				//if(contrast_flag==0){
					//contrast_flag=1;
					//ssd1306_send_cmd_1(INVERT_ON);
				//}
				//else{
					//contrast_flag=0;
					//ssd1306_send_cmd_1(INVERT_OFF);
				//}
				//if(i<=3){
					//for(uint8_t j=0; j<=122; j+=FONT_SIZE){
						//ssd1306_set_cursor(j+1, i);
						//ssd1306_show_char('*');
						//ssd1306_set_cursor(121-j, 7-i);
						//ssd1306_show_char('*');
						//j+=(FONT_SIZE+3)-FONT_SIZE;
						//_delay_ms(100);
						//if((EIFR&(1<<0)) != 0) return(0);
					//}
					//i++;
					//for(uint8_t j=0; j<=122; j+=FONT_SIZE){
						//ssd1306_set_cursor(121-j, i);
						//ssd1306_show_char('*');
						//ssd1306_set_cursor(j+1, 7-i);
						//ssd1306_show_char('*');
						//j+=(FONT_SIZE+3)-FONT_SIZE;
						//_delay_ms(100);
						//if((EIFR&(1<<0)) != 0) return(0);
					//}
				//}
				//if(contrast_flag==0){
					//contrast_flag=1;
					//ssd1306_send_cmd_1(INVERT_ON);
				//}
				//else{
					//contrast_flag=0;
					//ssd1306_send_cmd_1(INVERT_OFF);
				//}
				//if(i>3){
					//for(uint8_t j=0; j<=122; j+=FONT_SIZE){
						//ssd1306_set_cursor(j+1, i);
						//ssd1306_show_char(' ');
						//ssd1306_set_cursor(121-j, 7-i);
						//ssd1306_show_char(' ');
						//j+=(FONT_SIZE+3)-FONT_SIZE;
						//_delay_ms(100);
						//if((EIFR&(1<<0)) != 0) return(0);
					//}
					//i++;
					//for(uint8_t j=0; j<=122; j+=FONT_SIZE){
						//ssd1306_set_cursor(121-j, i);
						//ssd1306_show_char(' ');
						//ssd1306_set_cursor(j+1, 7-i);
						//ssd1306_show_char(' ');
						//j+=(FONT_SIZE+3)-FONT_SIZE;
						//_delay_ms(100);
						//if((EIFR&(1<<0)) != 0) return(0);
					//}
				//}
			//}
			//break;
		//}
		//case (3):{ //two circles horizontal
			//for(uint8_t k=0; k<2; k++){
				//for(uint8_t i=0; i<=3; i++){
					//if(contrast_flag==0){
						//contrast_flag=1;
						//ssd1306_send_cmd_1(INVERT_ON);
					//}
					//else{
						//contrast_flag=0;
						//ssd1306_send_cmd_1(INVERT_OFF);
					//}
					//for(uint8_t j=1; j<=122; j++){
						//ssd1306_set_cursor(j, i);
						//ssd1306_show_char(0x20+95);
						//ssd1306_set_cursor(123-j, 7-i);
						//ssd1306_show_char(0x20+95);
						//_delay_ms(25);
						//ssd1306_set_cursor(j, i);
						//ssd1306_show_char(' ');
						//ssd1306_set_cursor(123-j, 7-i);
						//ssd1306_show_char(' ');
						////						if((EIFR&(1<<0)) != 0) return(0);
						//if(flag != 2) return(0);
					//}
					//if(contrast_flag==0){
						//contrast_flag=1;
						//ssd1306_send_cmd_1(INVERT_ON);
					//}
					//else{
						//contrast_flag=0;
						//ssd1306_send_cmd_1(INVERT_OFF);
					//}
					//i++;
					//for(uint8_t j=1; j<=122; j++){
						//ssd1306_set_cursor(123-j, i);
						//ssd1306_show_char(0x20+95);
						//ssd1306_set_cursor(j, 7-i);
						//ssd1306_show_char(0x20+95);
						//_delay_ms(25);
						//ssd1306_set_cursor(123-j, i);
						//ssd1306_show_char(' ');
						//ssd1306_set_cursor(j, 7-i);
						//ssd1306_show_char(' ');
						////if((EIFR&(1<<0)) != 0) return(0);
						//if(flag != 2) return(0);
					//}
				//}
				//for(int8_t i=0; i<=3; i++){
					//if(contrast_flag==0){
						//contrast_flag=1;
						//ssd1306_send_cmd_1(INVERT_ON);
					//}
					//else{
						//contrast_flag=0;
						//ssd1306_send_cmd_1(INVERT_OFF);
					//}
					//for(uint8_t j=1; j<=122; j++){
						//ssd1306_set_cursor(j, 3-i);
						//ssd1306_show_char(0x20+95);
						//ssd1306_set_cursor(123-j, 4+i);
						//ssd1306_show_char(0x20+95);
						//_delay_ms(25);
						//ssd1306_set_cursor(j, 3-i);
						//ssd1306_show_char(' ');
						//ssd1306_set_cursor(123-j, 4+i);
						//ssd1306_show_char(' ');
						////if((EIFR&(1<<0)) != 0) return(0);
						//if(flag != 2) return(0);
					//}
					//if(contrast_flag==0){
						//contrast_flag=1;
						//ssd1306_send_cmd_1(INVERT_ON);
					//}
					//else{
						//contrast_flag=0;
						//ssd1306_send_cmd_1(INVERT_OFF);
					//}
					//i++;
					//for(uint8_t j=1; j<=122; j++){
						//ssd1306_set_cursor(123-j, 3-i);
						//ssd1306_show_char(0x20+95);
						//ssd1306_set_cursor(j, 4+i);
						//ssd1306_show_char(0x20+95);
						//_delay_ms(25);
						//ssd1306_set_cursor(123-j, 3-i);
						//ssd1306_show_char(' ');
						//ssd1306_set_cursor(j, 4+i);
						//ssd1306_show_char(' ');
						////if((EIFR&(1<<0)) != 0) return(0);
						//if(flag != 2) return(0);
					//}
				//}
			//}
			//break;
		//}
	//}
	//return(1);
//}