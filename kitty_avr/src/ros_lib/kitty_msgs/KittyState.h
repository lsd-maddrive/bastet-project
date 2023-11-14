#ifndef _ROS_kitty_msgs_KittyState_h
#define _ROS_kitty_msgs_KittyState_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "ros/time.h"

namespace kitty_msgs
{

  class KittyState : public ros::Msg
  {
    public:
      typedef ros::Time _stamp_type;
      _stamp_type stamp;
      typedef float _rotation_speed_type;
      _rotation_speed_type rotation_speed;
      typedef float _angle_steering_type;
      _angle_steering_type angle_steering;
      typedef float _front_right_steering_angle_type;
      _front_right_steering_angle_type front_right_steering_angle;
      typedef float _front_left_steering_angle_type;
      _front_left_steering_angle_type front_left_steering_angle;
      typedef float _rear_right_steering_angle_type;
      _rear_right_steering_angle_type rear_right_steering_angle;
      typedef float _rear_left_steering_angle_type;
      _rear_left_steering_angle_type rear_left_steering_angle;
      typedef float _front_right_rotation_speed_type;
      _front_right_rotation_speed_type front_right_rotation_speed;
      typedef float _front_left_rotation_speed_type;
      _front_left_rotation_speed_type front_left_rotation_speed;
      typedef float _rear_right_rotation_speed_type;
      _rear_right_rotation_speed_type rear_right_rotation_speed;
      typedef float _rear_left_rotation_speed_type;
      _rear_left_rotation_speed_type rear_left_rotation_speed;

