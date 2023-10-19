#include "include/dc_motor.h"
#include <avr/io.h>

//uint16_t tim2_count=0, dc_mot_enc_count = 0, enc_result = 0;
uint16_t tim2_count=0, dc_mot_enc_count[4]={0,0,0,0}, enc_result[4]={0,0,0,0};
float reg_speed[4]={0,0,0,0}, integral[4]={0,0,0,0};
//uint32_t enc_result = 0;
float set_speed = 0;


void Tim2DcMotInit(void){
	//����� PH6(OC2B), PB4(OC2A) � ���, ����� PH4 - �����������
	TCCR2A |= (1<<COM2A1);
	TCCR2A |= (1<<COM2B1);
	TIMSK2 |=(1<<TOIE2);
	//����� PH3(OC4A), PH5(OC4C) � ���
	TCCR4A |= (1<<COM4A1);
	TCCR4A |= (1<<COM4C1);
	/* TIMER2 - ��������� �������: ��� � ������� ����������,
	����������������� �����, ������������ �� 256 122.55hz */
	TCCR2A |= (1<<WGM20);
	TCCR2B |= (1<<CS22) | (1<<CS21);
	/* TIMER4 - ��������� �������: ������� ���,
	����������������� �����, 8 bit, TOP = 0xFF, ������������ �� 256*/
	TCCR4A |= (1 << WGM40);
	TCCR4B |= (1<<CS42);
}

void DcMotInit(void){
	Tim2DcMotInit();
	IntDcMotEcoderInit();
	DC_MOT1_SPEED_DDR|=(1<<DC_MOT1_SPEED_DDR_PIN);
	DC_MOT2_SPEED_DDR|=(1<<DC_MOT2_SPEED_DDR_PIN);
	DC_MOT3_SPEED_DDR|=(1<<DC_MOT3_SPEED_DDR_PIN);
	DC_MOT4_SPEED_DDR|=(1<<DC_MOT4_SPEED_DDR_PIN);
	
	DC_MOT_DIR_DDR|=(1<<DC_MOT_DIR_DDR_PIN);
}


uint16_t* GetSpeed(void){
	return(enc_result);
}


void SetSpeed(float desired_speed){
	if(desired_speed>=0) DC_MOT_FOR;
	else DC_MOT_REV;
	
	set_speed = abs(desired_speed);
}


void DcMotGo(float* speed){

	OCR2A = speed[3];
	OCR2B = speed[2];
	OCR4A = speed[1];
	OCR4C = speed[0];
}

void DcMotPIDGo(float set_speed){
	//float speed_test[4]={0,0,0,0};
	for(uint8_t i=0; i<4; i++){
		//reg_speed[i] = ComputePI(GetSpeed()[i], input_speed);
		reg_speed[i] = ComputePI(GetSpeed()[i], set_speed, i);
	}
	DcMotGo(reg_speed);
}

void IntDcMotEcoderInit(void){
	EICRA=(1<<ISC01) | (1<<ISC11) | (1<<ISC21) | (1<<ISC31);
	EIMSK=(1<<INT0) | (1<<INT1) | (1<<INT2) | (1<<INT3);

}


//

ISR (INT0_vect)
{
	PORTB^=(1<<7);
	dc_mot_enc_count[0]+=1;
}

ISR (INT1_vect)
{
	//PORTB^=(1<<7);
	dc_mot_enc_count[1]+=1;
}

ISR (INT2_vect)
{
	//PORTB^=(1<<7);
	dc_mot_enc_count[2]+=1;
}

ISR (INT3_vect)
{
	//PORTB^=(1<<7);
	dc_mot_enc_count[3]+=1;
}

ISR(TIMER2_OVF_vect){ //isr executes every 8 ms
	if(tim2_count<10) tim2_count++; //every 80 ms
	else{
		for(uint8_t i=0; i<4; i++){
			enc_result[i]=((uint32_t)dc_mot_enc_count[i]*12.5*60)/115; //rev per minute
			dc_mot_enc_count[i]=0;
		}
		DcMotPIDGo(set_speed);
		//PORTB^=(1<<7);
		tim2_count=0;
	}
}


float ComputePI(uint16_t input, float setpoint, uint8_t integral_num){
	float kp = 0.7;
	float ki = 0.3;
	float dt = 0.08;
	float error = setpoint - input;
	integral[integral_num] = integral[integral_num] + (error*dt);
	float control = (error * kp) + (integral[integral_num] * ki);
	if(control > 255) //����������� ������� ���������� ������
	control = 255;
	if(control < 0) //����������� ������� ���������� �����
	control = 0;
	return(control);
}

//previous_error = 0;
//integral = 0;
//OCRnA = 1500;
//CurrentOCR = 1500;
//dt = 50; //for instance
//while(1)
//{
//actual = ReadADC(); //read sonar value
//error = SetPoint - actual; //calculate P error
//integral = integral + (error*dt); //calculate integral
//derivative = (error - previous_error)*(1/dt); //calculate derivative
//output = (PGain*error) + (IGain*integral) + (DGain*derivative); //calculate PID sum
//OCRnA = CurrentOCR + output; //change PWM output based on the PID value
//previous_error = error; //set previous_error to current error
//_delay_ms(dt);
//}




//ISR(TIMER2_OVF_vect){ //isr executes every 8 ms
//if(tim2_count<12) tim2_count++; //every 100 ms
//else{
//SetSpeed(dc_mot_enc_count);
//dc_mot_enc_count=0;
//tim2_count=0;
//}
//}

