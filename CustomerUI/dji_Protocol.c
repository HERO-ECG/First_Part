#include "dji_Protocol.h"
#include "robodata.h"
#include <math.h>
#include "usart.h"
#include "chassis.h"
#include "m_transmit.h"
#include "t_monitor.h"

//自定义UI,坐标
//................................(1920,1080)
//...........................................
//...........................................
//...........................................
//................(960,540)..................
//...........................................
//...........................................
//...........................................
//(0,0)......................................
#define aim_x  960
#define aim_y  475

uint16_t beat;
int test;

extern uint16_t allow17num;
DJI_Data_t DJI_ReadData;//定义帧数据指针用来保存数据信息
//crc8 generator polynomial:G(x)=x8+x5+x4+1
//..........................................operate/type   /name/   layer/color/width/ start_x/  start_y/radius/  end_x/  end_y/start_angle/end_angle/text_lengh/text[30]/
ext_client_graphic_draw_t aimleft_1    	=  {   1,    1, {1 ,0,0,0,0},  1,   6,    2,    aim_x-35,aim_y   ,  0,  aim_x-15,aim_y,       0,         0,        0,        {0}};
ext_client_graphic_draw_t aimleft_2    	=  {   1,    1, {2 ,0,0,0,0},  1,   6,    1,    aim_x-24,aim_y-15,  0,  aim_x-15,aim_y-15,    0,         0,        0,        {0}};
ext_client_graphic_draw_t aimleft_3   	=  {   1,    1, {3 ,0,0,0,0},  1,   6,    1,    aim_x-24,aim_y-30,  0,  aim_x-15,aim_y-30,    0,         0,        0,        {0}};
ext_client_graphic_draw_t aimleft_4  		=  {   1,    1, {4 ,0,0,0,0},  1,   6,    1,    aim_x-24,aim_y-45,  0,  aim_x-15,aim_y-45,    0,         0,        0,        {0}};
ext_client_graphic_draw_t aimleft_5  		=  {   1,    1, {5 ,0,0,0,0},  1,   6,    1,    aim_x-24,aim_y-60,  0,  aim_x-15,aim_y-60,    0,         0,        0,        {0}};
ext_client_graphic_draw_t aimright_1    =  {   1,    1, {6 ,0,0,0,0},  1,   6,    2,    aim_x+15,aim_y   ,  0,  aim_x+35,aim_y,       0,         0,        0,        {0}};
ext_client_graphic_draw_t aimright_2    =  {   1,    1, {7 ,0,0,0,0},  1,   6,    1,    aim_x+15,aim_y-15,  0,  aim_x+24,aim_y-15,    0,         0,        0,        {0}};
ext_client_graphic_draw_t aimright_3   	=  {   1,    1, {8 ,0,0,0,0},  1,   6,    1,    aim_x+15,aim_y-30,  0,  aim_x+24,aim_y-30,    0,         0,        0,        {0}};
ext_client_graphic_draw_t aimright_4  	=  {   1,    1, {9 ,0,0,0,0},  1,   6,    1,    aim_x+15,aim_y-45,  0,  aim_x+24,aim_y-45,    0,         0,        0,        {0}};
ext_client_graphic_draw_t aimright_5  	=  {   1,    1, {10,0,0,0,0},  1,   6,    1,    aim_x+15,aim_y-60,  0,  aim_x+24,aim_y-60,    0,         0,        0,        {0}};
ext_client_graphic_draw_t aimshu_1      =  {   1,    1, {11,0,0,0,0},  1,   6,    1,    aim_x-15,aim_y   ,  0,  aim_x-15,aim_y-60,    0,         0,        0,        {0}};
ext_client_graphic_draw_t aimshu_2      =  {   1,    1, {12,0,0,0,0},  1,   6,    1,    aim_x+15,aim_y   ,  0,  aim_x+15,aim_y-60,    0,         0,        0,        {0}};
ext_client_graphic_draw_t rect_1        =  {   1,    2, {13,0,0,0,0},  2,   0,    3,    aim_x+200,aim_y  ,  0,  aim_x+400,aim_y-100,  0,         0,        0,        {0}};
ext_client_graphic_draw_t circle_1      =  {   1,    3, {14,0,0,0,0},  3,   1,    2,    aim_x+200,aim_y  ,  60,  aim_x+15,aim_y-60,   0,         0,        0,        {0}};
ext_client_graphic_draw_t ellipse_1     =  {   1,    4, {15,0,0,0,0},  4,   2,    4,    aim_x+400,aim_y  ,  0,        80,    60  ,    0,         0,        0,        {0}};
ext_client_graphic_draw_t text_1        =  {   1,    6, {16,0,0,0,0},  5,   3,    3,    aim_x+200,aim_y-120,30,  aim_x+15,aim_y-60,   0,         0,        4,        {"HERO"}};

//ext_client_graphic_draw_t custom_shape;
//ext_client_graphic_draw_t aimdot_10000    =  {   1,    2, {1,0,0,0,0},  1,   6,    2,    aim_x,   aim_y,     0,  aim_x+1,  aim_y+1,    0,         0,        0,        {0}};
//ext_client_graphic_draw_t aimup_20000     =  {   1,    1, {2,0,0,0,0},  1,   6,    1,    aim_x,   aim_y+25,  0,  aim_x,    aim_y+50,   0,         0,        0,        {0}};
//ext_client_graphic_draw_t aimdown_30000   =  {   1,    1, {3,0,0,0,0},  1,   6,    1,    aim_x,   aim_y-50,  0,  aim_x,    aim_y-25,   0,         0,        0,        {0}};
//ext_client_graphic_draw_t aimleft_40000   =  {   1,    1, {4,0,0,0,0},  1,   6,    1,    aim_x-50,aim_y,     0,  aim_x-25, aim_y,      0,         0,        0,        {0}};
//ext_client_graphic_draw_t aimright_50000  =  {   1,    1, {5,0,0,0,0},  1,   6,    1,    aim_x+25,aim_y,     0,  aim_x+50, aim_y,      0,         0,        0,        {0}};
//ext_client_graphic_draw_t aimlevel1_60000 =  {   1,    1, {6,0,0,0,0},  1,   6,    1,    aim_x-25,aim_y-25,  0,  aim_x+25, aim_y-25,   0,         0,        0,        {0}};
//ext_client_graphic_draw_t aimlevel2_70000 =  {   1,    1, {7,0,0,0,0},  1,   6,    1,    aim_x-18,aim_y-35,  0,  aim_x+18, aim_y-35,   0,         0,        0,        {0}};
//ext_client_graphic_draw_t aimlevel3_80000 =  {   1,    1, {8,0,0,0,0},  1,   6,    1,    aim_x-12,aim_y-45,  0,  aim_x+12, aim_y-45,   0,         0,        0,        {0}};
//ext_client_graphic_draw_t aimlevel4_90000 =  {   1,    1, {9,0,0,0,0},  1,   6,    1,    aim_x-6, aim_y-55,  0,  aim_x+6,  aim_y-55,   0,         0,        0,        {0}};