    KittyState():
      stamp(),
      rotation_speed(0),
      angle_steering(0),
      front_right_steering_angle(0),
      front_left_steering_angle(0),
      rear_right_steering_angle(0),
      rear_left_steering_angle(0),
      front_right_rotation_speed(0),
      front_left_rotation_speed(0),
      rear_right_rotation_speed(0),
      rear_left_rotation_speed(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->stamp.sec >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->stamp.sec >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->stamp.sec >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->stamp.sec >> (8 * 3)) & 0xFF;
      offset += sizeof(this->stamp.sec);
      *(outbuffer + offset + 0) = (this->stamp.nsec >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->stamp.nsec >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->stamp.nsec >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->stamp.nsec >> (8 * 3)) & 0xFF;
      offset += sizeof(this->stamp.nsec);
      union {
        float real;
        uint32_t base;
      } u_rotation_speed;
      u_rotation_speed.real = this->rotation_speed;
      *(outbuffer + offset + 0) = (u_rotation_speed.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_rotation_speed.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_rotation_speed.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_rotation_speed.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->rotation_speed);
      union {
        float real;
        uint32_t base;
      } u_angle_steering;
      u_angle_steering.real = this->angle_steering;
      *(outbuffer + offset + 0) = (u_angle_steering.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_angle_steering.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_angle_steering.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_angle_steering.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->angle_steering);
      union {
        float real;
        uint32_t base;
      } u_front_right_steering_angle;
      u_front_right_steering_angle.real = this->front_right_steering_angle;
      *(outbuffer + offset + 0) = (u_front_right_steering_angle.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_front_right_steering_angle.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_front_right_steering_angle.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_front_right_steering_angle.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->front_right_steering_angle);
      union {
        float real;
        uint32_t base;
      } u_front_left_steering_angle;
      u_front_left_steering_angle.real = this->front_left_steering_angle;
      *(outbuffer + offset + 0) = (u_front_left_steering_angle.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_front_left_steering_angle.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_front_left_steering_angle.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_front_left_steering_angle.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->front_left_steering_angle);
      union {
        float real;
        uint32_t base;
      } u_rear_right_steering_angle;
      u_rear_right_steering_angle.real = this->rear_right_steering_angle;
      *(outbuffer + offset + 0) = (u_rear_right_steering_angle.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_rear_right_steering_angle.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_rear_right_steering_angle.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_rear_right_steering_angle.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->rear_right_steering_angle);
      union {
        float real;
        uint32_t base;
      } u_rear_left_steering_angle;
      u_rear_left_steering_angle.real = this->rear_left_steering_angle;
      *(outbuffer + offset + 0) = (u_rear_left_steering_angle.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_rear_left_steering_angle.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_rear_left_steering_angle.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_rear_left_steering_angle.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->rear_left_steering_angle);
      union {
        float real;
        uint32_t base;
      } u_front_right_rotation_speed;
      u_front_right_rotation_speed.real = this->front_right_rotation_speed;
      *(outbuffer + offset + 0) = (u_front_right_rotation_speed.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_front_right_rotation_speed.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_front_right_rotation_speed.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_front_right_rotation_speed.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->front_right_rotation_speed);
      union {
        float real;
        uint32_t base;
      } u_front_left_rotation_speed;
      u_front_left_rotation_speed.real = this->front_left_rotation_speed;
      *(outbuffer + offset + 0) = (u_front_left_rotation_speed.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_front_left_rotation_speed.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_front_left_rotation_speed.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_front_left_rotation_speed.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->front_left_rotation_speed);
      union {
        float real;
        uint32_t base;
      } u_rear_right_rotation_speed;
      u_rear_right_rotation_speed.real = this->rear_right_rotation_speed;
      *(outbuffer + offset + 0) = (u_rear_right_rotation_speed.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_rear_right_rotation_speed.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_rear_right_rotation_speed.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_rear_right_rotation_speed.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->rear_right_rotation_speed);
      union {
        float real;
        uint32_t base;
      } u_rear_left_rotation_speed;
      u_rear_left_rotation_speed.real = this->rear_left_rotation_speed;
      *(outbuffer + offset + 0) = (u_rear_left_rotation_speed.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_rear_left_rotation_speed.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_rear_left_rotation_speed.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_rear_left_rotation_speed.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->rear_left_rotation_speed);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      this->stamp.sec =  ((uint32_t) (*(inbuffer + offset)));
      this->stamp.sec |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->stamp.sec |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->stamp.sec |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->stamp.sec);
      this->stamp.nsec =  ((uint32_t) (*(inbuffer + offset)));
      this->stamp.nsec |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->stamp.nsec |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->stamp.nsec |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->stamp.nsec);
      union {
        float real;
        uint32_t base;
      } u_rotation_speed;
      u_rotation_speed.base = 0;
      u_rotation_speed.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_rotation_speed.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_rotation_speed.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_rotation_speed.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->rotation_speed = u_rotation_speed.real;
      offset += sizeof(this->rotation_speed);
      union {
        float real;
        uint32_t base;
      } u_angle_steering;
      u_angle_steering.base = 0;
      u_angle_steering.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_angle_steering.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_angle_steering.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_angle_steering.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->angle_steering = u_angle_steering.real;
      offset += sizeof(this->angle_steering);
      union {
        float real;
        uint32_t base;
      } u_front_right_steering_angle;
      u_front_right_steering_angle.base = 0;
      u_front_right_steering_angle.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_front_right_steering_angle.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_front_right_steering_angle.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_front_right_steering_angle.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->front_right_steering_angle = u_front_right_steering_angle.real;
      offset += sizeof(this->front_right_steering_angle);
      union {
        float real;
        uint32_t base;
      } u_front_left_steering_angle;
      u_front_left_steering_angle.base = 0;
      u_front_left_steering_angle.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_front_left_steering_angle.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_front_left_steering_angle.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_front_left_steering_angle.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->front_left_steering_angle = u_front_left_steering_angle.real;
      offset += sizeof(this->front_left_steering_angle);
      union {
        float real;
        uint32_t base;
      } u_rear_right_steering_angle;
      u_rear_right_steering_angle.base = 0;
      u_rear_right_steering_angle.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_rear_right_steering_angle.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_rear_right_steering_angle.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_rear_right_steering_angle.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->rear_right_steering_angle = u_rear_right_steering_angle.real;
      offset += sizeof(this->rear_right_steering_angle);
      union {
        float real;
        uint32_t base;
      } u_rear_left_steering_angle;
      u_rear_left_steering_angle.base = 0;
      u_rear_left_steering_angle.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_rear_left_steering_angle.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_rear_left_steering_angle.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_rear_left_steering_angle.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->rear_left_steering_angle = u_rear_left_steering_angle.real;
      offset += sizeof(this->rear_left_steering_angle);
      union {
        float real;
        uint32_t base;
      } u_front_right_rotation_speed;
      u_front_right_rotation_speed.base = 0;
      u_front_right_rotation_speed.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_front_right_rotation_speed.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_front_right_rotation_speed.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_front_right_rotation_speed.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->front_right_rotation_speed = u_front_right_rotation_speed.real;
      offset += sizeof(this->front_right_rotation_speed);
      union {
        float real;
        uint32_t base;
      } u_front_left_rotation_speed;
      u_front_left_rotation_speed.base = 0;
      u_front_left_rotation_speed.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_front_left_rotation_speed.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_front_left_rotation_speed.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_front_left_rotation_speed.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->front_left_rotation_speed = u_front_left_rotation_speed.real;
      offset += sizeof(this->front_left_rotation_speed);
      union {
        float real;
        uint32_t base;
      } u_rear_right_rotation_speed;
      u_rear_right_rotation_speed.base = 0;
      u_rear_right_rotation_speed.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_rear_right_rotation_speed.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_rear_right_rotation_speed.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_rear_right_rotation_speed.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->rear_right_rotation_speed = u_rear_right_rotation_speed.real;
      offset += sizeof(this->rear_right_rotation_speed);
      union {
        float real;
        uint32_t base;
      } u_rear_left_rotation_speed;
      u_rear_left_rotation_speed.base = 0;
      u_rear_left_rotation_speed.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_rear_left_rotation_speed.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_rear_left_rotation_speed.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_rear_left_rotation_speed.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->rear_left_rotation_speed = u_rear_left_rotation_speed.real;
      offset += sizeof(this->rear_left_rotation_speed);
     return offset;
    }

    virtual const char * getType() override { return "kitty_msgs/KittyState"; };
    virtual const char * getMD5() override { return "2a8ad769553b99f342ff03e0bec339c5"; };

  };

}
#endif
