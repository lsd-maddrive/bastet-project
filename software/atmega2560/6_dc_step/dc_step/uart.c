#include "uart.h"

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
			UartTransmitByte('-');
		}
		/* проверяем количество нулевых разрядов в пятиразрядном числе, 
		например у числа 64 будет три нулевых незначащих разряда – 00064 */
		j=1;
		while(!(Digit(val, j++)));
		// не выводим незначащие нули в терминале
		for(i=j-1; i<=5; i++) UartTransmitByte(Digit(val, i)+'0');
	}
	/* если значение ноль – выводим ноль, иначе зависнем 
	в цикле */
	else UartTransmitByte('0');
}

// функция передачи строки, слова или фразы
void UartSendStr(uint8_t* string){
	/* передаём данные побайтово с помощью указателя *
	указатель – адрес «первого элемента» передаваемых данных*/
	while(*string) UartTransmitByte(*string++);
}

// посылка hex числа с МК в терминал
void UartSendHex(uint16_t val){
	uint8_t msb=(val & 0xF0) >> 4, lsb=val & 0x0F;
	msb += msb > 9 ? 'A' - 10 : '0';
	lsb += lsb > 9 ? 'A' - 10 : '0';	
	UartTransmitByte(msb);
	UartTransmitByte(lsb);
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

#define INTERRUPTS
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
	//скорость передачи 19200 бод
	UBRR0H=0;
	//для значения UBRR0L = 51 = 0x33	UBRR0L=0x33;
	UBRR0L=0x33;
	// выводим в терминал слово TEST_OK
	UartSendStr(word);
	// переходим в начало следующей строки
	UartTransmitByte('\r');
}

//чтобы работали прерывания, нужно в UCSR0B в UartInit() выставить бит RXCIE0
float UartReceiveData(void){
	return(angle_received);
}

ISR(USART0_RX_vect){
	digit_received=UDR0;
	if(digit_received=='\r') complete_flag=0;
	if(digit_received=='\n') complete_flag=1;
	if(digit_received=='-') negative_flag=1;
	if((digit_received>='0') && (digit_received<='9')){
		digit_received-='0';
		data_received*=10;
		data_received+=digit_received;
	}
	if(complete_flag==1){
		if(negative_flag==1) data_received=-data_received;
		angle_received=data_received;
		complete_flag=0;
		negative_flag=0;
		data_received=0;
	}
}

