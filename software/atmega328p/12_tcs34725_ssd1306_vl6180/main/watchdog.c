#include "watchdog.h"

void watchdog_init(void){
	cli();
	MCUSR&=~(1<<WDRF);
	WDTCSR|=(1<<WDCE)|(1<<WDE);
	WDTCSR=(1<<WDIE)|(1<<WDP2)|(1<<WDP1); //period - 1s
	sei();
}