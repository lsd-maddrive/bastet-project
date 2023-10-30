#include "buzzer.h"

void buzzer_init(void){
	DDRB|=(1<<BUZZ_PIN);	
}