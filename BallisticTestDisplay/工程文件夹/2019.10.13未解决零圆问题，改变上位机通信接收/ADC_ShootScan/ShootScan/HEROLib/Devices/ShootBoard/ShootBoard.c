#include "ShootBoard.h"
#include "ComToMaster.h"

#include "adc.h"
#include "FIFO.h"

//正方向：左上角为原点，分别向下向右为正

#define CLEAN_AWDG_FLAG (hadc1.Instance->SR &= ~(0x01))

//启动标识(当第一次连接到上位机时，此标志置一，并将向上位机请求一次采样阈值)
bool isStart = false;

//边界阈值数据
uint16_t BoundX_Min;
uint16_t BoundX_Max;
uint16_t BoundY_Min;
uint16_t BoundY_Max;

//触发阈值
uint16_t Trig_X;
uint16_t Trig_Y;

//转换缓冲区（0为Y，1为X）
uint16_t ConvertBuffer[2] = {0};

//波谷值
uint16_t Trough_X = ADC_MAX_VALUE;
uint16_t Trough_Y = ADC_MAX_VALUE;


//标记是否是未发出的数据，若为未发出的数据，将会把老数据丢弃，存入新数据
bool isNewData = false;

//标记是否一波采样已经完成
bool isSampleComplet = false;

//丢弃计数器，标记丢弃了多少次数据
uint32_t DiscardCnt = 0;

//向上位机回传的采样值
SampleDataTypedef EmptyData = {0};
SampleDataTypedef SampleData = {0};

uint16_t SendFIFO[SEND_BUF_SIZE][2];
FIFO_Typedef SendDataFIFO;

//采样数据缓冲区(循环队列)
#define FIFO_SIZE 250
uint16_t SampleFIFO[FIFO_SIZE][2];
uint32_t DiscardSampleCnt = 0;
FIFO_Typedef SampleDataFIFO;

//记录长度计数器
#define SAVE_SIZE 200
uint32_t SaveCnt = 0;

//写保护互斥锁：防止在正写入数据的时候把缓冲区发出去
bool isWritingData = false;

//校准模式标志
bool isCorrectMode = false;

//私有函数声明
void CaptureShoot(uint16_t Sample_X, uint16_t Sample_Y);

//限幅函数
uint16_t Limit_uint16(uint16_t amt, uint16_t low, uint16_t high) 
{
	return ((amt)<(low) ? (low) : ((amt)>(high)?(high):(amt)));
}

//消除初始化标志
void DeInitBoard(void)
{
    isStart = false;
}

//执行一次启动流程，并置位启动标识
void GetInitData(void)
{
    //如果已经初始化过了就不做任何操作
    if(isStart) { return; }
    
    //在缓冲区写入指令内容
    CtrlCMD.CMD = Board_Start;
    
    //请求发送数据
    RequestSend(MstrTx_CtrlCMD);
}

//初始化完成，执行初始化扫描流程
void StartScan(uint16_t TrigValue)
{
    //标记已经启动扫描
    isStart  =true;
        
    //初始化波峰波谷值
    Trough_X = BoundX_Max;
    Trough_Y = BoundY_Max;    
    
    //初始化数据FIFO
    FIFO_Init(&SampleDataFIFO, SampleFIFO, FIFO_SIZE, sizeof(SampleFIFO[0]));
    
    //初始化发送数据FIFO
    FIFO_Init(&SendDataFIFO, SendFIFO, SEND_BUF_SIZE, sizeof(SendFIFO[0]));
    
    //清空丢弃计数器
    DiscardCnt = 0;
    
    //初始化的时候先关中断，只是转换和传输
    HAL_NVIC_DisableIRQ(DMA1_Channel1_IRQn);
    
    //设定模拟看门狗阈值
    SetTrigValue(TrigValue, ADC_MAX_VALUE);
        
    //开始传输
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ConvertBuffer, 2);
    
}

//设定DMA是循环模式还是非循环模式
void ADC_DMA_NormalModeEnable(bool isEnable)
{
    if(isEnable)
    {
        //置零寄存器位
        hadc1.DMA_Handle->Instance->CCR &= ~(0x01 << 5);
    }
    else
    {
        //置一寄存器位
        hadc1.DMA_Handle->Instance->CCR |= (0x01 << 5);
    }
}

