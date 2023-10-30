#include "control_system.h"

void update(pid_t* motor, float current_speed){
	float error = motor.setpoint - current_speed;
	motor.int_sum += error;
	float voltage = error*motor.kp + (error - motor.prev_err) * dt * motor.kd + motor.int_sum * motor.ki;
}
