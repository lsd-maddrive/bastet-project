#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define W 0
#define R 1
//7-bit address of ssd1306
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
#define FONT_SIZE 5

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

uint8_t ssd1306_line_pos, ssd1306_col_pos;
float crgb[4]; //clear red green blue
float hsv[3]; //hue saturation value

uint8_t colors[][11]={"Black  ", "White  ", "Red    ", "Orange ", "Yellow ", "Green  ", "Cyan   ", "Blue   ", "Magenta", "Pink   ", "N/D"};
uint8_t fin_color_word[]={"Prb. color: "};
float *crgb_res, *hsv_res; 
uint8_t color_res=0;

const uint8_t FontTable[][FONT_SIZE] PROGMEM = {
//                                          hex ASCII
  {0x00, 0x00, 0x00, 0x00, 0x00},   // space 00   20
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
  {0x55, 0xAA, 0x55, 0xAA, 0x55},   //       60   5c
  {0x00, 0x41, 0x41, 0x7F, 0x00},   // ]     61   5d
  {0x04, 0x02, 0x01, 0x02, 0x04},   // ^     62   5e
  {0x40, 0x40, 0x40, 0x40, 0x40},   // _     63   5f
  {0x00, 0x03, 0x05, 0x00, 0x00},   // `     64   60
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
  {0x00, 0x06, 0x09, 0x09, 0x06}    // ~     94   7e
};

void i2c_init(void){
	TWBR=12; //400 kHz
}

void i2c_start(void){
	TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while(!(TWCR&(1<<TWINT)));
}

void i2c_stop(void){
	TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}

void i2c_transmit_byte(uint8_t data){
	TWDR=data;
	TWCR=(1<<TWINT)|(1<<TWEN);
	while(!(TWCR&(1<<TWINT)));
}

uint8_t i2c_receive_byte(uint8_t is_last_byte){
	if(is_last_byte) TWCR=(1<<TWINT)|(1<<TWEN);
	else TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while(!(TWCR&(1<<TWINT)));
	return(TWDR);
}

uint8_t Digit(uint16_t d, uint8_t m){
	uint8_t i = 3, a;
	while(i) //цикл по разрядам числа
	{
		a = d%10; //выделяем очередной разряд
		if(i-- == m) break; //выделен заданный разряд-уходим
		d /= 10; //уменьшаем число в 10 раз
	}
	return(a);
}

float find_max_out_of_3(float a, float b, float c){
	if((a != b) && (a != c) && (b != c)){
		if(a > b){
			if(a > c) return(a);
			else return(c);
		}
		else{
			if(b > c) return(b);
			else return(c);
		}
	}
	else{
		if(a == b){
			if(a > c) return(a);
			else return(c);
		}
		else{
			if(a == c){
				if(a > b) return(a);
				else return(b);
			}
			else{
				if(b == c){
					if(b > a) return(b);
					else return(a);
				}
			}
		}
	}
	return(0);
}

float find_min_out_of_3(float a, float b, float c){
	if((a != b) && (a != c) && (b != c)){
		if(a < b){
			if(a < c) return(a);
			else return(c);
		}
		else{
			if(b < c) return(b);
			else return(c);
		}
	}
	else{
		if(a == b){
			if(a < c) return(a);
			else return(c);
		}
		else{
			if(a == c){
				if(a < b) return(a);
				else return(b);
			}
			else{
				if(b == c){
					if(b < a) return(b);
					else return(a);
				}
			}
		}
	}
	return(0);
}

void ssd1306_cmd(uint8_t *data_buf, uint8_t length){
  i2c_start();
  i2c_transmit_byte((DISPLAY_ADDR<<1) | W);
  //признак команды (D/C# в i2c диаграмме = 0)
  i2c_transmit_byte(0x00);
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
  //признак данных (D/C# в i2c диаграмме = 1)
  i2c_transmit_byte(0x40); //?0b01000000???
  i2c_transmit_byte(data);
  i2c_stop();
}

