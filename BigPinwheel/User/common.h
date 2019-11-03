#ifndef __COMMON_H__
#define __COMMON_H__
#include <stdint.h>
/*主从机切换*/
#define	 SLAVE//   MASTER

#define MASTER_ID 0x31F					//主机ID
#define SLAVE_ID 	0x302			//从机ID（301-305）
#define RESET_ID  0x33F					//复位ID
#define FINISH_ID 0x37F					//完成信号

#define MOTO_3508_ID		0x204



typedef enum
{
	/*灯灭*/
	LED_UNABLE,
	/*激活中*/
	LED_ABLEING,
	/*已激活*/
	LED_ENABLE,
	/*全部激活成功*/
	LED_FINISH
}LightStatus_e;

extern LightStatus_e g_slave_status;

#endif





