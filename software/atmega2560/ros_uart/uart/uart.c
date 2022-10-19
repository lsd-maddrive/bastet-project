//#include "uart.h"
#include <avr/io.h>
#define F_CPU 16000000

#define BAUD 57600
#define UBRR_VAL ((F_CPU / (16UL * BAUD)) - 1)

float data_received=0, angle_received=0;
uint8_t digit_received=0, negative_flag=0, complete_flag=0;

// функция передачи байта
void UartTransmitByte(uint8_t byte){
	/* ожидаем выставления бита UDRE0, а затем кладем посылку в UDR0 */
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0=byte;
}

// функция приёма байта
uint8_t UartReceiveByte(void){
	/* ожидаем выставления бита RXC0, а затем забираем посылку из UDR0 */
	while(!(UCSR0A & (1<<RXC0)));
	return(UDR0);
}

// посылка десятичного 16-битного числа с МК в терминал
void UartSendDec(int16_t val){
	uint8_t i, j;
	// если посылаемое МК число не ноль
	if(val!=0){
		if(val<0){
			val=-val;
			avr_uart_send_byte('-');
		}
		/* проверяем количество нулевых разрядов в пятиразрядном числе,
		например у числа 64 будет три нулевых незначащих разряда – 00064 */
		j=1;
		while(!(Digit(val, j++)));
		// не выводим незначащие нули в терминале
		for(i=j-1; i<=5; i++) avr_uart_send_byte(Digit(val, i)+'0');
	}
	/* если значение ноль – выводим ноль, иначе зависнем
	в цикле */
	else avr_uart_send_byte('0');
}


void avr_uart_send_byte(int16_t tx_byte)
{
	// Wait to be able to transmit
	while((UCSR0A & _BV(UDRE0)) == 0)
	asm volatile("nop"::);
	// Put the data into the send buffer
	UDR0 = tx_byte;
}


int16_t avr_uart_receive_byte(void)
{
	if((UCSR0A & _BV(RXC0)) != 0)
	{
		return UDR0;
	}
	else
	{
		return -1;
	}
}

void write(uint8_t* data, int length)
{
	for(int i = 0; i < length; i++)
	{
		avr_uart_send_byte(data[i]);
	}
}

// функция передачи строки, слова или фразы
void UartSendStr(uint8_t* string){
	/* передаём данные побайтово с помощью указателя *
	указатель – адрес «первого элемента» передаваемых данных*/
	while(*string) UartTransmitByte(*string++);
}


// приём десятичного 16-битного числа на МК с терминала
int16_t UartReceiveDec(void){
	int16_t data=0;
	uint8_t digit=0, negative_flag=0;
	// принимаем пятиразрядное число
	do{
		digit=UartReceiveByte();
		// принимаем, пока не увидим признак конца строки
		if(digit=='\r') continue;
		if(digit=='\n') break;
		//if(!((digit>='0') && (digit<='9'))) break;
		// выводим ASCII коды цифр
		if(digit=='-') negative_flag=1;
		else{
			digit-='0';
			data*=10;
			data+=digit;
		}
	}
	while(1);
	if(negative_flag==1) data=-data;
	// заканчиваем, когда увидели признак \r\n
	return(data);
}

#define NO_INTERRUPTS
/* функция инициализации: настройка МК на приём и передачу данных,
длина сообщения – 8 бит, один СТОП бит, без паритета, скорость обмена – 19200 бод */
void UartInit(void){
	uint8_t word[]={"UART_OK"};
	#ifdef INTERRUPTS
	UCSR0B|=(1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);
	#endif
	#ifdef NO_INTERRUPTS
	UCSR0B|=(1<<RXEN0)|(1<<TXEN0);
	#endif
	UCSR0C|=(1<<UCSZ01)|(1<<UCSZ00);
	//скорость передачи 57600 бод
	UBRR0H = (UBRR_VAL >> 8);
	UBRR0L = UBRR_VAL;
	// выводим в терминал слово TEST_OK
	UartSendStr(word);
	// переходим в начало следующей строки
	UartTransmitByte('\r');
}

//чтобы работали прерывания, нужно в UCSR0B в UartInit() выставить бит RXCIE0
float UartReceiveData(void){
	return(angle_received);
}

//ISR(USART0_RX_vect){
//digit_received=UDR0;
//if(digit_received=='\r') complete_flag=0;
//if(digit_received=='\n') complete_flag=1;
//if(digit_received=='-') negative_flag=1;
//if((digit_received>='0') && (digit_received<='9')){
//digit_received-='0';
//data_received*=10;
//data_received+=digit_received;
//}
//if(complete_flag==1){
//if(negative_flag==1) data_received=-data_received;
//angle_received=data_received;
//complete_flag=0;
//negative_flag=0;
//data_received=0;
//}
//}