CustomData_t customdata;
const unsigned char CRC8_INIT = 0xff;  // CRC8 校验初始值
uint16_t CRC16_INIT = 0xffff;   // CRC16 校验初始值
Frame_t Frame;
unsigned char FrameAllMessage[28];
unsigned char CustomShapeMessage[70];
const unsigned char CRC8_TAB[256];
const uint16_t wCRC_Table[256];
const unsigned char CRC8_TAB[256] =
{
    0x00, 0x5e, 0xbc, 0xe2, 0x61, 0x3f, 0xdd, 0x83, 0xc2, 0x9c, 0x7e, 0x20, 0xa3, 0xfd, 0x1f, 0x41,
    0x9d, 0xc3, 0x21, 0x7f, 0xfc, 0xa2, 0x40, 0x1e, 0x5f, 0x01, 0xe3, 0xbd, 0x3e, 0x60, 0x82, 0xdc,
    0x23, 0x7d, 0x9f, 0xc1, 0x42, 0x1c, 0xfe, 0xa0, 0xe1, 0xbf, 0x5d, 0x03, 0x80, 0xde, 0x3c, 0x62,
    0xbe, 0xe0, 0x02, 0x5c, 0xdf, 0x81, 0x63, 0x3d, 0x7c, 0x22, 0xc0, 0x9e, 0x1d, 0x43, 0xa1, 0xff,
    0x46, 0x18, 0xfa, 0xa4, 0x27, 0x79, 0x9b, 0xc5, 0x84, 0xda, 0x38, 0x66, 0xe5, 0xbb, 0x59, 0x07,
    0xdb, 0x85, 0x67, 0x39, 0xba, 0xe4, 0x06, 0x58, 0x19, 0x47, 0xa5, 0xfb, 0x78, 0x26, 0xc4, 0x9a,
    0x65, 0x3b, 0xd9, 0x87, 0x04, 0x5a, 0xb8, 0xe6, 0xa7, 0xf9, 0x1b, 0x45, 0xc6, 0x98, 0x7a, 0x24,
    0xf8, 0xa6, 0x44, 0x1a, 0x99, 0xc7, 0x25, 0x7b, 0x3a, 0x64, 0x86, 0xd8, 0x5b, 0x05, 0xe7, 0xb9,
    0x8c, 0xd2, 0x30, 0x6e, 0xed, 0xb3, 0x51, 0x0f, 0x4e, 0x10, 0xf2, 0xac, 0x2f, 0x71, 0x93, 0xcd,
    0x11, 0x4f, 0xad, 0xf3, 0x70, 0x2e, 0xcc, 0x92, 0xd3, 0x8d, 0x6f, 0x31, 0xb2, 0xec, 0x0e, 0x50,
    0xaf, 0xf1, 0x13, 0x4d, 0xce, 0x90, 0x72, 0x2c, 0x6d, 0x33, 0xd1, 0x8f, 0x0c, 0x52, 0xb0, 0xee,
    0x32, 0x6c, 0x8e, 0xd0, 0x53, 0x0d, 0xef, 0xb1, 0xf0, 0xae, 0x4c, 0x12, 0x91, 0xcf, 0x2d, 0x73,
    0xca, 0x94, 0x76, 0x28, 0xab, 0xf5, 0x17, 0x49, 0x08, 0x56, 0xb4, 0xea, 0x69, 0x37, 0xd5, 0x8b,
    0x57, 0x09, 0xeb, 0xb5, 0x36, 0x68, 0x8a, 0xd4, 0x95, 0xcb, 0x29, 0x77, 0xf4, 0xaa, 0x48, 0x16,
    0xe9, 0xb7, 0x55, 0x0b, 0x88, 0xd6, 0x34, 0x6a, 0x2b, 0x75, 0x97, 0xc9, 0x4a, 0x14, 0xf6, 0xa8,
    0x74, 0x2a, 0xc8, 0x96, 0x15, 0x4b, 0xa9, 0xf7, 0xb6, 0xe8, 0x0a, 0x54, 0xd7, 0x89, 0x6b, 0x35,
};
const uint16_t wCRC_Table[256] =
{
    0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
    0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
    0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
    0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
    0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
    0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
    0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
    0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
    0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
    0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
    0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
    0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
    0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
    0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
    0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
    0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
    0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
    0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
    0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
    0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
    0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
    0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
    0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
    0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
    0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
    0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
    0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
    0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
    0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
    0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
    0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
    0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};

/*----------------------------------------------------------------------------------------------------------------------------------------------------------*/
unsigned char Get_CRC8_Check_Sum(unsigned char *pchMessage,unsigned int dwLength,unsigned char ucCRC8)
{
    unsigned char ucIndex;
    while(dwLength--)
    {
        ucIndex = ucCRC8^(*pchMessage++);
        ucCRC8 = CRC8_TAB[ucIndex];
    }
    return(ucCRC8);
}

unsigned int Verify_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength)
{
    unsigned char ucExpected = 0;

    if ((pchMessage == 0) || (dwLength <= 2)) return 0;
    ucExpected = Get_CRC8_Check_Sum(pchMessage, dwLength-1, CRC8_INIT);
    return ( ucExpected == pchMessage[dwLength-1] );
}

void Append_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength)
{
    unsigned char ucCRC = 0;

    if ((pchMessage == 0) || (dwLength <= 2)) return;
    ucCRC = Get_CRC8_Check_Sum((unsigned char *)pchMessage, dwLength-1, CRC8_INIT);
    pchMessage[dwLength-1] = ucCRC;
}

uint16_t Get_CRC16_Check_Sum(uint8_t *pchMessage,uint32_t dwLength,uint16_t wCRC)
{
    uint8_t chData;

    if (pchMessage == 0)
    {
        return 0xFFFF;
    }
    while(dwLength--)
    {
        chData = *pchMessage++;
        (wCRC) = ((uint16_t)(wCRC) >> 8) ^ wCRC_Table[((uint16_t)(wCRC) ^ (uint16_t)(chData)) & 0x00ff];
    }
    return wCRC;
}

uint32_t Verify_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength)
{
    uint16_t wExpected = 0;

    if ((pchMessage == 0) || (dwLength <= 2))
    {
        return 0;
    }
    wExpected = Get_CRC16_Check_Sum(pchMessage, dwLength - 2, CRC16_INIT);
    return ((wExpected & 0xff) == pchMessage[dwLength - 2] && ((wExpected >> 8) & 0xff) == pchMessage[dwLength - 1]);
}

void Append_CRC16_Check_Sum(uint8_t * pchMessage,uint32_t dwLength)
{
    uint16_t wCRC = 0;

    if ((pchMessage == 0) || (dwLength <= 2))
    {
        return;
    }
    wCRC = Get_CRC16_Check_Sum ((uint8_t *)pchMessage, dwLength-2, CRC16_INIT);
    pchMessage[dwLength-2] = (uint8_t)(wCRC & 0x00ff);
    pchMessage[dwLength-1] = (uint8_t)((wCRC >> 8)& 0x00ff);
}

