#include "ros.h"
#include "std_msgs/Float32MultiArray.h"
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

void messageCb(const std_msgs::Float32MultiArray &speed_msg)
{
  SetSpeed(speed_msg.data[0]);
  SetAngle(speed_msg.data[1]);
}

ros::Subscriber<std_msgs::Float32MultiArray> sub("set_speed", &messageCb);

int main()
{
  uint32_t lasttime = 0UL;
  // Initialize ROS
  nh.initNode();
  nh.subscribe(sub);
  InitAll();

  while (1)
  {
    // Send the message every second
    // if(avr_time_now() - lasttime > my_delay)
    // {
    //   PORTB ^= (1 << PB7);
    //   lasttime = avr_time_now();

    // }
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

// #include "st_mot.h"
// #include "dc_motor.h"
// #include "utils.h"
// #include "uart.h"

// void InitAll(void);
// void GoRobot(int16_t speed);
// void Cmd(uint16_t speed);
// void ShowAngles();
// void ShowSpeeds();

// int main(void)
// {
//   InitAll();
//   float data = 0;
//   while (1)
//   {
//     // Cmd(300);
//     // data = UartReceiveData();
//     // if ((data >=-90) & (data <=90)) SetAngle(data);
//     // else if (data == 1000) SetSpeed(0);
//     // else SetSpeed(data);
//     ShowAngles();
//     // ShowSpeeds();
//   }
// }

// void InitAll(void)
// {
//   DDRB |= (1 << 7);
//   // AdcInit();
//   // StMotInit();
//   DcMotInit();
//   // SetAngle(1);
//   // SetSpeed(0);
//   _delay_ms(100);
//   sei();
// }

// void ShowAngles()
// {
//   UartTransmitByte('\t');
//   for (int i = 0; i < 4; i++)
//   {
//     UartSendDec(GetMotPos(i));
//     UartTransmitByte('\t');
//   }
//   UartTransmitByte('\r');
// }
// void ShowSpeeds()
// {

//   UartTransmitByte('\t');
//   for (int i = 0; i < 4; i++)
//   {
//     UartSendDec(GetSpeed()[i]);
//     UartTransmitByte('\t');
//   }
//   UartTransmitByte('\r');
// }
