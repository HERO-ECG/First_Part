#ifndef __MOTO_6020_H__
#define __MOTO_6020_H__

#include "stm32f10x.h"
/*6020电机结构体*/
typedef struct
{
	int16_t angle;
	int16_t speed;
	int16_t current;
	uint16_t temperature;
}Moto6020_par_t;

typedef struct
{
	int16_t Setspeed;
	int16_t Setcurrent;
	
	int16_t Outspeed_limit;
	int16_t Outcurrent_limit;
	float kp_speed;float ki_speed;float kd_speed;
	float kp_current;float ki_current;float kd_current;
}Pid_t;




#endif