uint32_t shoottime;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------*/
short dji_DataProcess(uint8_t *pData, DJI_Data_t *DJI_ReadData, RoboData_t *RoboData)
{
    int i;
    u8	length;
    if(pData==NULL) return 0;
    for(i=0; i<len_uart6_rx_buff; i++)
    {
        if(pData[i] == 0xA5)
        {
            Frame_t* tempData;
            tempData = (Frame_t *)(&pData[i]);
            length = tempData->FrameHeader.DataLength;
            if(Verify_CRC8_Check_Sum(&pData[i],5)&&Verify_CRC16_Check_Sum(&pData[i],length+9))
            {
                DJI_ReadData->FrameHeader = tempData->FrameHeader;
                DJI_ReadData->CmdID = tempData->CmdID;
                switch(tempData->CmdID)
                {
                case 0x0001:
                {
                    DJI_ReadData->ext_game_state = tempData->Data.ext_game_state;


                }
                break;
                case 0x0002:
                {
                    DJI_ReadData->ext_game_result = tempData->Data.ext_game_result;


                }
                break;
                case 0x0003:
                {
                    DJI_ReadData->ext_game_robot_survivors = tempData->Data.ext_game_robot_survivors;


                }
                break;
                case 0x0101:
                {
                    DJI_ReadData->ext_event_data = tempData->Data.ext_event_data;
                    /*本地热量与裁判系统及时热量对齐*/
//						RoboData->shoot_heat_ctr.nowHeat17 = DJI_ReadData->extPowerHeatData.shooterHeat17;
//						RoboData->shoot_heat_ctr.nowHeat42 = DJI_ReadData->extPowerHeatData.shooterHeat42;


                }
                break;
                case 0x0102:
                {
                    DJI_ReadData->ext_supply_projectile_action = tempData->Data.ext_supply_projectile_action;

                }
                break;
                case 0x0103:
                {
                    DJI_ReadData->ext_supply_projectile_booking = tempData->Data.ext_supply_projectile_booking;


                }
                break;
                case 0x0201:
                {
                    DJI_ReadData->ext_game_robot_state = tempData->Data.ext_game_robot_state;


                }
                break;
                case 0x0202:
                {
                    DJI_ReadData->ext_power_heat_data = tempData->Data.ext_power_heat_data;


                }
                break;

                case 0x0203://extGameRobotPos:
                {
                    DJI_ReadData->extGameRobotPos = tempData->Data.extGameRobotPos;

                }
                break;
                case 0x0204:
                {
                    DJI_ReadData->ext_buff_musk = tempData->Data.ext_buff_musk;

                }
                break;
                case 0x0205:
                {
                    DJI_ReadData->aerial_robot_energy = tempData->Data.aerial_robot_energy;

                }
                break;
                case 0x0206:
                {
                    DJI_ReadData->ext_robot_hurt = tempData->Data.ext_robot_hurt;

                }
                break;
                case 0x0207:
                {
                    DJI_ReadData->ext_shoot_data = tempData->Data.ext_shoot_data;
                    shoottime++;
                }
                break;
                case 0x0301:
                {
                    DJI_ReadData->client_custom_data = tempData->Data.client_custom_data;

                }
                break;
                default:
                {
                } break;
                }

            }
        }


    }

    return 0;
}




/*--------------------- */

void GetDataBit(CustomData_t *customdata)
{
    uint8_t databit=0x00;
    if(customdata->databit.databit.b1==1)
        databit|=0x01;
    if(customdata->databit.databit.b2==1)
        databit|=0x02;
    if(customdata->databit.databit.b3==1)
        databit|=0x04;
    if(customdata->databit.databit.b4==1)
        databit|=0x08;
    if(customdata->databit.databit.b5==1)
        databit|=0x10;
    if(customdata->databit.databit.b6==1)
        databit|=0x20;
    customdata->databit.val=databit;
}

void CustomTransmit(CustomData_t CustomData)
{
    float data1,data2,data3;
    u8 mask = 1;
    u16 tempcrc16;
    uint16_t robotID1;
    uint16_t robotID2;

    robotID1=DJI_ReadData.ext_game_robot_state.robot_id;
    robotID2=robotID1|0x0100;

    GetDataBit(&CustomData);

    data1 = CustomData.fdata[0];
    data2 = CustomData.fdata[1];
    data3 = CustomData.fdata[2];
    mask = CustomData.databit.val;
    //	mask = 0x3f;
    FrameAllMessage[0] = 0xA5;
    FrameAllMessage[1] = 19;
    FrameAllMessage[2] = 19>>8;
    FrameAllMessage[3] = 1;
    FrameAllMessage[4] = Get_CRC8_Check_Sum(FrameAllMessage,5-1,CRC8_INIT);

    FrameAllMessage[5] = (unsigned char)0x0301;
    FrameAllMessage[6] = 0x0301>>8;
    FrameAllMessage[7] = (unsigned char)0xD180;
    FrameAllMessage[8] = 0xD180>>8;
    FrameAllMessage[9] = (unsigned char)ROBOT_TX_ID;
    FrameAllMessage[10]= ROBOT_TX_ID>>8;
    FrameAllMessage[11]= (unsigned char)ROBOT_RX_ID;
    FrameAllMessage[12]= ROBOT_RX_ID>>8;

    FrameAllMessage[13]= (*(uint32_t  *)&data1);
    FrameAllMessage[14]= (*(uint32_t  *)&data1)>>8;
    FrameAllMessage[15]= (*(uint32_t  *)&data1)>>16;
    FrameAllMessage[16]= (*(uint32_t  *)&data1)>>24;
    FrameAllMessage[17]= (*(uint32_t  *)&data2);
    FrameAllMessage[18]= (*(uint32_t  *)&data2)>>8;
    FrameAllMessage[19]= (*(uint32_t  *)&data2)>>16;
    FrameAllMessage[20]= (*(uint32_t  *)&data2)>>24;
    FrameAllMessage[21]= (*(uint32_t  *)&data3);
    FrameAllMessage[22]= (*(uint32_t  *)&data3)>>8;
    FrameAllMessage[23]= (*(uint32_t  *)&data3)>>16;
    FrameAllMessage[24]= (*(uint32_t  *)&data3)>>24;
    FrameAllMessage[25]= mask;

    FrameAllMessage[26] = CRC16_INIT;
    FrameAllMessage[27] = CRC16_INIT>>8;
    tempcrc16 = Get_CRC16_Check_Sum(FrameAllMessage,26,CRC16_INIT);
    FrameAllMessage[26] = tempcrc16&0xff;
    FrameAllMessage[27] = (tempcrc16>>8)&0xff;
    while(HAL_UART_Transmit(&huart6, FrameAllMessage, 28, 10) != HAL_OK);

}

void CustomSend(void)//5hz
{
    static uint8_t divider;
    customdata.fdata[0]=Chassis.supercapacitor.Voltage_filtered;
    customdata.fdata[1]=Chassis.supercapacitor.power_percentage*100;

    if(BtoADataBit.databit.b5==1)
        customdata.fdata[2]=11111;
    else
        customdata.fdata[2]=0;
//			if(Chassis.follow.mode.now==FRONT_FOLLOW)
//				customdata.fdata[2]=1;
//			else if(Chassis.follow.mode.now==FRONT_NOT_FOLLOW)
//				customdata.fdata[2]=0;
//			else if(Chassis.follow.mode.now==BACK_FOLLOW)
//				customdata.fdata[2]=1;
//			else if(Chassis.follow.mode.now==BACK_NOT_FOLLOW)
//				customdata.fdata[2]=0;
//			else if(Chassis.follow.mode.now==SPIN)
//				customdata.fdata[2]=2;

    if(TX2_Data.mode_now==OFFLINE)
        customdata.databit.databit.b1=0;
    else
        customdata.databit.databit.b1=1;

    customdata.databit.databit.b2=BtoADataBit.databit.b4;
//			customdata.databit.databit.b3=BtoADataBit.databit.b5;
    if(Chassis.power.unlimited==1)
        customdata.databit.databit.b3=1;
    else
        customdata.databit.databit.b3=0;
    customdata.databit.databit.b4=BtoADataBit.databit.b1;
    customdata.databit.databit.b5=BtoADataBit.databit.b2;
    customdata.databit.databit.b6=BtoADataBit.databit.b3;
    if(divider==0)
    {   //2.5hz
        CustomTransmit(customdata);
        divider=1;
    }
    else
    {   //2.5hz,0.4s
        if(beat<65535)
            beat++;
				if(beat%20==1)
					CustomShapeTransmit(&aimleft_1);
        if(beat%20==2)
					CustomShapeTransmit(&aimleft_2);
				if(beat%20==3)
					CustomShapeTransmit(&aimleft_3);
				if(beat%20==4)
					CustomShapeTransmit(&aimleft_4);
				if(beat%20==5)
					CustomShapeTransmit(&aimleft_5);
				if(beat%20==6)
					CustomShapeTransmit(&aimright_1);
				if(beat%20==7)
					CustomShapeTransmit(&aimright_2);
				if(beat%20==8)
					CustomShapeTransmit(&aimright_3);
				if(beat%20==9)
					CustomShapeTransmit(&aimright_4);
				if(beat%20==10)
					CustomShapeTransmit(&aimright_5);
				if(beat%20==11)
					CustomShapeTransmit(&aimshu_1);
				if(beat%20==12)
					CustomShapeTransmit(&aimshu_2);
				if(beat%20==13)
					CustomShapeTransmit(&rect_1);
				if(beat%20==14)
					CustomShapeTransmit(&circle_1);
				if(beat%20==15)
					CustomShapeTransmit(&ellipse_1);
				if(beat%20==16)
					CustomShapeTransmit(&text_1);
        divider=0;
    }


}

