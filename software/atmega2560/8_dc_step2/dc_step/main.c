#include "include/utils.h"
#include "include/st_mot.h"
#include "include/dc_motor.h"
#include "include/uart.h"

void InitAll(void);
void GoRobot(int16_t speed);
void Cmd(uint16_t speed);
void ShowMotorAngles();
void ShowSetAngles(uint8_t angle);
void ShowSpeeds();
void ShowFormuls(uint8_t data, uint8_t angle);

int main(void)
{
	InitAll();
	float data = 0;
	float angle = 0;
	while (1)
	{
		data = UartReceiveData();
		// data = 0;
		if ((data >= -90) & (data <= 90))
		{
			angle = data;
			SetAngle(angle);
		}
		else if (data == 1000)
			SetSpeed(0, 0);
		else
			SetSpeed(data, angle);

		// ShowMotorAngles();
		// ShowSpeeds();
		// ShowFormuls(data, angle);
		ShowSetAngles(angle);

	}
}

void InitAll(void)
{
	UartInit();
	DDRB |= (1 << 7);
	AdcInit();
	StMotInit();
	DcMotInit();
	// SetAngle(1);
	// SetSpeed(0);
	_delay_ms(100);
	sei();
}
void GoRobot(int16_t speed)
{
	SetSpeed(speed, 0);
	_delay_ms(1000);
	SetSpeed(0, 0);
	_delay_ms(500);
}

void ShowMotorAngles()
{
	UartTransmitByte('\t');
	for (int i = 0; i < 4; i++)
	{
		UartSendDec(GetMotPos(i));
		UartTransmitByte('\t');
	}
	UartTransmitByte('\r');
}

void ShowSetAngles(uint8_t angle)
{
	UartTransmitByte('\t');
	for (int i = 0; i < 3; i++)
	{
		UartSendDec(GetAngles(angle)[i]*100);
		UartTransmitByte('\t');
	}
	UartTransmitByte('\r');
}

void ShowSpeeds()
{

	UartTransmitByte('\t');
	for (int i = 0; i < 4; i++)
	{
		UartSendDec(GetSpeed()[i]);
		UartTransmitByte('\t');
	}
	UartTransmitByte('\r');
}

void ShowFormuls(uint8_t data, uint8_t angle)
{
	UartTransmitByte('\t');
	for (int i = 0; i < 6; i++)
	{
		UartSendDec(((int16_t)(GetFormuls(data, angle)[i])*10));
		UartTransmitByte('\t');
	}
	UartTransmitByte('\r');
}

void Cmd(uint16_t speed)
{
	uint16_t robot_speed = speed;

	_delay_ms(1000);
	SetAngle(1);
	_delay_ms(1000);
	GoRobot(robot_speed);
	_delay_ms(1000);
	SetAngle(90);
	_delay_ms(1000);
	GoRobot(robot_speed);
	_delay_ms(1000);
	SetAngle(0);
	_delay_ms(1000);
	GoRobot(-robot_speed);
	_delay_ms(1000);

	SetAngle(-90);
	_delay_ms(1000);
	GoRobot(robot_speed);
	_delay_ms(1000);
}
