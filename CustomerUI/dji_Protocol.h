#ifndef __M_DJI_PROTOCOL_H
#define __M_DJI_PROTOCOL_H
/**����ϵͳͨ��Э��**/
#include "mytype.h"
#include "robodata.h"
/********************  ��������  ***************************/
//0-3 bits
#define Strike_0   0x00 //0��װ���棨ǰ��
#define Strike_1   0x01 //1��װ���棨��
#define Strike_2   0x02 //2��װ���棨��
#define Strike_3   0x03 //3��װ���棨�ң�
#define Strike_4   0x04 //4��װ���棨��1��
#define Strike_5   0x05 //5��װ���棨��2��
//4-7 bits
#define BC_Strike          0x00 //װ���˺����ܵ������� BloodChanged    BC
#define BC_ModuleOffline   0x01 //ģ�����߿�Ѫ
#define BC_ShootSpeed      0x02 //�ӵ����ٿ�Ѫ
#define BC_ShootFreq       0x03 //�ӵ���Ƶ��Ѫ
#define BC_ShootQLimit     0x04 //ǹ����������
#define BC_CmPLimit        0x05 //���̹��ʳ���

#define BC_CommonFoul      0x06 //��ͨ�����Ѫ
#define BC_Tarmac          0x0a //ͣ��ƺ��Ѫ
#define BC_EngineerOuto    0x0b //���̻������Զ���Ѫ



//#ifdef INFANTRY3

//	#ifdef RED
//	
//	#else


typedef	struct
{
	  u8 b1;
    u8 b2;
		u8 b3;
		u8 b4;
		u8 b5;
		u8 b6;
}DataBit_t;

typedef	struct
{
	uint8_t val;
	DataBit_t databit;
}CustomDataBit_t;



typedef	struct
{
	CustomDataBit_t databit;
	float fdata[3];
}CustomData_t;

//����������״̬��0x0001��
typedef __packed struct
{
  uint8_t game_type : 4;
  uint8_t game_progress : 4;
  uint16_t stage_remain_time;
} ext_game_state_t;

typedef __packed struct
{
uint8_t operate_tpye;
uint8_t graphic_tpye;
uint8_t graphic_name[5];
uint8_t layer;
uint8_t color;
uint8_t width;
uint16_t start_x;
uint16_t start_y;
uint16_t radius;
uint16_t end_x;
uint16_t end_y;
int16_t start_angle;
int16_t end_angle;
uint8_t text_lengh;
uint8_t text[30];
} ext_client_graphic_draw_t;

//�˺����ݣ�0x0002��
typedef __packed struct
{
  uint8_t winner;
} ext_game_result_t;

//ʵʱ�����Ϣ��0x0003��
typedef __packed struct
{
  uint16_t robot_legion;
} ext_game_robot_survivors_t;

//ʵʱ�����������ݣ�0x004��
typedef __packed struct
{
  uint32_t event_type;
} ext_event_data_t;



//���ؽ������ݣ�0x0005��
typedef __packed struct
{
  uint8_t supply_projectile_id; 
  uint8_t supply_robot_id; 
  uint8_t supply_projectile_step; 
uint8_t supply_projectile_num;
} ext_supply_projectile_action_t;

//����ʤ�����ݣ�0x006��
typedef __packed struct
{
  uint8_t supply_projectile_id;
  uint8_t supply_robot_id; 
uint8_t supply_num;
} ext_supply_projectile_booking_t;

//Buff��ȡ���ݣ�0x0007��
typedef __packed struct
{
  uint8_t robot_id;
  uint8_t robot_level;
  uint16_t remain_HP;
  uint16_t max_HP;
  uint16_t shooter_heat0_cooling_rate;
  uint16_t shooter_heat0_cooling_limit;
  uint16_t shooter_heat1_cooling_rate;
  uint16_t shooter_heat1_cooling_limit;
  uint8_t mains_power_gimbal_output : 1;
  uint8_t mains_power_chassis_output : 1;
  uint8_t mains_power_shooter_output : 1;
} ext_game_robot_state_t;

typedef __packed struct
{
  uint16_t chassis_volt; 
  uint16_t chassis_current; 
  float chassis_power; 
  uint16_t chassis_power_buffer; 
  uint16_t shooter_heat0; 
  uint16_t shooter_heat1; 
} ext_power_heat_data_t;

//�������Զ�������
typedef __packed struct
{
	float data1;
	float data2;
	float data3;
	uint8_t mask;
}extShowData_t;
typedef __packed union 
{
	unsigned char datatransmit[4];
	float data;
}TransmitData_t;
typedef __packed struct
{
  uint16_t data_cmd_id;
  uint16_t send_ID;
  uint16_t receiver_ID;
}ext_student_interactive_header_data_t;

typedef __packed struct
{
  float data1;
  float data2;
  float data3;
  uint8_t masks;
} client_custom_data_t;
typedef __packed struct
{
  uint8_t data[113];
} robot_interactive_data_t;
//������λ�ó�����Ϣ
typedef __packed struct
{
 float x;
  float y;
  float z;
  float yaw;
} ext_game_robot_pos_t;

typedef __packed struct
{
  uint8_t power_rune_buff;
}ext_buff_musk_t;