void CustomShapeTransmit(ext_client_graphic_draw_t *custom_shape)
{
    int i;

    u16 tempcrc16;
    //	mask = 0x3f;
    CustomShapeMessage[0] = 0xA5;
    CustomShapeMessage[1] = 61;
    CustomShapeMessage[2] = 61>>8;
    CustomShapeMessage[3] = 0;
    CustomShapeMessage[4] = Get_CRC8_Check_Sum(CustomShapeMessage,5-1,CRC8_INIT);

    CustomShapeMessage[5] = (unsigned char)0x0301;
    CustomShapeMessage[6] = 0x0301>>8;
    CustomShapeMessage[7] = (unsigned char)0x0100;
    CustomShapeMessage[8] = 0x0100>>8;
    CustomShapeMessage[9] = (unsigned char)ROBOT_TX_ID;
    CustomShapeMessage[10]= ROBOT_TX_ID>>8;
    CustomShapeMessage[11]= (unsigned char)ROBOT_RX_ID;
    CustomShapeMessage[12]= ROBOT_RX_ID>>8;

    if(1)
    {
			if(test==1)
			{
        custom_shape->operate_tpye = 1;

        custom_shape->graphic_tpye = 1;

        custom_shape->graphic_name[0] = 1;
        custom_shape->graphic_name[1] = 0;
        custom_shape->graphic_name[2] = 0;
        custom_shape->graphic_name[3] = 0;
        custom_shape->graphic_name[4] = 0;

        custom_shape->layer = 1;

        custom_shape->color = 3;

        custom_shape->width = 3;

        custom_shape->start_x = 900;
        custom_shape->start_y = 650;

        custom_shape->radius = 60;

        custom_shape->end_x = 1020;
        custom_shape->end_y = 700;
        custom_shape->start_angle = 0;
        custom_shape->end_angle = 0;
        custom_shape->text_lengh = 0;
        for(i=0; i<30; i++)
        {
            custom_shape->text[i]= 0;
        }
			}
        CustomShapeMessage[13]= custom_shape->operate_tpye;
        CustomShapeMessage[14]= custom_shape->graphic_tpye;
        CustomShapeMessage[15]= custom_shape->graphic_name[0];
        CustomShapeMessage[16]= custom_shape->graphic_name[1];
        CustomShapeMessage[17]= custom_shape->graphic_name[2];
        CustomShapeMessage[18]= custom_shape->graphic_name[3];
        CustomShapeMessage[19]= custom_shape->graphic_name[4];
        CustomShapeMessage[20]= custom_shape->layer;
        CustomShapeMessage[21]= custom_shape->color;
        CustomShapeMessage[22]= custom_shape->width;
        CustomShapeMessage[23]= custom_shape->start_x;
        CustomShapeMessage[24]= custom_shape->start_x>>8;
        CustomShapeMessage[25]= custom_shape->start_y;
        CustomShapeMessage[26]= custom_shape->start_y>>8;
        CustomShapeMessage[27]= custom_shape->radius;
        CustomShapeMessage[28]= custom_shape->radius>>8;
        CustomShapeMessage[29]= custom_shape->end_x;
        CustomShapeMessage[30]= custom_shape->end_x>>8;
        CustomShapeMessage[31]= custom_shape->end_y;
        CustomShapeMessage[32]= custom_shape->end_y>>8;
        CustomShapeMessage[33]= custom_shape->start_angle;
        CustomShapeMessage[34]= custom_shape->start_angle>>8;
        CustomShapeMessage[35]= custom_shape->end_angle;
        CustomShapeMessage[36]= custom_shape->end_angle>>8;
        CustomShapeMessage[37]= custom_shape->text_lengh;

        for(i=0; i<30; i++)
        {
            CustomShapeMessage[i+38]= custom_shape->text[i];
        }

        CustomShapeMessage[68] = CRC16_INIT;
        CustomShapeMessage[69] = CRC16_INIT>>8;
        tempcrc16 = Get_CRC16_Check_Sum(CustomShapeMessage,68,CRC16_INIT);
        CustomShapeMessage[68] = tempcrc16&0xff;
        CustomShapeMessage[69] = (tempcrc16>>8)&0xff;
        while(HAL_UART_Transmit(&huart6, CustomShapeMessage, 70, 10) != HAL_OK);
    
	}
}
//void CustomShapeTransmit_10000_aim(void)
//{

//    int i;
//    u16 tempcrc16;
//    //	mask = 0x3f;

//    if(beat%10==1)
//    {
//			if(test==1)
//			{
//        CustomShapeMessage[0] = 0xA5;
//        CustomShapeMessage[1] = 61;
//        CustomShapeMessage[2] = 61>>8;
//        CustomShapeMessage[3] = 0;
//        CustomShapeMessage[4] = Get_CRC8_Check_Sum(CustomShapeMessage,5-1,CRC8_INIT);

//        CustomShapeMessage[5] = (unsigned char)0x0301;
//        CustomShapeMessage[6] = 0x0301>>8;
//        CustomShapeMessage[7] = (unsigned char)0x0100;//
//        CustomShapeMessage[8] = 0x0100>>8;
//        CustomShapeMessage[9] = (unsigned char)ROBOT_TX_ID;
//        CustomShapeMessage[10]= ROBOT_TX_ID>>8;
//        CustomShapeMessage[11]= (unsigned char)ROBOT_RX_ID;
//        CustomShapeMessage[12]= ROBOT_RX_ID>>8;

//        custom_shape.operate_tpye = 1;		//增加图形

//        custom_shape.graphic_tpye = 1;		//直线

//        custom_shape.graphic_name[0] = 1;
//        custom_shape.graphic_name[1] = 0;
//        custom_shape.graphic_name[2] = 0;
//        custom_shape.graphic_name[3] = 0;
//        custom_shape.graphic_name[4] = 0;

//        custom_shape.layer = 1;

//        custom_shape.color = 3;			//白色

//        custom_shape.width = 1;			//线宽:1像素

