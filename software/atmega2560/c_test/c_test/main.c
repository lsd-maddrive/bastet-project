#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

void Tim0Init(void);


uint16_t timer_counter=0;


int main(void)
{
	Tim0Init();
	DDRB|=(1<<7);
	sei();
    /* Replace with your application code */
    while (1) 
    {
    }
}


void Tim0Init(void){
	////timer in normal mode
	TCCR0B|=(1<<CS01) | (1<<CS00); //prescaler 64
	TIMSK0|=(1<<TOIE0);
}

ISR(TIMER0_OVF_vect){
	if (timer_counter < 1000) timer_counter++;
	else{
		PORTB^=(1 << 7);		
		timer_counter=0;
	}
		
	//if (timer_counter < 100) timer_counter++;
	//else
	//{
		//PORTB^=(1 << 7);
		//timer_counter=0;
	//}
}