void ssd1306_set_cursor(uint8_t col_pos, uint8_t line_pos){
	if((line_pos <= 7) && (col_pos <= 127)){
		// глобальная переменная текущий номер строки
		ssd1306_line_pos = line_pos;
		// глобальная переменная текущий номер столбца
		ssd1306_col_pos = col_pos;
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

void ssd1306_next_line(void){
	ssd1306_line_pos++;
	ssd1306_line_pos = ssd1306_line_pos & 0x07;
	ssd1306_set_cursor(0, ssd1306_line_pos);
}

void ssd1306_clear(){
  ssd1306_send_cmd_3(SET_COLUMN_ADDR, 0, 127);
  ssd1306_send_cmd_3(SET_PAGE_ADDR, 0, 7);
  /* 128 байт (по 8 вертикальных бит) на строку * 8 
  строк = 1024 */
  for(uint8_t lineCnt=0; lineCnt<8; lineCnt++){
  //(LCDWIDTH*LCDHEIGHT/8)
    for(uint8_t index=0; index<128; index++) ssd1306_send_data(0x00);
  }
  ssd1306_set_cursor(0,0);
}

void ssd1306_show_char(uint8_t data){
  uint8_t charColumnBitMap, charBitMapIndex = 0;

  if(((ssd1306_col_pos + FONT_SIZE) >= 128) || (data=='\n')){ // font size=5
    /* если на строке больше нет места для записи и 
    подана команда перехода на новую строку, 
    передвигатем курсор на следующую строку */
    ssd1306_next_line();
  }
  if(data != '\n'){
    // таблица символов начинается с адреса 0x20
    data = data - 0x20; 
    while(1){
      charColumnBitMap = pgm_read_byte(&FontTable[data][charBitMapIndex]);
      ssd1306_send_data(charColumnBitMap);
      ssd1306_col_pos++;
      charBitMapIndex++;
      //выйти из цикла после посылки пяти байт
      if(charBitMapIndex == FONT_SIZE) {
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

uint8_t ssd1306_init(uint8_t dev_addr){
  //включить дисплей
  ssd1306_send_cmd_1(DISPLAY_OFF);
  //выставить частоту обновления экрана
  ssd1306_send_cmd_2(SET_DISPLAY_CLOCK, 0x80);
  //коэффициент мультиплексирования COM выводов
  ssd1306_send_cmd_2(SET_MULTIPLEX_RATIO, 0x3F);
  //вертикальное смещение дисплея
  ssd1306_send_cmd_2(SET_DISPLAY_OFFSET, 0x00);
  //смещение ОЗУ
  ssd1306_send_cmd_1(SET_START_LINE | 0x00);
  //схема питания (0x14 - внутренний DC-DC вкл, 0x10 - выкл)
  ssd1306_send_cmd_2(CHARGE_DCDC_PUMP, 0x14);
  //адресация (0x00-гориз.,  0x01-верт., 0x10-стран.)
  ssd1306_send_cmd_2(ADDR_MODE, 0x00);
  //строчная развертка (left-to-right)
  ssd1306_send_cmd_1(SET_REMAP_L_TO_R);
  //кадровая развертка (top-to-bottom)
  ssd1306_send_cmd_1(SET_REMAP_T_TO_D);
  //аппаратная конфигурация COM выводов
  ssd1306_send_cmd_2(SET_COM_PINS, 0x12);
  //установка контраста (0x00...0xFF)
  ssd1306_send_cmd_2(SET_CONTRAST, 0xCF);  
  //схема работы DC-DC преобразователя
  //(0xF1 - Vcc берется с DC/DC преобразователя
  // 0x22 - Vcc беретсяс внешнего источника)
  ssd1306_send_cmd_2(SET_PRECHARGE_PERIOD, 0xF1);
  /*устанавливаем питание светодиодов VcomH в значение 
  выше чем по умолчанию 0x30: 0x00, 0x10, 0x20, 0x30, 
  0x40, 0x50, 0x60, 0x70 */
  ssd1306_send_cmd_2(SET_VCOM_DESELECT, 0x40);
  //разрешение отображения RAM памяти
  ssd1306_send_cmd_1(RAM_ON);
  //отключение инверсию цвета
  ssd1306_send_cmd_1(INVERT_OFF);
  //включить дисплей
  ssd1306_send_cmd_1(DISPLAY_ON);
  //очистить дисплей
  ssd1306_clear();
  return(0);
}

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
	_delay_ms(3);
	OCR0A = 1;
	_delay_ms(100);
	OCR0A = 0;
	return(0);
}

float* tcs34725_get_rgb(void){
	uint16_t raw_clear[3]={0,0,0}, raw_red[3]={0,0,0}, raw_green[3]={0,0,0}, raw_blue[3]={0,0,0};
	tcs34725_send_data(ENABLE, (1<<0)|(1<<1)); //power supply on and prepare for color recognition
	//making three measurements with different ambient light (LED on the sensor`s pcb)
	cli();
	for(uint8_t i=0; i<3; i++){
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
		_delay_ms(30); //delay = (256-0xF6)*2.4 = 24мс -> 30мс (0xF6 <- ATIME)
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

int main(void){
	TCCR0A=(1<<COM0A1)|(1<<WGM00); //tcs34725 led (A-channel)
	TCCR0B=(1<<CS00); //prescaler - 1
	DDRD|=(1<<LED_PIN);
	i2c_init();
	ssd1306_init(DISPLAY_ADDR);
	tcs34725_init(RGB_SENSOR_ADDR);
	uint8_t oled_rdy[]={"OLED: rdy"};
	ssd1306_set_cursor(64-((FONT_SIZE+1)*(sizeof(oled_rdy)-1)/2),1);
	ssd1306_show_str(oled_rdy);
	uint8_t tcs_rdy[]={"TCS: rdy 0x"};
	ssd1306_set_cursor(64-((FONT_SIZE+1)*(sizeof(tcs_rdy)+1)/2),3);
	ssd1306_show_str(tcs_rdy);
	ssd1306_show_hex(tcs34725_get_data(ID)); //slave address of rgb sensor
	while(1){
		crgb_res=tcs34725_get_rgb();
		hsv_res=tcs34725_calc_hsv();
		color_res=tcs34725_get_color();
		ssd1306_set_cursor(1,5);
		ssd1306_show_str(fin_color_word);
		ssd1306_show_str(colors[color_res]);
		_delay_ms(1000);
	}
}

