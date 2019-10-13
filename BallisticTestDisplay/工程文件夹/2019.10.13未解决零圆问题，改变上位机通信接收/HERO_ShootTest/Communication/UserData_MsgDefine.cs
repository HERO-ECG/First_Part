using System.Collections.Generic;
using System.Runtime.InteropServices;


namespace Communication
{
    //意图以后可以在UI上定制通信协议
    public partial class UserDatas : DataTools
    {
        #region 数据格式

        //数据包的格式化数据头
        public struct Globle_Datahead//8
        {
            public byte Data_Head1;    //固定数据头1（0xaa）
            public byte Data_Head2;    //固定数据头2（0xbb）
            public byte CMD_type;      //指令类型（标识数据类型）@ref enum ROV_MsgTypesID
            public byte DataFeedback;  //下位机接收到的数据类型反馈 @ref enum DataFeedbackTypes

            public int HeadVerify;    //数据头校验，相乘校验。用于辅助辨识数据头
        };

        //数据包的格式化数据尾
        public struct Globle_Datatail//4
        {
            public uint verify;        //校验（求和）
        };
        #endregion

        #region 数据内容

        //注意，C#中bool类型占四个字，而单片机中占一个字，枚举类型同理。
        //因此，需要注意把协议中涉及的逻辑变量改成byte型。

        //测试控制指令
        public struct CtrlCMD//4
        {
            public int CMD; //指令
        }

        public const int SEND_BUF_SIZE = 6;

        //结构体定义：校准采样值
        unsafe public struct SampleData//28
        {
            public ushort DataSize;      //数据个数
            public ushort Reserve;      //预留占位符

            //0为X，1为Y，二维数组
            public fixed ushort SampleFIFO[SEND_BUF_SIZE * 2];

        };

        //结构体定义：初始化数据
        public struct InitData
        {
            public ushort X_Max_Voltage;
            public ushort X_Min_Voltage;
            public ushort Y_Max_Voltage;
            public ushort Y_Min_Voltage;

            public ushort X_TrigValue;
            public ushort Y_TrigValue;
        };
        #endregion

        #region 常量和枚举标识

        //格式化数据头
        public const byte Datahead1 = 0xaa, Datahead2 = 0xbb;

        //通信数据类型标识集合（发送和接收分别从0开始，一一对应）
        public enum MsgTypesID
        {
            /***********  发送组  *************/
            //0 - 异常标识
            Tx_ErrorData = 0x00,
            //1 - 发送空指令
            Tx_EmptyCMD,
            //2 - 发送控制指令
            Tx_CtrlCMD,
            //3 - 发送初始化指令，包含校准信息
            Tx_InitCMD,
            //待发送的数据的编码最大值，用做安全检查
            TxMaxCode,

            /************  接收组  *************/
            //0 - 接收到错误数据
            Rx_ErrorData = 0x00,
            //1 - 空数据指令
            Rx_EmptyData,
            //2 - 控制指令
            Rx_CtrlData,
            //3 - 普通采样坐标值
            Rx_SampleData,
            //4 - 校准坐标采样值
            Rx_CorrectData,
            //接收到的数据的编码最大值，用做安全检查
            RxMaxCode             

        };
        #endregion

        #region 数据缓冲区及其大小

        //数据头
        public Globle_Datahead Tx_Datahead = new Globle_Datahead();
        //数据尾
        public Globle_Datatail Tx_Datatail = new Globle_Datatail();
        //发送数据内容缓冲区
        public CtrlCMD      Tx_CtrlCMD = new CtrlCMD();
        public InitData     Tx_InitCMD = new InitData();

        private byte EmptyTxBuffer = new byte();

        //接收缓冲区长度的最大值
        private int MaxRxBufferSize = 0;
        //发送缓冲区长度的最大值
        private int MaxTxBufferSize = 0;
        #endregion

        //初始化数据缓冲区及其相关信息
        private void InitBuffers()
        {
            //初始化通用接收缓冲区长度
            MaxRxBufferSize = GetSize(typeof(Globle_Datahead))
                + GetSize(typeof(Globle_Datatail))
                + GetMaxSize(   typeof(CtrlCMD),
                                typeof(SampleData));

            //初始化通用发送缓冲区长度
            MaxTxBufferSize = GetSize(typeof(Globle_Datahead))
                + GetSize(typeof(Globle_Datatail))
                + GetMaxSize(   typeof(CtrlCMD),
                                typeof(InitData));
        }

    }
}
