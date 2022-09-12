#include "adc.h"

uint16_t pot_res[12]={0,0,0,0,0,0,0,0,0,0,0,0};
float pot_av[4]={0,0,0,0}, filtered_pot[4]={0,0,0,0}, lp_fil_a =0.93, lp_fil_b=0.07;
uint8_t adc_channel=ST_MOT1_POT, measure_number=0;
uint16_t adc_res[4]={0,0,0,0};

void AdcInit(void){
	#ifdef ARM_READ
	
	//ExpPinModePort(EXP_DEF_ADDR, ((1<<0)|(1<<1)|(1<<2)|(1<<3)), INPUT); //pins 0,1,2,3 - input
	//ExpWriteCmd(EXP_DEF_ADDR, EXP_ADC_LOWPASS_FILTER_ON); //ON by default
	//ExpWriteCmd8BitVal(EXP_DEF_ADDR, EXP_ADC_SPEED, 6); //6 by default
	
	#endif
	
	#ifdef AVR_READ
	
	ADCSRA=(1<<ADEN)|(1<<ADSC)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS0);
	ADMUX=(1<<REFS0)|adc_channel;
	
	#endif
}

uint16_t* AdcGetPos(void){
	#ifdef ARM_READ
	
	for(uint8_t i=0; i<4; i++) adc_res[i]=ExpAnalogRead(EXP_DEF_ADDR, i);

	#endif
	return adc_res;
}

ISR(ADC_vect){
	switch(adc_channel){
		case ST_MOT1_POT:{
			pot_res[measure_number]=ADCL|(ADCH<<8);
			adc_channel=ST_MOT2_POT;
			ADMUX=(1<<REFS0)|adc_channel;
			break;
		}
		case ST_MOT2_POT:{
			pot_res[measure_number]=ADCL|(ADCH<<8);
			adc_channel=ST_MOT3_POT;
			ADMUX=(1<<REFS0)|adc_channel;
			break;
		}
		case ST_MOT3_POT:{
			pot_res[measure_number]=ADCL|(ADCH<<8);
			adc_channel=ST_MOT4_POT;
			ADMUX=(1<<REFS0)|adc_channel;
			break;
		}
		case ST_MOT4_POT:{
			pot_res[measure_number]=ADCL|(ADCH<<8);
			adc_channel=ST_MOT1_POT;
			ADMUX=(1<<REFS0)|adc_channel;
			break;
		}
	}
	if(measure_number<11) measure_number++;
	else{
		measure_number=0;
		#ifdef MAX_MIN
		
		adc_res[0]=(FindMaxValue(pot_res[0],pot_res[4],pot_res[8])+FindMinValue(pot_res[0],pot_res[4],pot_res[8]))/2;
		adc_res[1]=(FindMaxValue(pot_res[1],pot_res[5],pot_res[9])+FindMinValue(pot_res[1],pot_res[5],pot_res[9]))/2;
		adc_res[2]=(FindMaxValue(pot_res[2],pot_res[6],pot_res[10])+FindMinValue(pot_res[2],pot_res[6],pot_res[10]))/2;
		adc_res[3]=(FindMaxValue(pot_res[3],pot_res[7],pot_res[11])+FindMinValue(pot_res[3],pot_res[7],pot_res[11]))/2;
		
		#endif
		#ifdef MIDDLE
		
		adc_res[0]=FindMiddleValue(pot_res[0],pot_res[4],pot_res[8]);
		adc_res[1]=FindMiddleValue(pot_res[1],pot_res[5],pot_res[9]);
		adc_res[2]=FindMiddleValue(pot_res[2],pot_res[6],pot_res[10]);
		adc_res[3]=FindMiddleValue(pot_res[3],pot_res[7],pot_res[11]);
		
		#endif
		#ifdef LOW_PASS_FILTER
		
		pot_av[0]=(pot_res[0]+pot_res[4]+pot_res[8])/3;
		pot_av[1]=(pot_res[1]+pot_res[5]+pot_res[9])/3;
		pot_av[2]=(pot_res[2]+pot_res[6]+pot_res[10])/3;
		pot_av[3]=(pot_res[3]+pot_res[7]+pot_res[11])/3;
		for(uint8_t i=0; i<4; i++){
			filtered_pot[i]=lp_fil_a*filtered_pot[i]+lp_fil_b*pot_av[i];
			adc_res[i]=(uint16_t)filtered_pot[i];
		}
		
		#endif
	}
	ADCSRA|=(1<<ADSC);
}