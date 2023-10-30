/*
 * main.c
 *
 * Created: 26.01.2022 14:09:03
 * Author : Michael
 */ 

#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
#include <avr/interrupt.h>

//7-битный адрес устройства
#define SENS_ADDR 0x29
//регистры датчика
#define SYSTEM__INTERRUPT_CONFIG_GPIO 0x014
#define SYSTEM__INTERRUPT_CLEAR 0x015
#define SYSTEM__FRESH_OUT_OF_RESET 0x016
#define SYSRANGE__START 0x018
#define SYSALS__START 0x038
#define SYSALS__ANALOGUE_GAIN 0x03F
#define SYSALS__INTEGRATION_PERIOD_HI 0x040
#define SYSALS__INTEGRATION_PERIOD_LO 0x041
#define RESULT__INTERRUPT_STATUS_GPIO 0x04F
#define RESULT__ALS_VAL_HI 0x050
#define RESULT__ALS_VAL_LO 0x051
#define RESULT__RANGE_VAL 0x062
#define FAILURE_RESET -1
//чтение/запись по I2C
#define W 0
#define R 1

void uart_init(void);
void uart_transmit(uint8_t byte);
uint8_t uart_receive(void);
void uart_transmit_str(uint8_t* string);
uint8_t Digit(uint16_t d, uint8_t m);
void uart_transmit_dec(uint16_t val);
uint16_t uart_receive_dec(void);

void i2c_start();
void i2c_stop();
void i2c_write_byte(uint8_t data);
uint8_t i2c_read_byte(uint8_t is_last_byte);
void i2c_send_data(uint8_t dev_addr, uint16_t reg_addr, uint8_t val);
uint8_t i2c_get_data(uint8_t dev_addr, uint16_t reg_addr);
uint8_t vl6180x_init(uint8_t dev_addr);
uint8_t get_dist(uint8_t dev_addr);
uint16_t get_amb_light(uint8_t dev_addr);

uint8_t t1_flag=0; //глобальная переменная для таймера
uint8_t dist_mm; //показания датчика
float amb_light;

int main(void)
{
	_delay_ms(100);
	TCCR1B=(1<<CS11); //период таймера №1 10 мс
	TCNT1H=0xB1; //начальное значение таймера
	TCNT1L=0xE0; 
	TIMSK1 = (1<<TOIE1);
	TWBR = 12; //400 кГц частота I2C
	uart_init();
	vl6180x_init(SENS_ADDR);
	_delay_ms(100);
	sei();
	while(1);//здесь можем считать dist_mm
}

/* функция инициализации: настройка МК на приём и передачу данных, длина сообщения – 8 бит, один СТОП бит, без паритета, скорость обмена – 9600 бод */
void uart_init(void){
  uint8_t word[] = {"TEST"};
  UCSR0B |= (1<<RXEN0) | (1<<TXEN0);
  UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);
  //скорость передачи 9600 бод
  UBRR0H = 0;
  //для значения 9600 UBRR0L = 103 = 0x67
  UBRR0L = 0x67;
  // выводим в терминал слово TEST
  uart_transmit_str(word);
  // переходим в начало следующей строки
  uart_transmit('\r');
}

// функция передачи байта
void uart_transmit(uint8_t byte){
  /* ожидаем выставления бита UDRE0, а затем кладем
  посылку в UDR0 */
  while(!(UCSR0A & (1<<UDRE0)));
  UDR0 = byte;
}

// функция приёма байта
uint8_t uart_receive(void){
  /* ожидаем выставления бита RXC0, а затем забираем
  посылку из UDR0 */
  while(!(UCSR0A & (1<<RXC0)));
  return UDR0;
}

// функция передачи строки, слова или фразы
void uart_transmit_str(uint8_t* string){
/* передаём данные побайтово с помощью указателя *
указатель – адрес «первого элемента» передаваемых данных*/
  while(*string)
    uart_transmit(*string++);
}

