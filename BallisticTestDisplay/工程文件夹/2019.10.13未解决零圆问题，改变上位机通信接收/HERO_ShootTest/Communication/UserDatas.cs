using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Communication
{
    //意图以后可以在UI上定制通信协议
    public partial class UserDatas : DataTools
    {
        //获取数据头类型
        public Type DataHeadType
        {
            get
            {
                return typeof(Globle_Datahead);
            }
        }
        //获取数据尾类型
        public Type DataTailType
        {
            get
            {
                return typeof(Globle_Datatail);
            }
        }
        //获取数据头大小
        public int DataHeadSize
        {
            get
            {
                return GetSize(typeof(Globle_Datahead));
            }
        }
        //获取数据尾大小
        public int DataTailSize
        {
            get
            {
                return GetSize(typeof(Globle_Datatail));
            }
        }
        //获取发送缓冲区总长
        public int RxBufferSize
        {
            get
            {
                return MaxRxBufferSize;
            }
        }
        //获取发送缓冲区总长
        public int TxBufferSize
        {
            get
            {
                return MaxTxBufferSize;
            }
        }
        //获取发送内容缓冲区长度
        public int TxDataContentSize
        {
            get
            {
                return (MaxTxBufferSize - DataHeadSize - DataTailSize);
            }
        }
        //获取接收内容缓冲区长度
        public int RxDataContentSize
        {
            get
            {
                return (MaxRxBufferSize - DataHeadSize - DataTailSize);
            }
        }

        //数据处理方法委托类型
        public delegate byte DealFunc(byte[] Buffer);

        //委托数组，用于处理接收到的数据
        public DealFunc[] DataDealFuncs;

        #region 处理数据的函数

        #region 校验相关

        //获取数据头缓冲区中的数据
        //起始位置指定：数据头数据在数据缓冲区中的位置
        public Globle_Datahead GetHeadData(byte[] Buffer, int StartIndex)
        {
            if ((StartIndex + GetSize(typeof(Globle_Datahead)) > Buffer.Length))
            {
                throw new Exception("<获取数据头信息>:输入参数非法！");
            }

            return (Globle_Datahead)BytesToStruct(Buffer, StartIndex, DataHeadType);
        }

        //获取数据尾缓冲区中的数据
        //起始位置指定：数据尾数据在数据缓冲区中的位置
        public Globle_Datatail GetTailData(byte[] Buffer, int StartIndex)
        {
            if ((StartIndex + GetSize(typeof(Globle_Datatail)) > Buffer.Length))
            {
                throw new Exception("<获取数据尾信息>:输入参数非法！");
            }

            return (Globle_Datatail)BytesToStruct(Buffer, StartIndex, DataTailType);
        }

        //校验接收到的数据
        public bool CheckRxData(byte[] Bytes)
        {
            uint check = 0;

            //获取数据尾数据
            Globle_Datatail tempTail = GetTailData(Bytes, 16 - DataTailSize);

            //获取校验值
            check = GetCheckValue(Bytes, 0, (uint)(16 - DataTailSize));

            //若不满足校验
            if (check != tempTail.verify)
                return false;

            return true;
        }

        //对数据头进行校验，是决定一组数据是不是数据头的关键判据
        //此处没有安全检查，因为它只在数据校验环节被调用了，可以确保其安全性。
        public bool CheckDataHead(byte[] Bytes)
        {
            int HeadVerify = (int)BytesToStruct(Bytes, 4, typeof(int));

            int check = Bytes[0] * Bytes[1] * Bytes[2] * Bytes[3];

            return (check == HeadVerify);
        }
        #endregion

        #region 发送数据处理

        //更新数据头信息
        //参数用于选定本次发送的数据类型和指定反馈到下位机的数据类型
        public void UpdateTxDataHead(byte TxDataID, byte FeedbackID)
        {
            Tx_Datahead.Data_Head1 = UserDatas.Datahead1;
            Tx_Datahead.Data_Head2 = UserDatas.Datahead2;
            Tx_Datahead.CMD_type = TxDataID;
            Tx_Datahead.DataFeedback = FeedbackID;

            Tx_Datahead.HeadVerify = Tx_Datahead.Data_Head1 * Tx_Datahead.Data_Head2
                                   * Tx_Datahead.CMD_type * Tx_Datahead.DataFeedback;
        }

        //由数据类型标签到发送缓冲区的映射
        public object GetDataToSend(byte TxDataID)
        {
            object Msg = null;

            //获取待发送的数据内容
            switch ((MsgTypesID)TxDataID)
            {
                case MsgTypesID.Tx_ErrorData:
                    Msg = EmptyTxBuffer;
                    break;

                case MsgTypesID.Tx_EmptyCMD:
                    Msg = EmptyTxBuffer;
                    break;

                case MsgTypesID.Tx_CtrlCMD:
                    Msg = Tx_CtrlCMD;
                    break;

                case MsgTypesID.Tx_InitCMD:
                    Msg = Tx_InitCMD;
                    break;
                default:
                    Msg = null;
                    break;
            }

            return Msg;
        }
        #endregion

        #region 自定义数据解析

        /**  关于自定义数据解析部分的说明
         * 
         * 自定义数据解析中包含了两种处理方式：
         * 
         * ①状态数据，即在需要时读取即可的数据，
         *  此类数据被直接以公共字段的形式被不断更新；
         * 
         * ②指令/关键信息，即每回传一次都需要及时处理的数据，
         *  此类数据在每次接收到之后会调用相应的处理委托函数，该委托需要用户另外实现；
         * 
         * <note> 关于数据处理委托:
         * 1)委托变量在该类被实例化的时候初始化为null，
         *  因此使用前应该调用其对应的初始化函数对其进行初始化，该操作将在3)中给予示例。
         *  
         * 2)由于数据处理函数是在串口接收线程中被调用，
         *  而数据处理读取和更新的时候往往涉及窗体控件的修改（跨线程调用窗体控件），
         *  因此委托的实现应有特殊写法，该写法将在3)中给予示例。
         * 
         * 3)调用范例：
         * 
         *  ·对于委托(在本类中声明）：
         *   //采样数据处理委托
         *   public delegate void SampleDataHandlerTypedef(SampleData Data);
         *   //委托实体字段
         *   public SampleDataHandlerTypedef SampleDataHandler = null;
         *   //委托初始化函数
         *   public void SampleHandlerInit(SampleDataHandlerTypedef Handler)
         *   {
         *       SampleDataHandler = Handler;
         *   }
         *   
         *  ·初始化操作（在调用方）：
         *   //初始化处理方法
         *   Datas.SampleHandlerInit(new UserDatas.SampleDataHandlerTypedef(DealSampleData));
         *   [其中，“Datas”为本类的一个实例，“DealSampleData”为委托的实现↓]
         *   
         *  ·委托实现（在调用方）：
         *   private void DealSampleData(UserDatas.SampleData Data)
         *   {
         *       if (this.InvokeRequired)
         *       {
         *           //在此使调用方所在的线程重新调用委托
         *           BeginInvoke(Datas.SampleDataHandler, Data);
         *       }
         *       else
         *       {
         *           //在此写入实际修改窗体控件的操作
         *           this.lb_TestKp.Text = Data.Time.ToString();
         *       }
         *   }
         */

        #region 由回调操作产生的委托集合

        //控制请求处理委托
        public delegate void CtrlHandlerTypedef(CtrlCMD Data);
        //委托实体字段
        public CtrlHandlerTypedef CtrlHandler = null;
        //初始化委托
        public void CtrlHandlerInit(CtrlHandlerTypedef Handler)
        {
            CtrlHandler = Handler;
        }

        //采样数据处理委托
        public delegate void SampleDataHandlerTypedef(SampleData Data);
        //委托实体字段
        public SampleDataHandlerTypedef SampleDataHandler = null;
        //初始化委托
        public void SampleHandlerInit(SampleDataHandlerTypedef Handler)
        {
            SampleDataHandler = Handler;
        }

        //校准数据处理委托
        public delegate void CorrectDataHandlerTypedef(SampleData Data);
        //委托实体字段
        public CorrectDataHandlerTypedef CorrectDataHandler = null;
        //初始化委托
        public void CorrectHandlerInit(CorrectDataHandlerTypedef Handler)
        {
            CorrectDataHandler = Handler;
        }
        #endregion

        //处理下位机传上来的异常数据
        private byte GetErrorData(byte[] Buffer)
        {
            //TODO：写入异常处理函数

            return (byte)MsgTypesID.Rx_ErrorData;
        }

        //处理下位机传上来的空数据，此处为无处理。
        private byte GetEmptyData(byte[] Buffer)
        {
            return (byte)MsgTypesID.Rx_EmptyData;
        }

        //处理下位机传上来的控制请求
        private byte GetCtrlData(byte[] Buffer)
        {
            //将数据写入缓冲区
            CtrlCMD Rx_CtrlData = (CtrlCMD)BytesToStruct(Buffer, GetSize(typeof(Globle_Datahead)), typeof(CtrlCMD));

            //调用写入数据的方法
            if (null == CtrlHandler)
            {
                throw new Exception("<调用控制请求处理函数>：未初始化委托！");
            }
            else
            {
                CtrlHandler(Rx_CtrlData);
            }

            return (byte)MsgTypesID.Rx_CtrlData;
        }

        //处理下位机传上来的采样点数据
        private byte GetSampleData(byte[] Buffer)
        {
            //将数据写入缓冲区
            SampleData Rx_SampleData = (SampleData)BytesToStruct(Buffer, GetSize(typeof(Globle_Datahead)), typeof(SampleData));

            //调用写入数据的方法
            if (null == SampleDataHandler)
            {
                throw new Exception("<调用采样数据处理函数>：未初始化采样数据处理委托！");
            }
            else
            {
                SampleDataHandler(Rx_SampleData);
            }

            return (byte)MsgTypesID.Rx_SampleData;
        }

        //处理下位机传上来的校准电压数据
        private byte GetCorrectData(byte[] Buffer)
        {
            //将数据写入缓冲区
            SampleData Rx_CorrectData = (SampleData)BytesToStruct(Buffer, GetSize(typeof(Globle_Datahead)), typeof(SampleData));

            //调用写入数据的方法
            if (null == CorrectDataHandler)
            {
                throw new Exception("<调用校准数据处理函数>：未初始化处理委托！");
            }
            else
            {
                CorrectDataHandler(Rx_CorrectData);
            }
            return (byte)MsgTypesID.Rx_CorrectData;
        }
        #endregion
        #endregion

        //构造函数
        public UserDatas()
        {
            //初始化缓冲区
            InitBuffers();

            DataDealFuncs = new DealFunc[(int)MsgTypesID.RxMaxCode]
            {
                new DealFunc(GetErrorData),     //接收到错误数据
                new DealFunc(GetEmptyData),     //接收到空数据
                new DealFunc(GetCtrlData),     //接收到空数据
                new DealFunc(GetSampleData),    //接收到采样数据
                new DealFunc(GetCorrectData),//接收到基本运动数据
            };

            return;
        }

        ~UserDatas()
        {
            return;
        }
    }
}