typedef __packed struct
{
  uint8_t armor_id : 4;
  uint8_t hurt_type : 4;
} ext_robot_hurt_t;

typedef __packed struct
{
  uint8_t energy_point;
  uint8_t attack_time;
} aerial_robot_energy_t;

typedef __packed struct
{
  uint8_t bullet_type;
  uint8_t bullet_freq;
  float bullet_speed;
} ext_shoot_data_t;

typedef __packed struct
{
	uint8_t SOF;          //������ʼ�ֽڣ��̶�Ϊ0xA5          
	uint16_t DataLength;  //���ݳ���
	uint8_t Seq;          //�����
	uint8_t CRC8;         //֡ͷCRCУ��
}FrameHeader_t;//֡ͷ

typedef enum //����ǰ�˳������
{
	ext_game_state = 0x0001,     //����״̬����
	ext_game_result,             //�����������
	ext_game_robot_survivors,    //���������˴������       
	ext_event_data=0x101,        //�����¼�����
	ext_supply_projectile_action,//���ز���վ������ʶ���ݣ�
	ext_supply_projectile_booking,//���󲹸�վ��������
	ext_game_robot_state = 0x0201,//������״̬����
	ext_power_heat_data = 0x0202, //ʵʱ�����������ݣ�
    extGameRobotPos = 0x0203,     //������λ������
    ext_buff_musk,            //��������������
	aerial_robot_energy,     //���л���������״̬����
	ext_robot_hurt,          //�˺�״̬����
	ext_shoot_data,        //ʵʱ�������
	extInteractiveData=0x0301   //�����˼佻������
}CmdID_t;
typedef __packed union
	{
	float data;
	unsigned char divideddata[4];
	}floatdata_t;
typedef __packed struct
{
	FrameHeader_t    FrameHeader;
	CmdID_t          CmdID;
	__packed union 
	{
        ext_game_state_t   		    ext_game_state;
		ext_game_result_t			ext_game_result;
		ext_game_robot_survivors_t  ext_game_robot_survivors;
		ext_event_data_t            ext_event_data;
		ext_supply_projectile_action_t    ext_supply_projectile_action;
		ext_supply_projectile_booking_t   ext_supply_projectile_booking;
		ext_game_robot_state_t            ext_game_robot_state;
		ext_power_heat_data_t             ext_power_heat_data;		
		ext_game_robot_pos_t	  extGameRobotPos;
		ext_buff_musk_t           ext_buff_musk;
		aerial_robot_energy_t     aerial_robot_energy;
		ext_robot_hurt_t          ext_robot_hurt;
		ext_shoot_data_t          ext_shoot_data;
		ext_student_interactive_header_data_t  extInteractiveData;
		client_custom_data_t      client_custom_data;
		//TransmitData_t  TransmitData;	
	}Data;
	uint16_t        CRC16;         //֮ǰ��������CRCУ��   ע������ݺ�֮ǰ�����ݿ��ܲ����������Բ�Ҫֱ��ʹ�ã�����Ҫֱ��ʹ�ã������ڴ˸�ֵ
	
}Frame_t;  //����֡

typedef __packed struct
{
		FrameHeader_t    FrameHeader;
		CmdID_t          CmdID;
		ext_game_state_t   		    ext_game_state;
		ext_game_result_t			ext_game_result;
		ext_game_robot_survivors_t  ext_game_robot_survivors;
		ext_event_data_t            ext_event_data;
		ext_supply_projectile_action_t    ext_supply_projectile_action;
		ext_supply_projectile_booking_t   ext_supply_projectile_booking;
		ext_game_robot_state_t            ext_game_robot_state;
		ext_power_heat_data_t             ext_power_heat_data;		
		ext_game_robot_pos_t	  extGameRobotPos;
		ext_buff_musk_t           ext_buff_musk;
		aerial_robot_energy_t     aerial_robot_energy;
		ext_robot_hurt_t          ext_robot_hurt;
		ext_shoot_data_t          ext_shoot_data;
		ext_student_interactive_header_data_t  extInteractiveData;
        client_custom_data_t      client_custom_data;	
}DJI_Data_t; //����֡

extern DJI_Data_t DJI_ReadData;
extern unsigned char Get_CRC8_Check_Sum(unsigned char *pchMessage,unsigned int dwLength,unsigned char ucCRC8);
extern uint16_t Get_CRC16_Check_Sum(uint8_t *pchMessage,uint32_t dwLength,uint16_t wCRC);
extern short dji_DataProcess(uint8_t *pData, DJI_Data_t *DJI_ReadData, RoboData_t *RoboData);
void GetDataBit(CustomData_t *customdata);
void CustomTransmit(CustomData_t CustomData);
extern void  floattest(uint8_t *pData);
void CustomSend(void);

void CustomShapeTransmit(ext_client_graphic_draw_t *shape);

void CustomShapeTransmit_10000_aim(void);
void CustomShapeTransmit_20000_aimup(void);
void CustomShapeTransmit_30000_aimdown(void);
void CustomShapeTransmit_40000_aimleft(void);
void CustomShapeTransmit_50000_aimright(void);
void CustomShapeTransmit_60000_level1(void);

void CustomShapeClearAll(void);
#endif