uint8_t Digit(uint16_t d, uint8_t m){
  uint8_t i = 5, a;
  while(i){ //цикл по разрядам числа
    a = d%10; //выделяем очередной разряд
    //выделен заданный разряд - уходим
    if(i-- == m) break; 
    d /= 10; //уменьшаем число в 10 раз
    }
  return(a);
}
// посылка десятичного 16-битного числа с МК в терминал
void uart_transmit_dec(uint16_t val){
  uint8_t i, j;
// если посылаемое МК число не ноль
  if(val!=0)
  {
    /* проверяем количество нулевых разрядов в 
    пятиразрядном числе, например у числа 64 будет три 
    нулевых незначащих разряда – 00064 */
    j=1;
    while(!(Digit(val, j++)));
    // не выводим незначащие нули в терминале
    for(i=j-1; i<=5; i++) uart_transmit(Digit(val, i)+'0');
  }
  /* если значение ноль – выводим ноль, иначе зависнем 
  в цикле */
  else 
    uart_transmit('0');
}

// приём десятичного 16-битного числа на МК с терминала
uint16_t uart_receive_dec(void){
  uint16_t data = 0;
  uint8_t digit = 0;
// принимаем пятиразрядное число
  do
  {
    digit = uart_receive();
    // принимаем, пока не увидим признак конца строки
    if(digit == '\r') continue;
    if(digit == '\n') break;
    if(!((digit >= '0') && (digit <= '9'))) break;
    // выводим ASCII коды цифр
    digit -= '0';
    data *= 10;
    data += digit;
  }
  while(1);
  // заканчиваем, когда увидели признак \r\n
  return data;
}

void i2c_start(void)
{
	TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while(!(TWCR&(1<<TWINT)));
}
void i2c_stop(void)
{
	TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}

void i2c_write_byte(uint8_t data)
{
	TWDR=data;
	TWCR=(1<<TWINT)|(1<<TWEN);
	while(!(TWCR&(1<<TWINT)));
}

uint8_t i2c_read_byte(uint8_t is_last_byte)
{
	if(is_last_byte) TWCR=(1<<TWINT)|(1<<TWEN);
	else TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while(!(TWCR&(1<<TWINT)));
	return(TWDR);
}

void i2c_send_data(uint8_t dev_addr, uint16_t reg_addr, uint8_t val)
{
	uint8_t reg_hbv=(reg_addr>>8), reg_lbv=reg_addr;
	i2c_start();
	i2c_write_byte((dev_addr<<1) | W);
	i2c_write_byte(reg_hbv);
	i2c_write_byte(reg_lbv);
	i2c_write_byte(val);
	i2c_stop();
}

uint8_t i2c_get_data(uint8_t dev_addr, uint16_t reg_addr)
{
	uint8_t val, reg_hbv=(reg_addr>>8), reg_lbv=reg_addr;
	i2c_start();
	i2c_write_byte((dev_addr<<1) | W);
	i2c_write_byte(reg_hbv);
	i2c_write_byte(reg_lbv);
	i2c_start();
	i2c_write_byte((dev_addr<<1) | R);
	val = i2c_read_byte(1);
	i2c_stop();
	return val;
}

