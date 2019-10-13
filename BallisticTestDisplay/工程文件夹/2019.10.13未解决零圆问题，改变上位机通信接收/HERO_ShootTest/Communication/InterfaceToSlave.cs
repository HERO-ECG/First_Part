using System;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using System.Threading;
using System.IO.Ports;

namespace Communication
{
    //实现通信协议
    public class InterfaceToSlave : ComEntity  //继承通信实体类
    {
        #region 通用组件

        //用户数据
        public UserDatas Datas;

        //数据接收缓冲区
        private byte[] Rx_Buffer;
        private int BufferIndex = 0;

        //数据接收计数器
        private UInt64 DataCounter_Byte = 0;
        private UInt64 DataCounter_Pack = 0;
        private UInt64 DataCounter_ErrorPack = 0;

        //接收到的字符数据缓冲区
        private string ReceiveStrBuffer = "";
        private int SrtBufferLength = 20000; //文本缓冲区的大小

        //标志
        private bool isShowInHex = true;
        private bool isTimeValid = true;
        private bool isRandomDataValid = false;

        //状态属性：接收到的包数量
        public UInt64 PackReceived
        {
            get
            {
                return DataCounter_Pack;
            }
            set
            {
                DataCounter_Pack = value;
            }
        }

        //状态属性：接收到的字节数
        public UInt64 ByteReceived
        {
            get
            {
                return DataCounter_Byte;
            }
            set
            {
                DataCounter_Byte = value;
            }
        }

        //状态属性：接收到的错误包数量
        public UInt64 ErrorPackReceived
        {
            get
            {
                return DataCounter_ErrorPack;
            }
            set
            {
                DataCounter_ErrorPack = value;
            }
        }

        //设置属性：接收字符串数据缓冲区大小，单位为字符个数
        public int RcvBufferSize
        {
            get
            {
                return SrtBufferLength;
            }
            set
            {
                SrtBufferLength = value;
            }
        }

        //设置属性：是否十六进制解析非格式化数据
        public bool IsShowInhex
        {
            get
            {
                return isShowInHex;
            }
            set
            {
                isShowInHex = value;
            }
        }

        //设置属性：是否在每个读取间隔加上时间标记
        public bool IsTimeMarkValid
        {
            get
            {
                return isTimeValid;
            }
            set
            {
                isTimeValid = value;
            }
        }

        //设置属性：接收到的非格式化数据是否有效
        public bool IsRandomDataValid
        {
            get
            {
                return isRandomDataValid;
            }
            set
            {
                isRandomDataValid = value;
            }
        }

        //内容属性：文本缓冲区(信息接收时，用于存储非致命通知的缓冲区,读取后自动清空)
        public string RcvStringBuffer
        {
            get
            {
                string temp = ReceiveStrBuffer;

                ReceiveStrBuffer = "";

                return temp;
            }
        }

        //动作属性:获取文本缓冲区的内容（受保护）
        protected string DataStrBuffer_Read
        {
            get
            {
                return ReceiveStrBuffer;
            }
        }

        //构造函数
        public InterfaceToSlave(ref UserDatas Data)
        {
            //填充引用
            Datas = Data;

            //加载通信配置
            LoadSettings();
        }

        //析构函数
        ~InterfaceToSlave()
        {
            //保存通信配置
            SaveSettings();
        }

        //实例化用户数据
        public void InitUserDatas(ref UserDatas UserData)
        {
            Datas = UserData;

            //由于类在构造时先构造成员类，因此此处的用法是安全的
            //即Datas的RxBufferSize此时已经被初始化完成了，不是0值。
            Rx_Buffer = new byte[Datas.RxBufferSize];

            //允许上位机先发数据
            COM_log.isCouldSendMsg = true;
        }

        //将数据写入字符串数据缓冲区
        protected void WriteDataInBuffer(string Str)
        {
            if (ReceiveStrBuffer.Length <= SrtBufferLength)
            {
                ReceiveStrBuffer += Str;
            }
            else
            {
                if (this.isRandomDataValid)
                {
                    throw new Exception("<文本缓冲区>:保存的字符已经达到规定的上限！");
                }
                else
                {
                    ReceiveStrBuffer = "";
                }
            }
        }