//        custom_shape.start_x = aim_x-30;
//        custom_shape.start_y = aim_y-30;

//        custom_shape.radius = 0;

//        custom_shape.end_x = aim_x+30;
//        custom_shape.end_y = aim_y-30;

//        custom_shape.start_angle = 0;
//        custom_shape.end_angle = 0;
//        custom_shape.text_lengh = 0;
//        for(i=0; i<30; i++)
//        {
//            custom_shape.text[i]= 0;
//        }
//			}
//        CustomShapeMessage[13]= custom_shape.operate_tpye;
//        CustomShapeMessage[14]= custom_shape.graphic_tpye;
//        CustomShapeMessage[15]= custom_shape.graphic_name[0];
//        CustomShapeMessage[16]= custom_shape.graphic_name[1];
//        CustomShapeMessage[17]= custom_shape.graphic_name[2];
//        CustomShapeMessage[18]= custom_shape.graphic_name[3];
//        CustomShapeMessage[19]= custom_shape.graphic_name[4];
//        CustomShapeMessage[20]= custom_shape.layer;
//        CustomShapeMessage[21]= custom_shape.color;
//        CustomShapeMessage[22]= custom_shape.width;
//        CustomShapeMessage[23]= custom_shape.start_x;
//        CustomShapeMessage[24]= custom_shape.start_x>>8;
//        CustomShapeMessage[25]= custom_shape.start_y;
//        CustomShapeMessage[26]= custom_shape.start_y>>8;
//        CustomShapeMessage[27]= custom_shape.radius;
//        CustomShapeMessage[28]= custom_shape.radius>>8;
//        CustomShapeMessage[29]= custom_shape.end_x;
//        CustomShapeMessage[30]= custom_shape.end_x>>8;
//        CustomShapeMessage[31]= custom_shape.end_y;
//        CustomShapeMessage[32]= custom_shape.end_y>>8;
//        CustomShapeMessage[33]= custom_shape.start_angle;
//        CustomShapeMessage[34]= custom_shape.start_angle>>8;
//        CustomShapeMessage[35]= custom_shape.end_angle;
//        CustomShapeMessage[36]= custom_shape.end_angle>>8;
//        CustomShapeMessage[37]= custom_shape.text_lengh;

//        for(i=0; i<30; i++)
//        {
//            CustomShapeMessage[i+38]= custom_shape.text[i];
//        }

//        CustomShapeMessage[68] = CRC16_INIT;
//        CustomShapeMessage[69] = CRC16_INIT>>8;
//        tempcrc16 = Get_CRC16_Check_Sum(CustomShapeMessage,68,CRC16_INIT);
//        CustomShapeMessage[68] = tempcrc16&0xff;
//        CustomShapeMessage[69] = (tempcrc16>>8)&0xff;
//        while(HAL_UART_Transmit(&huart6, CustomShapeMessage, 70, 10) != HAL_OK);
//    }
//}
//void CustomShapeTransmit_20000_aimup(void)
//{

//    int i;
//    u16 tempcrc16;
//    //	mask = 0x3f;

//    if(beat==6)
//    {
//        CustomShapeMessage[0] = 0xA5;
//        CustomShapeMessage[1] = 61;
//        CustomShapeMessage[2] = 61>>8;
//        CustomShapeMessage[3] = 0;
//        CustomShapeMessage[4] = Get_CRC8_Check_Sum(CustomShapeMessage,5-1,CRC8_INIT);

//        CustomShapeMessage[5] = (unsigned char)0x0301;
//        CustomShapeMessage[6] = 0x0301>>8;
//        CustomShapeMessage[7] = (unsigned char)0x0100;//
//        CustomShapeMessage[8] = 0x0100>>8;
//        CustomShapeMessage[9] = (unsigned char)ROBOT_TX_ID;
//        CustomShapeMessage[10]= ROBOT_TX_ID>>8;
//        CustomShapeMessage[11]= (unsigned char)ROBOT_RX_ID;
//        CustomShapeMessage[12]= ROBOT_RX_ID>>8;

//        custom_shape.operate_tpye = 1;		//增加图形

//        custom_shape.graphic_tpye = 1;		//直线

//        custom_shape.graphic_name[0] = 6;
//        custom_shape.graphic_name[1] = 0;
//        custom_shape.graphic_name[2] = 0;
//        custom_shape.graphic_name[3] = 0;
//        custom_shape.graphic_name[4] = 0;

//        custom_shape.layer = 1;

//        custom_shape.color = 8;			//白色

//        custom_shape.width = 1;			//线宽:1像素

//        custom_shape.start_x = aim_x-30;
//        custom_shape.start_y = aim_y-30;

//        custom_shape.radius = 0;

//        custom_shape.end_x = aim_x+30;
//        custom_shape.end_y = aim_y-30;

//        custom_shape.start_angle = 0;
//        custom_shape.end_angle = 0;
//        custom_shape.text_lengh = 0;
//        for(i=0; i<30; i++)
//        {
//            custom_shape.text[i]= 0;
//        }
//        CustomShapeMessage[13]= custom_shape.operate_tpye;
//        CustomShapeMessage[14]= custom_shape.graphic_tpye;
//        CustomShapeMessage[15]= custom_shape.graphic_name[0];
//        CustomShapeMessage[16]= custom_shape.graphic_name[1];
//        CustomShapeMessage[17]= custom_shape.graphic_name[2];
//        CustomShapeMessage[18]= custom_shape.graphic_name[3];
//        CustomShapeMessage[19]= custom_shape.graphic_name[4];
//        CustomShapeMessage[20]= custom_shape.layer;
//        CustomShapeMessage[21]= custom_shape.color;
//        CustomShapeMessage[22]= custom_shape.width;
//        CustomShapeMessage[23]= custom_shape.start_x;
//        CustomShapeMessage[24]= custom_shape.start_x>>8;
//        CustomShapeMessage[25]= custom_shape.start_y;
//        CustomShapeMessage[26]= custom_shape.start_y>>8;
//        CustomShapeMessage[27]= custom_shape.radius;
//        CustomShapeMessage[28]= custom_shape.radius>>8;
//        CustomShapeMessage[29]= custom_shape.end_x;
//        CustomShapeMessage[30]= custom_shape.end_x>>8;
//        CustomShapeMessage[31]= custom_shape.end_y;
//        CustomShapeMessage[32]= custom_shape.end_y>>8;
//        CustomShapeMessage[33]= custom_shape.start_angle;
//        CustomShapeMessage[34]= custom_shape.start_angle>>8;
//        CustomShapeMessage[35]= custom_shape.end_angle;
//        CustomShapeMessage[36]= custom_shape.end_angle>>8;
//        CustomShapeMessage[37]= custom_shape.text_lengh;

//        for(i=0; i<30; i++)
//        {
//            CustomShapeMessage[i+38]= custom_shape.text[i];
//        }

//        CustomShapeMessage[68] = CRC16_INIT;
//        CustomShapeMessage[69] = CRC16_INIT>>8;
//        tempcrc16 = Get_CRC16_Check_Sum(CustomShapeMessage,68,CRC16_INIT);
//        CustomShapeMessage[68] = tempcrc16&0xff;
//        CustomShapeMessage[69] = (tempcrc16>>8)&0xff;
//        while(HAL_UART_Transmit(&huart6, CustomShapeMessage, 70, 10) != HAL_OK);
//    }
//}
//void CustomShapeTransmit_30000_aimdown(void)
//{

