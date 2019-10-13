#include "ShootBoard.h"
#include "ComToMaster.h"

#include "adc.h"
#include "FIFO.h"

//���������Ͻ�Ϊԭ�㣬�ֱ���������Ϊ��

#define CLEAN_AWDG_FLAG (hadc1.Instance->SR &= ~(0x01))

//������ʶ(����һ�����ӵ���λ��ʱ���˱�־��һ����������λ������һ�β�����ֵ)
bool isStart = false;

//�߽���ֵ����
uint16_t BoundX_Min;
uint16_t BoundX_Max;
uint16_t BoundY_Min;
uint16_t BoundY_Max;

//������ֵ
uint16_t Trig_X;
uint16_t Trig_Y;

//ת����������0ΪY��1ΪX��
uint16_t ConvertBuffer[2] = {0};

//����ֵ
uint16_t Trough_X = ADC_MAX_VALUE;
uint16_t Trough_Y = ADC_MAX_VALUE;


//����Ƿ���δ���������ݣ���Ϊδ���������ݣ�����������ݶ���������������
bool isNewData = false;

//����Ƿ�һ�������Ѿ����
bool isSampleComplet = false;

//��������������Ƕ����˶��ٴ�����
uint32_t DiscardCnt = 0;

//����λ���ش��Ĳ���ֵ
SampleDataTypedef EmptyData = {0};
SampleDataTypedef SampleData = {0};

uint16_t SendFIFO[SEND_BUF_SIZE][2];
FIFO_Typedef SendDataFIFO;

//�������ݻ�����(ѭ������)
#define FIFO_SIZE 250
uint16_t SampleFIFO[FIFO_SIZE][2];
uint32_t DiscardSampleCnt = 0;
FIFO_Typedef SampleDataFIFO;

//��¼���ȼ�����
#define SAVE_SIZE 200
uint32_t SaveCnt = 0;

//д��������������ֹ����д�����ݵ�ʱ��ѻ���������ȥ
bool isWritingData = false;

//У׼ģʽ��־
bool isCorrectMode = false;

//˽�к�������
void CaptureShoot(uint16_t Sample_X, uint16_t Sample_Y);

//�޷�����
uint16_t Limit_uint16(uint16_t amt, uint16_t low, uint16_t high) 
{
	return ((amt)<(low) ? (low) : ((amt)>(high)?(high):(amt)));
}

//������ʼ����־
void DeInitBoard(void)
{
    isStart = false;
}

//ִ��һ���������̣�����λ������ʶ
void GetInitData(void)
{
    //����Ѿ���ʼ�����˾Ͳ����κβ���
    if(isStart) { return; }
    
    //�ڻ�����д��ָ������
    CtrlCMD.CMD = Board_Start;
    
    //����������
    RequestSend(MstrTx_CtrlCMD);
}

//��ʼ����ɣ�ִ�г�ʼ��ɨ������
void StartScan(uint16_t TrigValue)
{
    //����Ѿ�����ɨ��
    isStart  =true;
        
    //��ʼ�����岨��ֵ
    Trough_X = BoundX_Max;
    Trough_Y = BoundY_Max;    
    
    //��ʼ������FIFO
    FIFO_Init(&SampleDataFIFO, SampleFIFO, FIFO_SIZE, sizeof(SampleFIFO[0]));
    
    //��ʼ����������FIFO
    FIFO_Init(&SendDataFIFO, SendFIFO, SEND_BUF_SIZE, sizeof(SendFIFO[0]));
    
    //��ն���������
    DiscardCnt = 0;
    
    //��ʼ����ʱ���ȹ��жϣ�ֻ��ת���ʹ���
    HAL_NVIC_DisableIRQ(DMA1_Channel1_IRQn);
    
    //�趨ģ�⿴�Ź���ֵ
    SetTrigValue(TrigValue, ADC_MAX_VALUE);
        
    //��ʼ����
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ConvertBuffer, 2);
    
}

//�趨DMA��ѭ��ģʽ���Ƿ�ѭ��ģʽ
void ADC_DMA_NormalModeEnable(bool isEnable)
{
    if(isEnable)
    {
        //����Ĵ���λ
        hadc1.DMA_Handle->Instance->CCR &= ~(0x01 << 5);
    }
    else
    {
        //��һ�Ĵ���λ
        hadc1.DMA_Handle->Instance->CCR |= (0x01 << 5);
    }
}