        //加载设置
        public void LoadSettings()
        {
            //加载通信设置         
            this.SP_Baudrate = DataTools.ReadIni("BAUDRATE", "baudrate", "9600");
            this.SP_DataBits = DataTools.ReadIni("DATABITS", "databits", "8");
            this.SP_StopBits = DataTools.ReadIni("STOP", "stop", "1");
            this.SP_Parity = DataTools.ReadIni("PARITY", "parity", "NONE");
        }

        //保存设置
        public void SaveSettings()
        {
            DataTools.WriteIni("BAUDRATE", "baudrate", this.SP_Baudrate);
            DataTools.WriteIni("DATABITS", "databits", this.SP_DataBits);
            DataTools.WriteIni("STOP", "stop", this.SP_StopBits);
            DataTools.WriteIni("PARITY", "parity", this.SP_Parity);
        }
        #endregion

        #region 具体通信协议实现相关

        #region 模拟半双工机制

        //发送状态记录器
        HDState COM_log = new HDState();

        //[发送请求]该属性写入任何值都会清零
        public UInt64 RequestErrorCounter
        {
            get
            {
                return COM_log.RequestErrorCnt;
            }
            set
            {
                COM_log.RequestErrorCnt = 0;
            }
        }

        //重置发送标志以重新激活发送动作
        public void ReConnectSlave()
        {
            COM_log.HaveRcvdID = (byte)UserDatas.MsgTypesID.Tx_ErrorData;
            COM_log.isCouldSendMsg = true;

            SendData(0);
        }
        #endregion

        #region 数据处理部分

        //处理待发送的数据
        protected override byte[] DealDataToSend(byte SendID)
        {
            byte[] DataToSend;
            byte TempID = 0x00;

            if(COM_log.isCouldSendMsg)
            {
                DataToSend = new byte[Datas.TxBufferSize];

                //置零数组
                Array.Clear(DataToSend, 0, DataToSend.Length);

                //获得本次要发送的指令
                if(COM_log.isSendSuccessful)
                {
                    TempID = SendID;

                    //发送请求驳回计数器清零
                    COM_log.RequestErrorCnt = 0;
                }
                else
                {
                    TempID = COM_log.HaveSendID;

                    //发送请求驳回计数器自增一
                    COM_log.RequestErrorCnt++;
                }

                COM_log.HaveSendID = TempID;

                //标定数据头
                Datas.UpdateTxDataHead(TempID, COM_log.HaveRcvdID);

                //写入数据头信息
                Datas.StructToBytes(Datas.Tx_Datahead, ref DataToSend, 0);

                //写入数据内容
                Datas.StructToBytes(Datas.GetDataToSend(Datas.Tx_Datahead.CMD_type), ref DataToSend, Datas.DataHeadSize);

                //获取校验值
                Datas.Tx_Datatail.verify = Datas.GetCheckValue(DataToSend, 0, (uint)(Datas.DataHeadSize + Datas.TxDataContentSize));

                //写入数据尾
                Datas.StructToBytes(Datas.Tx_Datatail, ref DataToSend, Datas.DataHeadSize + Datas.TxDataContentSize);

                COM_log.isCouldSendMsg = false;
            }
            else
            {
                //返回空数组
                DataToSend = new byte[0];

                //发送请求驳回计数器自增一
                COM_log.RequestErrorCnt++;
            }
            return DataToSend;
        }

        //接收组件缓冲区中的数据
        protected override void ReceiveData(byte[] DataBuffer)
        {
            //记录接收到的字符数
            DataCounter_Byte += (UInt64)DataBuffer.Length;

            //按照是否是格式化数据的类别区分处理
            foreach(byte Data in DataBuffer)
            {
                //单个字符检索
                GetFormatData(Data);
            }
                        
            //如果使能任意数据接收
            if (isRandomDataValid)
            {
                GetRandomData(DataBuffer);
            }
        }

