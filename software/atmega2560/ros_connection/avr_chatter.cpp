#include "ros.h"
#include "std_msgs/Int32.h"
#include "std_msgs/String.h"
// Include C headers (ie, non C++ headers) in this block
extern "C"
{
  #include <util/delay.h>
}

// Needed for AVR to use virtual functions
extern "C" void __cxa_pure_virtual(void);
void __cxa_pure_virtual(void) {}

ros::NodeHandle nh;

std_msgs::String str_msg;
ros::Publisher chatter1("hello_chatter", &str_msg);

std_msgs::Int32 int_msg;
ros::Publisher chatter2("int_chatter", &int_msg);

char hello[40] = "hello Katya! We did it!!! ";



int main()
{
  uint32_t lasttime = 0UL;
  // Initialize ROS
  nh.initNode();
  nh.advertise(chatter1);
  nh.advertise(chatter2);

  DDRB = (1 << PB7);


  while(1)
  {
    // Send the message every second
    if(avr_time_now() - lasttime > 500)
    {
      PORTB ^= (1 << PB7);
      str_msg.data = hello;
      int_msg.data = lasttime;

      chatter1.publish(&str_msg);
      chatter2.publish(&int_msg);

      lasttime = avr_time_now();
      
    }
    nh.spinOnce();
  }

  return 0;
}
