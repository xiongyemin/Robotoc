#include "pwm.h"
#include "pid.h"
void all_pid_init(void){
	for(int i=0; i<4; i++){
		pid_init(&motor_pid[i]);
		motor_pid[i].f_param_init(&motor_pid[i],PID_Speed,16384,5000,10,0,8000,0,1.5,0.1,0);
  }
}
