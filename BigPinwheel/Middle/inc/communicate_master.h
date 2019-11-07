#ifndef __COMMUNICATE_MASTER_H__
#define __COMMUNICATE_MASTER_H__


#include "stm32f10x.h"
#include "LightBar_logic.h"
#include "common.h"

void master_control_init(void);
void master_setstatus(uint8_t slave_id,LightStatus_e state);
void master_reset(void);
void master_setactivation(void);



#endif
