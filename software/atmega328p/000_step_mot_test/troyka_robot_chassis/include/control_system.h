#include "utils.h"

struct {
	float kp=0;
	float kd;
	float ki;
	float setpoint;
	float int_sum;
	float prev_err;
	float int_limit;
	uint8_t flag;
	}pid; 
	
typedef struct pid pid_t;

struct {
	pid_t motor1;
	pid_t motor2;
	pid_t motor3;
	pid_t motor4;
	
}robot;
	
void update(pid_t motor);