        //处理格式化数据
        private void GetFormatData(byte Data)
        {
            bool isInvalid = false;    //标识本次接受的数据是否非法

            //在数据头处逐字检查以确保数据头被合理捕获
            switch(BufferIndex)
            {
                case 0:
                    isInvalid = (Data != UserDatas.Datahead1);
                    break;

                case 1:
                    isInvalid = (Data != UserDatas.Datahead2);
                    break;

                case 2:
                    isInvalid = (Data >= (byte)UserDatas.MsgTypesID.RxMaxCode);
                    break;

                case 3:
                    isInvalid = (Data >= (byte)UserDatas.MsgTypesID.RxMaxCode);
                    break;

                case 7:
                    //如果数据头校验不通过
                    if(!Datas.CheckDataHead(Rx_Buffer))
                    {
                        Rx_Buffer[BufferIndex] = Data;

                        BufferIndex = 0;

                        //检查中间接收的第4.5.6.7个字节是否隐藏着真正的数据头
                        for(int i = 4; i < 8; i++)
                        {
                            GetFormatData(Rx_Buffer[i]);
                        }
                        return;
                    }
                    break;

                default:break;
            }

            //发现是非法数据就舍弃
            if (isInvalid)
            {
                BufferIndex = 0;

                //针对在非法处理中刚好遇到数据头的情况
                if(Data == UserDatas.Datahead1)
                {
                    GetFormatData(Data);
                }

                return;
            }

            //将接收到的数据写入接收缓冲区
            Rx_Buffer[BufferIndex] = Data;

            //更新数组索引
            BufferIndex++;

            //数据包长度符合要求时再做拆包操作
            //至此，数据包以及具备的特征是，长度符合要求，数据头正确。
            if (Datas.RxBufferSize <= BufferIndex)
            {
                try
                {
                    DealFormatReceived(Rx_Buffer);
                }
                catch(Exception ex)
                {
                    throw new Exception(ex.Message);
                }
                BufferIndex = 0;
            }
        }

        //处理任意数据
        private void GetRandomData(byte[] unFrmtData)
        {
            //如果使能了时间戳功能
            if (isTimeValid && DataStrBuffer_Read == "")
            {
                //输出当前时间
                DateTime dt = DateTime.Now;
                WriteDataInBuffer(dt.GetDateTimeFormats('f')[0].ToString() + "\r\n");
            }

            string strRcv = null;

            //解析字符数据
            if (!isShowInHex)
            {
                for (int i = 0; i < unFrmtData.Length; i++)       //窗体显示
                {
                    strRcv += (char)unFrmtData[i];           //按字符格式显示
                }
            }
            else
            {
                for (int i = 0; i < unFrmtData.Length; i++)
                {
                    strRcv += (unFrmtData[i].ToString("X2") + " "); //16进制显示
                }
            }

            WriteDataInBuffer(strRcv);            
        }

        //解析接收到的数据
        private void DealFormatReceived(byte[] Buffer)
        {
            bool iserror = true;

            //获取数据头相应的信息
            UserDatas.Globle_Datahead tempHead = Datas.GetHeadData(Rx_Buffer, 0);

            if(Datas.CheckRxData(Rx_Buffer))
            {
                //判断上次的发送下位机是否成功接收到了
                COM_log.isSendSuccessful = (tempHead.DataFeedback == COM_log.HaveSendID);

                if(tempHead.CMD_type < (byte)UserDatas.MsgTypesID.RxMaxCode)
                {
                    //处理数据并向下位机标识本次接收到的数据类型
                    COM_log.HaveRcvdID = Datas.DataDealFuncs[tempHead.CMD_type](Buffer);

                    //记录正确的包数量
                    DataCounter_Pack++;

                    iserror = false;
                }
            }
            
            //如果数据接收有问题
            if(iserror)
            {
                //输出当前时间
                DateTime dt = DateTime.Now;
                WriteDataInBuffer(dt.GetDateTimeFormats('f')[0].ToString() + "\r\n");

                string strRcv = null;

                //记录错误的包数量
                DataCounter_ErrorPack++;

                //解析字符数据
                for (int i = 0; i < Rx_Buffer.Length; i++)
                {
                    strRcv += (Rx_Buffer[i].ToString("X2") + " "); //16进制显示
                }

                SaveInfo("错误包：" + strRcv + "\r\n");

                //向下位机标识本次接收到的数据类型
                COM_log.isSendSuccessful = false;
                COM_log.HaveRcvdID = (byte)UserDatas.MsgTypesID.Rx_ErrorData;
            }

            //接收到数据包，意味着下位机完成了一次有效发送，进入等待回应的状态
            COM_log.isCouldSendMsg = true;
        }
        #endregion
        #endregion
    }
}