//    int i;
//    u16 tempcrc16;
//    //	mask = 0x3f;

//    if(beat==6)
//    {
//        CustomShapeMessage[0] = 0xA5;
//        CustomShapeMessage[1] = 61;
//        CustomShapeMessage[2] = 61>>8;
//        CustomShapeMessage[3] = 0;
//        CustomShapeMessage[4] = Get_CRC8_Check_Sum(CustomShapeMessage,5-1,CRC8_INIT);

//        CustomShapeMessage[5] = (unsigned char)0x0301;
//        CustomShapeMessage[6] = 0x0301>>8;
//        CustomShapeMessage[7] = (unsigned char)0x0100;//
//        CustomShapeMessage[8] = 0x0100>>8;
//        CustomShapeMessage[9] = (unsigned char)ROBOT_TX_ID;
//        CustomShapeMessage[10]= ROBOT_TX_ID>>8;
//        CustomShapeMessage[11]= (unsigned char)ROBOT_RX_ID;
//        CustomShapeMessage[12]= ROBOT_RX_ID>>8;

//        custom_shape.operate_tpye = 1;		//增加图形

//        custom_shape.graphic_tpye = 1;		//直线

//        custom_shape.graphic_name[0] = 6;
//        custom_shape.graphic_name[1] = 0;
//        custom_shape.graphic_name[2] = 0;
//        custom_shape.graphic_name[3] = 0;
//        custom_shape.graphic_name[4] = 0;

//        custom_shape.layer = 1;

//        custom_shape.color = 8;			//白色

//        custom_shape.width = 1;			//线宽:1像素

//        custom_shape.start_x = aim_x-30;
//        custom_shape.start_y = aim_y-30;

//        custom_shape.radius = 0;

//        custom_shape.end_x = aim_x+30;
//        custom_shape.end_y = aim_y-30;

//        custom_shape.start_angle = 0;
//        custom_shape.end_angle = 0;
//        custom_shape.text_lengh = 0;
//        for(i=0; i<30; i++)
//        {
//            custom_shape.text[i]= 0;
//        }
//        CustomShapeMessage[13]= custom_shape.operate_tpye;
//        CustomShapeMessage[14]= custom_shape.graphic_tpye;
//        CustomShapeMessage[15]= custom_shape.graphic_name[0];
//        CustomShapeMessage[16]= custom_shape.graphic_name[1];
//        CustomShapeMessage[17]= custom_shape.graphic_name[2];
//        CustomShapeMessage[18]= custom_shape.graphic_name[3];
//        CustomShapeMessage[19]= custom_shape.graphic_name[4];
//        CustomShapeMessage[20]= custom_shape.layer;
//        CustomShapeMessage[21]= custom_shape.color;
//        CustomShapeMessage[22]= custom_shape.width;
//        CustomShapeMessage[23]= custom_shape.start_x;
//        CustomShapeMessage[24]= custom_shape.start_x>>8;
//        CustomShapeMessage[25]= custom_shape.start_y;
//        CustomShapeMessage[26]= custom_shape.start_y>>8;
//        CustomShapeMessage[27]= custom_shape.radius;
//        CustomShapeMessage[28]= custom_shape.radius>>8;
//        CustomShapeMessage[29]= custom_shape.end_x;
//        CustomShapeMessage[30]= custom_shape.end_x>>8;
//        CustomShapeMessage[31]= custom_shape.end_y;
//        CustomShapeMessage[32]= custom_shape.end_y>>8;
//        CustomShapeMessage[33]= custom_shape.start_angle;
//        CustomShapeMessage[34]= custom_shape.start_angle>>8;
//        CustomShapeMessage[35]= custom_shape.end_angle;
//        CustomShapeMessage[36]= custom_shape.end_angle>>8;
//        CustomShapeMessage[37]= custom_shape.text_lengh;

//        for(i=0; i<30; i++)
//        {
//            CustomShapeMessage[i+38]= custom_shape.text[i];
//        }

//        CustomShapeMessage[68] = CRC16_INIT;
//        CustomShapeMessage[69] = CRC16_INIT>>8;
//        tempcrc16 = Get_CRC16_Check_Sum(CustomShapeMessage,68,CRC16_INIT);
//        CustomShapeMessage[68] = tempcrc16&0xff;
//        CustomShapeMessage[69] = (tempcrc16>>8)&0xff;
//        while(HAL_UART_Transmit(&huart6, CustomShapeMessage, 70, 10) != HAL_OK);
//    }
//}
//void CustomShapeTransmit_40000_aimleft(void)
//{

//    int i;
//    u16 tempcrc16;
//    //	mask = 0x3f;

//    if(beat==6)
//    {
//        CustomShapeMessage[0] = 0xA5;
//        CustomShapeMessage[1] = 61;
//        CustomShapeMessage[2] = 61>>8;
//        CustomShapeMessage[3] = 0;
//        CustomShapeMessage[4] = Get_CRC8_Check_Sum(CustomShapeMessage,5-1,CRC8_INIT);

//        CustomShapeMessage[5] = (unsigned char)0x0301;
//        CustomShapeMessage[6] = 0x0301>>8;
//        CustomShapeMessage[7] = (unsigned char)0x0100;//
//        CustomShapeMessage[8] = 0x0100>>8;
//        CustomShapeMessage[9] = (unsigned char)ROBOT_TX_ID;
//        CustomShapeMessage[10]= ROBOT_TX_ID>>8;
//        CustomShapeMessage[11]= (unsigned char)ROBOT_RX_ID;
//        CustomShapeMessage[12]= ROBOT_RX_ID>>8;

//        custom_shape.operate_tpye = 1;		//增加图形

//        custom_shape.graphic_tpye = 1;		//直线

//        custom_shape.graphic_name[0] = 6;
//        custom_shape.graphic_name[1] = 0;
//        custom_shape.graphic_name[2] = 0;
//        custom_shape.graphic_name[3] = 0;
//        custom_shape.graphic_name[4] = 0;

//        custom_shape.layer = 1;

//        custom_shape.color = 8;			//白色

//        custom_shape.width = 1;			//线宽:1像素

//        custom_shape.start_x = aim_x-30;
//        custom_shape.start_y = aim_y-30;

//        custom_shape.radius = 0;

//        custom_shape.end_x = aim_x+30;
//        custom_shape.end_y = aim_y-30;

//        custom_shape.start_angle = 0;
//        custom_shape.end_angle = 0;
//        custom_shape.text_lengh = 0;
//        for(i=0; i<30; i++)
//        {
//            custom_shape.text[i]= 0;
//        }
//        CustomShapeMessage[13]= custom_shape.operate_tpye;
//        CustomShapeMessage[14]= custom_shape.graphic_tpye;
//        CustomShapeMessage[15]= custom_shape.graphic_name[0];
//        CustomShapeMessage[16]= custom_shape.graphic_name[1];
//        CustomShapeMessage[17]= custom_shape.graphic_name[2];
//        CustomShapeMessage[18]= custom_shape.graphic_name[3];
//        CustomShapeMessage[19]= custom_shape.graphic_name[4];
//        CustomShapeMessage[20]= custom_shape.layer;
//        CustomShapeMessage[21]= custom_shape.color;
//        CustomShapeMessage[22]= custom_shape.width;
//        CustomShapeMessage[23]= custom_shape.start_x;
//        CustomShapeMessage[24]= custom_shape.start_x>>8;
//        CustomShapeMessage[25]= custom_shape.start_y;
//        CustomShapeMessage[26]= custom_shape.start_y>>8;
//        CustomShapeMessage[27]= custom_shape.radius;
//        CustomShapeMessage[28]= custom_shape.radius>>8;
//        CustomShapeMessage[29]= custom_shape.end_x;
//        CustomShapeMessage[30]= custom_shape.end_x>>8;
//        CustomShapeMessage[31]= custom_shape.end_y;
//        CustomShapeMessage[32]= custom_shape.end_y>>8;
//        CustomShapeMessage[33]= custom_shape.start_angle;
//        CustomShapeMessage[34]= custom_shape.start_angle>>8;
//        CustomShapeMessage[35]= custom_shape.end_angle;
//        CustomShapeMessage[36]= custom_shape.end_angle>>8;
//        CustomShapeMessage[37]= custom_shape.text_lengh;