//�趨���岶�񴥷����ޣ����ޣ���ֵ��Ϊ0~4096.�Ƿ�����ᱻ�����ڴ˷�Χ�ڡ�
void SetTrigValue(uint16_t LowTrigValue, uint16_t HighTrigValue)
{
    //�޷�
    HighTrigValue = Limit_uint16(HighTrigValue, 0, 4096);
    LowTrigValue = Limit_uint16(LowTrigValue, 0, 4096);
    
    //����ʱ��ֹ�ж��󴥷�
    HAL_NVIC_DisableIRQ(ADC1_2_IRQn);
    
    //�趨�Ĵ�����ֵ
    hadc1.Instance->HTR = HighTrigValue;   //���ĸ���ֵ
    hadc1.Instance->LTR = LowTrigValue;    //���ĵ���ֵ
    
    //���ģ�⿴�Ź���־
    CLEAN_AWDG_FLAG;
    
    //���ж�
    HAL_NVIC_EnableIRQ(ADC1_2_IRQn);
}

//��λ���ݼ�¼��
void ResetDataLog(void)
{
    Trough_X = BoundX_Max;
    Trough_Y = BoundY_Max;
}

//�����ݴ��뻺��������DMA�ж��е��ã�
void SaveSampleData(void)
{
    uint16_t TempBuffer[2] = {0};
    
    TempBuffer[0] = ConvertBuffer[1];    //����X
    TempBuffer[1] = ConvertBuffer[0];    //����Y
    
    //�ж��Ƿ��Ѿ��������������������ݣ�����������������
    if(!FIFO_SaveData(&SampleDataFIFO, TempBuffer))
    {
        DiscardSampleCnt++;
    }
    
    SaveCnt++;
    
    //���Ǽǹ������ݣ��͹ر��ж�
    if(SaveCnt > (SAVE_SIZE - 1))
    {
        SaveCnt = 0;
        
        //���жϣ�ֻ��ת���ʹ���
        HAL_NVIC_DisableIRQ(DMA1_Channel1_IRQn);
                
        //���һ�����������
        isSampleComplet = true;
    }
}

//��մ���������ݣ����������е���
void DealSampleData(void)
{
    uint16_t TempBuffer[2] = {0};
    
    //������зǿգ���ô���ж�������
    if(FIFO_ReadData(&SampleDataFIFO, TempBuffer))
    {
        //��������
        CaptureShoot(TempBuffer[0], TempBuffer[1]);
    }
    else if(isSampleComplet) //����������������Ѿ��ѻ�����������ݴ�������
    {
        isSampleComplet = false;
        
        //��¼�˲���ֵ
        TempBuffer[0] = Trough_X;
        TempBuffer[1] = Trough_Y;
        
        isWritingData = true;
        
        //д�뻺����
        if(!FIFO_SaveData(&SendDataFIFO, TempBuffer))
        {
            //����Դ˿�ʼ��������
            isNewData = true;
        }
        
        isWritingData = false;
        
        //���㶪�������ݸ���
        if(isNewData)
        {
            DiscardCnt++;
        }
        
        //������������
        if(isCorrectMode)
        {
            RequestSend(MstrTx_CorrectData);
        }
        else
        {
            RequestSend(MstrTx_SampleData);
        }
        
        //��λ��¼��
        ResetDataLog();
    }

}

//��ʶ���е㣬����Ϊ�������ֵ��
//�����о�Ϊ���ֵһ��ʱ����û�б�ͻ�ƣ��ˡ�һ��ʱ�䡱��ͨ������ʵ���õ�ײ��ʱ��������˲���ʱ�䳣������ġ�
//�����������������Ƶ�ʼٶ�Ϊ100Hz����ôӦ������10ms�Ļظ�ʱ�䣬Ҳ���������ʱ���ڵ�ѹ��Ӧ�������������ƣ�ֱ�������ϱ߽硣
void CaptureShoot(uint16_t Sample_X, uint16_t Sample_Y)
{    
    //��X��Y,�޷�����
    Sample_X = Limit_uint16(Sample_X, BoundX_Min, BoundX_Max);
    Sample_Y = Limit_uint16(Sample_Y, BoundY_Min, BoundY_Max);
            
    //��¼��С��ֵ
    Trough_X = Trough_X < Sample_X ? Trough_X : Sample_X;
    Trough_Y = Trough_Y < Sample_Y ? Trough_Y : Sample_Y;
    
}

//��ȡ�������ݲ���λ�����ݱ�־
SampleDataTypedef GetSampleData(void)
{
    int TempData = SendDataFIFO.TailPointer - SendDataFIFO.HeadPointer;
    uint32_t Cnt = 0;
        
    //��Ǵ������ѷ���
    isNewData = false;
    
    SampleData.DataSize = TempData < 0 ? (TempData + SendDataFIFO.FIFO_Size) : TempData;
    
    //��FIFO�е�����ȫ������
    while(FIFO_ReadData(&SendDataFIFO, SampleData.SampleBuffer + Cnt))
    {
        Cnt++;
    }
    
    return SampleData;
}
