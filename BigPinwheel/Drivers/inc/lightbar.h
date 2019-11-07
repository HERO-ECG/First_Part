#ifndef __LIGHTBAR_H__
#define __LIGHTBAR_H__

#include "stm32f10x.h"
#include "sk6812.h"
#include "common.h"



extern void test(void);
void LightBar_init(void);
void BigPinWheel_showlight(LightStatus_e status);
void BigPinWheel_show_runarrow(void);

#endif