//        for(i=0; i<30; i++)
//        {
//            CustomShapeMessage[i+38]= custom_shape.text[i];
//        }

//        CustomShapeMessage[68] = CRC16_INIT;
//        CustomShapeMessage[69] = CRC16_INIT>>8;
//        tempcrc16 = Get_CRC16_Check_Sum(CustomShapeMessage,68,CRC16_INIT);
//        CustomShapeMessage[68] = tempcrc16&0xff;
//        CustomShapeMessage[69] = (tempcrc16>>8)&0xff;
//        while(HAL_UART_Transmit(&huart6, CustomShapeMessage, 70, 10) != HAL_OK);
//    }
//}

//void CustomShapeTransmit_50000_aimright(void)
//{

//    int i;
//    u16 tempcrc16;
//    //	mask = 0x3f;

//    if(beat==6)
//    {
//        CustomShapeMessage[0] = 0xA5;
//        CustomShapeMessage[1] = 61;
//        CustomShapeMessage[2] = 61>>8;
//        CustomShapeMessage[3] = 0;
//        CustomShapeMessage[4] = Get_CRC8_Check_Sum(CustomShapeMessage,5-1,CRC8_INIT);

//        CustomShapeMessage[5] = (unsigned char)0x0301;
//        CustomShapeMessage[6] = 0x0301>>8;
//        CustomShapeMessage[7] = (unsigned char)0x0100;//
//        CustomShapeMessage[8] = 0x0100>>8;
//        CustomShapeMessage[9] = (unsigned char)ROBOT_TX_ID;
//        CustomShapeMessage[10]= ROBOT_TX_ID>>8;
//        CustomShapeMessage[11]= (unsigned char)ROBOT_RX_ID;
//        CustomShapeMessage[12]= ROBOT_RX_ID>>8;

//        custom_shape.operate_tpye = 1;		//增加图形

//        custom_shape.graphic_tpye = 1;		//直线

//        custom_shape.graphic_name[0] = 6;
//        custom_shape.graphic_name[1] = 0;
//        custom_shape.graphic_name[2] = 0;
//        custom_shape.graphic_name[3] = 0;
//        custom_shape.graphic_name[4] = 0;

//        custom_shape.layer = 1;

//        custom_shape.color = 8;			//白色

//        custom_shape.width = 1;			//线宽:1像素

//        custom_shape.start_x = aim_x-30;
//        custom_shape.start_y = aim_y-30;

//        custom_shape.radius = 0;

//        custom_shape.end_x = aim_x+30;
//        custom_shape.end_y = aim_y-30;

//        custom_shape.start_angle = 0;
//        custom_shape.end_angle = 0;
//        custom_shape.text_lengh = 0;
//        for(i=0; i<30; i++)
//        {
//            custom_shape.text[i]= 0;
//        }
//        CustomShapeMessage[13]= custom_shape.operate_tpye;
//        CustomShapeMessage[14]= custom_shape.graphic_tpye;
//        CustomShapeMessage[15]= custom_shape.graphic_name[0];
//        CustomShapeMessage[16]= custom_shape.graphic_name[1];
//        CustomShapeMessage[17]= custom_shape.graphic_name[2];
//        CustomShapeMessage[18]= custom_shape.graphic_name[3];
//        CustomShapeMessage[19]= custom_shape.graphic_name[4];
//        CustomShapeMessage[20]= custom_shape.layer;
//        CustomShapeMessage[21]= custom_shape.color;
//        CustomShapeMessage[22]= custom_shape.width;
//        CustomShapeMessage[23]= custom_shape.start_x;
//        CustomShapeMessage[24]= custom_shape.start_x>>8;
//        CustomShapeMessage[25]= custom_shape.start_y;
//        CustomShapeMessage[26]= custom_shape.start_y>>8;
//        CustomShapeMessage[27]= custom_shape.radius;
//        CustomShapeMessage[28]= custom_shape.radius>>8;
//        CustomShapeMessage[29]= custom_shape.end_x;
//        CustomShapeMessage[30]= custom_shape.end_x>>8;
//        CustomShapeMessage[31]= custom_shape.end_y;
//        CustomShapeMessage[32]= custom_shape.end_y>>8;
//        CustomShapeMessage[33]= custom_shape.start_angle;
//        CustomShapeMessage[34]= custom_shape.start_angle>>8;
//        CustomShapeMessage[35]= custom_shape.end_angle;
//        CustomShapeMessage[36]= custom_shape.end_angle>>8;
//        CustomShapeMessage[37]= custom_shape.text_lengh;

//        for(i=0; i<30; i++)
//        {
//            CustomShapeMessage[i+38]= custom_shape.text[i];
//        }

//        CustomShapeMessage[68] = CRC16_INIT;
//        CustomShapeMessage[69] = CRC16_INIT>>8;
//        tempcrc16 = Get_CRC16_Check_Sum(CustomShapeMessage,68,CRC16_INIT);
//        CustomShapeMessage[68] = tempcrc16&0xff;
//        CustomShapeMessage[69] = (tempcrc16>>8)&0xff;
//        while(HAL_UART_Transmit(&huart6, CustomShapeMessage, 70, 10) != HAL_OK);
//    }
//}

//void CustomShapeTransmit_60000_level1(void)
//{

//    int i;
//    u16 tempcrc16;
//    //	mask = 0x3f;

//    if(beat==6)
//    {
//        CustomShapeMessage[0] = 0xA5;
//        CustomShapeMessage[1] = 61;
//        CustomShapeMessage[2] = 61>>8;
//        CustomShapeMessage[3] = 0;
//        CustomShapeMessage[4] = Get_CRC8_Check_Sum(CustomShapeMessage,5-1,CRC8_INIT);

//        CustomShapeMessage[5] = (unsigned char)0x0301;
//        CustomShapeMessage[6] = 0x0301>>8;
//        CustomShapeMessage[7] = (unsigned char)0x0100;//
//        CustomShapeMessage[8] = 0x0100>>8;
//        CustomShapeMessage[9] = (unsigned char)ROBOT_TX_ID;
//        CustomShapeMessage[10]= ROBOT_TX_ID>>8;
//        CustomShapeMessage[11]= (unsigned char)ROBOT_RX_ID;
//        CustomShapeMessage[12]= ROBOT_RX_ID>>8;

//        custom_shape.operate_tpye = 1;		//增加图形

//        custom_shape.graphic_tpye = 1;		//直线

//        custom_shape.graphic_name[0] = 6;
//        custom_shape.graphic_name[1] = 0;
//        custom_shape.graphic_name[2] = 0;
//        custom_shape.graphic_name[3] = 0;
//        custom_shape.graphic_name[4] = 0;

