#include "ros.h"
#include "std_msgs/Int32.h"
// Include C headers (ie, non C++ headers) in this block
extern "C"
{
  #include <util/delay.h>
}

// Needed for AVR to use virtual functions
extern "C" void __cxa_pure_virtual(void);
void __cxa_pure_virtual(void) {}

ros::NodeHandle nh;
uint16_t my_delay = 500;

void messageCb( const std_msgs::Int32& delay_msg){
  my_delay=delay_msg.data;   // blink the led
}

ros::Subscriber<std_msgs::Int32> sub("led_delay", &messageCb);

int main()
{
  uint32_t lasttime = 0UL;
  // Initialize ROS
  nh.initNode();
  nh.subscribe(sub);

  DDRB = (1 << PB7);

  while(1)
  {
    // Send the message every second
    if(avr_time_now() - lasttime > my_delay)
    {
      PORTB ^= (1 << PB7);
      lasttime = avr_time_now();
     
    }
    nh.spinOnce();
  }

  return 0;
}