//设定脉冲捕获触发阈限（下限），值域为0~4096.非法输入会被限制在此范围内。
void SetTrigValue(uint16_t LowTrigValue, uint16_t HighTrigValue)
{
    //限幅
    HighTrigValue = Limit_uint16(HighTrigValue, 0, 4096);
    LowTrigValue = Limit_uint16(LowTrigValue, 0, 4096);
    
    //设置时防止中断误触发
    HAL_NVIC_DisableIRQ(ADC1_2_IRQn);
    
    //设定寄存器的值
    hadc1.Instance->HTR = HighTrigValue;   //更改高阈值
    hadc1.Instance->LTR = LowTrigValue;    //更改低阈值
    
    //清除模拟看门狗标志
    CLEAN_AWDG_FLAG;
    
    //开中断
    HAL_NVIC_EnableIRQ(ADC1_2_IRQn);
}

//复位数据记录器
void ResetDataLog(void)
{
    Trough_X = BoundX_Max;
    Trough_Y = BoundY_Max;
}

//将数据存入缓冲区（在DMA中断中调用）
void SaveSampleData(void)
{
    uint16_t TempBuffer[2] = {0};
    
    TempBuffer[0] = ConvertBuffer[1];    //存入X
    TempBuffer[1] = ConvertBuffer[0];    //存入Y
    
    //判断是否已经存满，存满则丢弃新数据，丢弃计数器自增。
    if(!FIFO_SaveData(&SampleDataFIFO, TempBuffer))
    {
        DiscardSampleCnt++;
    }
    
    SaveCnt++;
    
    //若是记够了数据，就关闭中断
    if(SaveCnt > (SAVE_SIZE - 1))
    {
        SaveCnt = 0;
        
        //关中断，只是转换和传输
        HAL_NVIC_DisableIRQ(DMA1_Channel1_IRQn);
                
        //标记一波采样已完成
        isSampleComplet = true;
    }
}

//抽空处理采样数据，在主函数中调用
void DealSampleData(void)
{
    uint16_t TempBuffer[2] = {0};
    
    //如果队列非空，那么从中读走数据
    if(FIFO_ReadData(&SampleDataFIFO, TempBuffer))
    {
        //处理数据
        CaptureShoot(TempBuffer[0], TempBuffer[1]);
    }
    else if(isSampleComplet) //如果采样结束并且已经把缓冲区里的数据处理完了
    {
        isSampleComplet = false;
        
        //记录此捕获值
        TempBuffer[0] = Trough_X;
        TempBuffer[1] = Trough_Y;
        
        isWritingData = true;
        
        //写入缓冲区
        if(!FIFO_SaveData(&SendDataFIFO, TempBuffer))
        {
            //标记自此开始丢弃数据
            isNewData = true;
        }
        
        isWritingData = false;
        
        //计算丢弃的数据个数
        if(isNewData)
        {
            DiscardCnt++;
        }
        
        //产生发送请求
        if(isCorrectMode)
        {
            RequestSend(MstrTx_CorrectData);
        }
        else
        {
            RequestSend(MstrTx_SampleData);
        }
        
        //复位记录器
        ResetDataLog();
    }

}

//辨识击中点，坐标为尖脉冲峰值，
//捕获判据为最大值一段时间内没有被突破，此“一段时间”是通过击打实验获得的撞击时间和输入滤波的时间常数算出的。
//两个弹丸射来的最大频率假定为100Hz，那么应有至少10ms的回复时间，也就是在这段时间内电压都应当处于上升趋势，直到到了上边界。
void CaptureShoot(uint16_t Sample_X, uint16_t Sample_Y)
{    
    //先X后Y,限幅控制
    Sample_X = Limit_uint16(Sample_X, BoundX_Min, BoundX_Max);
    Sample_Y = Limit_uint16(Sample_Y, BoundY_Min, BoundY_Max);
            
    //记录较小的值
    Trough_X = Trough_X < Sample_X ? Trough_X : Sample_X;
    Trough_Y = Trough_Y < Sample_Y ? Trough_Y : Sample_Y;
    
}

//获取采样数据并复位新数据标志
SampleDataTypedef GetSampleData(void)
{
    int TempData = SendDataFIFO.TailPointer - SendDataFIFO.HeadPointer;
    uint32_t Cnt = 0;
        
    //标记此数据已发出
    isNewData = false;
    
    SampleData.DataSize = TempData < 0 ? (TempData + SendDataFIFO.FIFO_Size) : TempData;
    
    //将FIFO中的数据全部读出
    while(FIFO_ReadData(&SendDataFIFO, SampleData.SampleBuffer + Cnt))
    {
        Cnt++;
    }
    
    return SampleData;
}