//        custom_shape.layer = 1;

//        custom_shape.color = 8;			//白色

//        custom_shape.width = 1;			//线宽:1像素

//        custom_shape.start_x = aim_x-30;
//        custom_shape.start_y = aim_y-30;

//        custom_shape.radius = 0;

//        custom_shape.end_x = aim_x+30;
//        custom_shape.end_y = aim_y-30;

//        custom_shape.start_angle = 0;
//        custom_shape.end_angle = 0;
//        custom_shape.text_lengh = 0;
//        for(i=0; i<30; i++)
//        {
//            custom_shape.text[i]= 0;
//        }
//        CustomShapeMessage[13]= custom_shape.operate_tpye;
//        CustomShapeMessage[14]= custom_shape.graphic_tpye;
//        CustomShapeMessage[15]= custom_shape.graphic_name[0];
//        CustomShapeMessage[16]= custom_shape.graphic_name[1];
//        CustomShapeMessage[17]= custom_shape.graphic_name[2];
//        CustomShapeMessage[18]= custom_shape.graphic_name[3];
//        CustomShapeMessage[19]= custom_shape.graphic_name[4];
//        CustomShapeMessage[20]= custom_shape.layer;
//        CustomShapeMessage[21]= custom_shape.color;
//        CustomShapeMessage[22]= custom_shape.width;
//        CustomShapeMessage[23]= custom_shape.start_x;
//        CustomShapeMessage[24]= custom_shape.start_x>>8;
//        CustomShapeMessage[25]= custom_shape.start_y;
//        CustomShapeMessage[26]= custom_shape.start_y>>8;
//        CustomShapeMessage[27]= custom_shape.radius;
//        CustomShapeMessage[28]= custom_shape.radius>>8;
//        CustomShapeMessage[29]= custom_shape.end_x;
//        CustomShapeMessage[30]= custom_shape.end_x>>8;
//        CustomShapeMessage[31]= custom_shape.end_y;
//        CustomShapeMessage[32]= custom_shape.end_y>>8;
//        CustomShapeMessage[33]= custom_shape.start_angle;
//        CustomShapeMessage[34]= custom_shape.start_angle>>8;
//        CustomShapeMessage[35]= custom_shape.end_angle;
//        CustomShapeMessage[36]= custom_shape.end_angle>>8;
//        CustomShapeMessage[37]= custom_shape.text_lengh;

//        for(i=0; i<30; i++)
//        {
//            CustomShapeMessage[i+38]= custom_shape.text[i];
//        }

//        CustomShapeMessage[68] = CRC16_INIT;
//        CustomShapeMessage[69] = CRC16_INIT>>8;
//        tempcrc16 = Get_CRC16_Check_Sum(CustomShapeMessage,68,CRC16_INIT);
//        CustomShapeMessage[68] = tempcrc16&0xff;
//        CustomShapeMessage[69] = (tempcrc16>>8)&0xff;
//        while(HAL_UART_Transmit(&huart6, CustomShapeMessage, 70, 10) != HAL_OK);
//    }
//}

//void CustomShapeClearAll(void)
//{
//    int i;
//    u16 tempcrc16;
//    //	mask = 0x3f;
//    CustomShapeMessage[0] = 0xA5;
//    CustomShapeMessage[1] = 61;
//    CustomShapeMessage[2] = 61>>8;
//    CustomShapeMessage[3] = 0;
//    CustomShapeMessage[4] = Get_CRC8_Check_Sum(CustomShapeMessage,5-1,CRC8_INIT);

//    CustomShapeMessage[5] = (unsigned char)0x0301;
//    CustomShapeMessage[6] = 0x0301>>8;
//    CustomShapeMessage[7] = (unsigned char)0x0100;
//    CustomShapeMessage[8] = 0x0100>>8;
//    CustomShapeMessage[9] = (unsigned char)ROBOT_TX_ID;
//    CustomShapeMessage[10]= ROBOT_TX_ID>>8;
//    CustomShapeMessage[11]= (unsigned char)ROBOT_RX_ID;
//    CustomShapeMessage[12]= ROBOT_RX_ID>>8;

//    custom_shape.operate_tpye = 6;
//    custom_shape.graphic_tpye = 3;
//    custom_shape.graphic_name[0] = 2;
//    custom_shape.graphic_name[1] = 0;
//    custom_shape.graphic_name[2] = 0;
//    custom_shape.graphic_name[3] = 0;
//    custom_shape.graphic_name[4] = 0;
//    custom_shape.layer = 9;
//    custom_shape.color = 1;
//    custom_shape.width = 0x0A;
//    custom_shape.start_x = 0x021C;
//    custom_shape.start_y = 0x021C;
//    custom_shape.radius = 0X0064;
//    custom_shape.end_x = 0;
//    custom_shape.end_y = 0;
//    custom_shape.start_angle = 0;
//    custom_shape.end_angle = 0;
//    custom_shape.text_lengh = 0;
//    for(i=0; i<30; i++)
//    {
//        custom_shape.text[i]= 0;
//    }
//    CustomShapeMessage[13]= custom_shape.operate_tpye;
//    CustomShapeMessage[14]= custom_shape.graphic_tpye;
//    CustomShapeMessage[15]= custom_shape.graphic_name[0];
//    CustomShapeMessage[16]= custom_shape.graphic_name[1];
//    CustomShapeMessage[17]= custom_shape.graphic_name[2];
//    CustomShapeMessage[18]= custom_shape.graphic_name[3];
//    CustomShapeMessage[19]= custom_shape.graphic_name[4];
//    CustomShapeMessage[20]= custom_shape.layer;
//    CustomShapeMessage[21]= custom_shape.color;
//    CustomShapeMessage[22]= custom_shape.width;
//    CustomShapeMessage[23]= custom_shape.start_x;
//    CustomShapeMessage[24]= custom_shape.start_x>>8;
//    CustomShapeMessage[25]= custom_shape.start_y;
//    CustomShapeMessage[26]= custom_shape.start_y>>8;
//    CustomShapeMessage[27]= custom_shape.radius;
//    CustomShapeMessage[28]= custom_shape.radius>>8;
//    CustomShapeMessage[29]= custom_shape.end_x;
//    CustomShapeMessage[30]= custom_shape.end_x>>8;
//    CustomShapeMessage[31]= custom_shape.end_y;
//    CustomShapeMessage[32]= custom_shape.end_y>>8;
//    CustomShapeMessage[33]= custom_shape.start_angle;
//    CustomShapeMessage[34]= custom_shape.start_angle>>8;
//    CustomShapeMessage[35]= custom_shape.end_angle;
//    CustomShapeMessage[36]= custom_shape.end_angle>>8;
//    CustomShapeMessage[37]= custom_shape.text_lengh;
//    for(i=0; i<30; i++)
//    {
//        CustomShapeMessage[i+38]= custom_shape.text[i];
//    }

//    CustomShapeMessage[68] = CRC16_INIT;
//    CustomShapeMessage[69] = CRC16_INIT>>8;
//    tempcrc16 = Get_CRC16_Check_Sum(CustomShapeMessage,68,CRC16_INIT);
//    CustomShapeMessage[68] = tempcrc16&0xff;
//    CustomShapeMessage[69] = (tempcrc16>>8)&0xff;
//    while(HAL_UART_Transmit(&huart6, CustomShapeMessage, 70, 10) != HAL_OK);

//}