uint8_t vl6180x_init(uint8_t dev_addr)
{
	if((i2c_get_data(dev_addr, SYSTEM__FRESH_OUT_OF_RESET)) != 1)
	return(FAILURE_RESET);
	i2c_send_data(dev_addr, SYSTEM__FRESH_OUT_OF_RESET, 0x00);
	/* настройка: адрес датчика, адрес внутреннего регистра, записываемые в регистр величины */
	i2c_send_data(dev_addr, 0x0207, 0x01);
	i2c_send_data(dev_addr, 0x0208, 0x01);
	i2c_send_data(dev_addr, 0x0096, 0x00);
	i2c_send_data(dev_addr, 0x0097, 0xfd);
	i2c_send_data(dev_addr, 0x00e3, 0x00);
	i2c_send_data(dev_addr, 0x00e4, 0x04);
	i2c_send_data(dev_addr, 0x00e5, 0x02);
	i2c_send_data(dev_addr, 0x00e6, 0x01);
	i2c_send_data(dev_addr, 0x00e7, 0x03);
	i2c_send_data(dev_addr, 0x00f5, 0x02);
	i2c_send_data(dev_addr, 0x00d9, 0x05);
	i2c_send_data(dev_addr, 0x00db, 0xce);
	i2c_send_data(dev_addr, 0x00dc, 0x03);
	i2c_send_data(dev_addr, 0x00dd, 0xf8);
	i2c_send_data(dev_addr, 0x009f, 0x00);
	i2c_send_data(dev_addr, 0x00a3, 0x3c);
	i2c_send_data(dev_addr, 0x00b7, 0x00);
	i2c_send_data(dev_addr, 0x00bb, 0x3c);
	i2c_send_data(dev_addr, 0x00b2, 0x09);
	i2c_send_data(dev_addr, 0x00ca, 0x09);
	i2c_send_data(dev_addr, 0x0198, 0x01);
	i2c_send_data(dev_addr, 0x01b0, 0x17);
	i2c_send_data(dev_addr, 0x01ad, 0x00);
	i2c_send_data(dev_addr, 0x00ff, 0x05);
	i2c_send_data(dev_addr, 0x0100, 0x05);
	i2c_send_data(dev_addr, 0x0199, 0x05);
	i2c_send_data(dev_addr, 0x01a6, 0x1b);
	i2c_send_data(dev_addr, 0x01ac, 0x3e);
	i2c_send_data(dev_addr, 0x01a7, 0x1f);
	i2c_send_data(dev_addr, 0x0030, 0x00);
	return(0);
}

uint8_t get_dist(uint8_t dev_addr)
{
	uint8_t result;
	i2c_send_data(dev_addr, SYSTEM__INTERRUPT_CONFIG_GPIO, (1<<2)); //new sample
	i2c_send_data(dev_addr, SYSRANGE__START, (1<<0)); //1-shot режим
	while((i2c_get_data(dev_addr, RESULT__INTERRUPT_STATUS_GPIO)) != (1<<2));
	result = i2c_get_data(dev_addr, RESULT__RANGE_VAL);
	i2c_send_data(dev_addr, SYSTEM__INTERRUPT_CLEAR, ((1<<2)|(1<<1)|(1<<0)));
	return result;
}

uint16_t get_amb_light(uint8_t dev_addr)
{
	uint16_t result;
	i2c_send_data(dev_addr, SYSTEM__INTERRUPT_CONFIG_GPIO, (1<<5)); //new sample
	i2c_send_data(dev_addr, SYSALS__INTEGRATION_PERIOD_HI, 0); //integration period is
	i2c_send_data(dev_addr, SYSALS__INTEGRATION_PERIOD_LO, 0x63); //100 ms
	i2c_send_data(dev_addr, SYSALS__ANALOGUE_GAIN, 0x46); //gain = 1
	i2c_send_data(dev_addr, SYSALS__START, (1<<0));
	_delay_ms(100);
	while((i2c_get_data(dev_addr, RESULT__INTERRUPT_STATUS_GPIO)) != (1<<5));
	result = i2c_get_data(dev_addr, RESULT__ALS_VAL_LO);
	result |= (i2c_get_data(dev_addr, RESULT__ALS_VAL_HI))<<8; 
	i2c_send_data(dev_addr, SYSTEM__INTERRUPT_CLEAR, ((1<<2)|(1<<1)|(1<<0)));
	return result;	
}

/* по таймеру 1, раз в секунду измеряем расстояние и освещённость в автоматическом режиме */
ISR (TIMER1_OVF_vect){
	TCNT1H=0xB1; //начальное значение таймера
	TCNT1L=0xE0;
	if(t1_flag<100) t1_flag++;
	else{
		t1_flag=0;
		dist_mm = get_dist(SENS_ADDR);
		uart_transmit_dec(dist_mm);
		uart_transmit('\t');
		amb_light = (float)get_amb_light(SENS_ADDR)*0.32; //from datasheet when gain=1 and period 100 ms
		uart_transmit_dec((uint16_t)(amb_light));
		uart_transmit('\r');
	}
}
