#include "ros.h"
#include "std_msgs/Float32MultiArray.h"
#include "kitty_msgs/KittyState.h"
#include "std_msgs/String.h"

// Include C headers (ie, non C++ headers) in this block
extern "C"
{
#include <util/delay.h>
#include "dc_step/st_mot.h"
#include "dc_step/dc_motor.h"
#include "dc_step/utils.h"
#include "dc_step/adc.h"
}

void InitAll(void);

// Needed for AVR to use virtual functions
extern "C" void __cxa_pure_virtual(void);
void __cxa_pure_virtual(void) {}

ros::NodeHandle nh;

void messageCb(const kitty_msgs::KittyState &speed_msg)
{
  SetSpeed(speed_msg.rotation_speed, speed_msg.angle_steering);
  SetAngle(speed_msg.angle_steering);
}

ros::Subscriber<kitty_msgs::KittyState> sub("set_speed", &messageCb);

kitty_msgs::KittyState kitty_state;
// std_msgs::Float32MultiArray angle_msg;
ros::Publisher kitty_state_pub("kitty_state", &kitty_state);



int main()
{
  uint32_t lasttime = 0UL;
  // Initialize ROS
  nh.initNode();
  nh.subscribe(sub);
  nh.advertise(kitty_state_pub);

  InitAll();

  while (1)
  {
    // Send the message every 0.2 second
    if (avr_time_now() - lasttime > 200)
    {

      PORTB ^= (1 << PB7);
      // kitty_state.rotation_speed = ;
      // kitty_state.angle_steering =  ;

      kitty_state.front_right_rotation_speed = GetSpeed()[1];
      kitty_state.front_left_rotation_speed = GetSpeed()[2];
      kitty_state.rear_right_rotation_speed = GetSpeed()[0];
      kitty_state.rear_left_rotation_speed = GetSpeed()[3];

      kitty_state.front_right_steering_angle = GetMotPos(1);
      kitty_state.front_left_steering_angle = GetMotPos(2);
      kitty_state.rear_right_steering_angle = GetMotPos(0);
      kitty_state.rear_left_steering_angle = GetMotPos(3);



      kitty_state_pub.publish(&kitty_state);
      lasttime = avr_time_now();
    }
    nh.spinOnce();
  }

  return 0;
}

void InitAll(void)
{
  DDRB |= (1 << 7);
  AdcInit();
  StMotInit();
  DcMotInit();
  // SetAngle(1);
  // SetSpeed(0);
  _delay_ms(100);
  sei();
}


// #include "include/utils.h"
// #include "include/st_mot.h"
// #include "include/dc_motor.h"
// #include "include/uart.h"

// void InitAll(void);
// void GoRobot(int16_t speed);
// void Cmd(uint16_t speed);
// void ShowMotorAngles();
// void ShowSetAngles(uint8_t angle);
// void ShowSpeeds();
// void ShowFormuls(uint8_t data, uint8_t angle);

// int main(void)
// {
// 	InitAll();
// 	float data = 0;
// 	float angle = 0;
// 	while (1)
// 	{
// 		data = UartReceiveData();
// 		// data = 0;
// 		if ((data >= -90) & (data <= 90))
// 		{
// 			angle = data;
// 			SetAngle(angle);
// 		}
// 		else if (data == 1000)
// 			SetSpeed(0, 0);
// 		else
// 			SetSpeed(data, angle);

// 		// ShowMotorAngles();
// 		// ShowSpeeds();
// 		// ShowFormuls(data, angle);
// 		ShowSetAngles(angle);

// 	}
// }

// void InitAll(void)
// {
// 	UartInit();
// 	DDRB |= (1 << 7);
// 	AdcInit();
// 	StMotInit();
// 	DcMotInit();
// 	// SetAngle(1);
// 	// SetSpeed(0);
// 	_delay_ms(100);
// 	sei();
// }
// void GoRobot(int16_t speed)
// {
// 	SetSpeed(speed, 0);
// 	_delay_ms(1000);
// 	SetSpeed(0, 0);
// 	_delay_ms(500);
// }

// void ShowMotorAngles()
// {
// 	UartTransmitByte('\t');
// 	for (int i = 0; i < 4; i++)
// 	{
// 		UartSendDec(GetMotPos(i));
// 		UartTransmitByte('\t');
// 	}
// 	UartTransmitByte('\r');
// }

// void ShowSetAngles(uint8_t angle)
// {
// 	UartTransmitByte('\t');
// 	for (int i = 0; i < 3; i++)
// 	{
// 		UartSendDec(GetAngles(angle)[i]*100);
// 		UartTransmitByte('\t');
// 	}
// 	UartTransmitByte('\r');
// }

// void ShowSpeeds()
// {

// 	UartTransmitByte('\t');
// 	for (int i = 0; i < 4; i++)
// 	{
// 		UartSendDec(GetSpeed()[i]);
// 		UartTransmitByte('\t');
// 	}
// 	UartTransmitByte('\r');
// }

// void ShowFormuls(uint8_t data, uint8_t angle)
// {
// 	UartTransmitByte('\t');
// 	for (int i = 0; i < 6; i++)
// 	{
// 		UartSendDec(((int16_t)(GetFormuls(data, angle)[i])*10));
// 		UartTransmitByte('\t');
// 	}
// 	UartTransmitByte('\r');
// }

// void Cmd(uint16_t speed)
// {
// 	uint16_t robot_speed = speed;

// 	_delay_ms(1000);
// 	SetAngle(1);
// 	_delay_ms(1000);
// 	GoRobot(robot_speed);
// 	_delay_ms(1000);
// 	SetAngle(90);
// 	_delay_ms(1000);
// 	GoRobot(robot_speed);
// 	_delay_ms(1000);
// 	SetAngle(0);
// 	_delay_ms(1000);
// 	GoRobot(-robot_speed);
// 	_delay_ms(1000);

// 	SetAngle(-90);
// 	_delay_ms(1000);
// 	GoRobot(robot_speed);
// 	_delay_ms(1000);
// }
