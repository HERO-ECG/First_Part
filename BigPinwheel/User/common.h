#ifndef __COMMON_H__
#define __COMMON_H__
#include <stdint.h>
/*���ӻ��л�*/
#define	 SLAVE//   MASTER

#define MASTER_ID 0x31F					//����ID
#define SLAVE_ID 	0x302			//�ӻ�ID��301-305��
#define RESET_ID  0x33F					//��λID
#define FINISH_ID 0x37F					//����ź�

#define MOTO_3508_ID		0x204



typedef enum
{
	/*����*/
	LED_UNABLE,
	/*������*/
	LED_ABLEING,
	/*�Ѽ���*/
	LED_ENABLE,
	/*ȫ������ɹ�*/
	LED_FINISH
}LightStatus_e;

extern LightStatus_e g_slave_status;

#endif





