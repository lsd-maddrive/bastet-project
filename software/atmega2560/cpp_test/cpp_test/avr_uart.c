#include <avr/io.h>

#define F_CPU 16000000
#define BAUD 57600
#define UBRR_VAL ((F_CPU / (16UL * BAUD)) - 1)


// Initialize the UART
void avr_uart_init(void)
{
  // Enable bidirectional UART
  UCSR0B |= _BV(RXEN0) | _BV(TXEN0);
  // Use 8-bit characters
  UCSR0C |= _BV(UCSZ00) | _BV(UCSZ01);
	
  // Set the Baud rate
  UBRR0H = (UBRR_VAL >> 8);
  UBRR0L = UBRR_VAL;  
}

//void UartSendStr(uint8_t* string){
	///* передаЄм данные побайтово с помощью указател€ *
	//указатель Ц адрес Ђпервого элементаї передаваемых данных*/
	//while(*string) avr_uart_send_byte(*string++);
//}

// Send one char (blocking)
void avr_uart_send_byte(uint8_t tx_byte)
{
  // Wait to be able to transmit
  while((UCSR0A & _BV(UDRE0)) == 0)
    asm volatile("nop"::);
  // Put the data into the send buffer
  UDR0 = tx_byte;
}


// Get one char if available, otherwise -1
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
