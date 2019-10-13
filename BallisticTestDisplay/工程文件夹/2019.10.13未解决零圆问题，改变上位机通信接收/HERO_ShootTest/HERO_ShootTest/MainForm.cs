using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using Communication;


namespace HERO_ShootTest
{
    public partial class MainForm : Form
    {
        //声明画板引用
        ShootBoard TestBoard;

        //用户数据
        public UserDatas Datas = new UserDatas();

        //通信组件
        InterfaceToSlave COM;

        //用于临时记录发送请求的变量
        byte TempSendID = (byte)UserDatas.MsgTypesID.Tx_EmptyCMD;

        //状态标志
        bool isFullScreen = false;      //全屏
        bool isTouchCorrect = false;    //触摸校准

        //下位机请求
        enum BoardCMD
        {
            Board_Start,    //启动操作，向上位机请求阈值数据
            Ctrl_EnterCorrect,  //进入校准模式的指令
            Ctrl_ExitCorrect,   //离开校准模式的指令

        };

        public MainForm()
        {
            InitializeComponent();

            //实例化通信组件
            COM = new InterfaceToSlave(ref Datas);

            //挂靠用户数据引用
            COM.InitUserDatas(ref Datas);

            //初始化通信组件串口
            COM.InitSerialPort(ref SPort);

            //初始化测试绘图板
            TestBoard = new ShootBoard(ref groupBox1);

            //初始化采样点坐标处理委托
            Datas.SampleHandlerInit(new UserDatas.SampleDataHandlerTypedef(GetShootPoint));

            //初始化校准采样处理委托
            Datas.CorrectHandlerInit(new UserDatas.CorrectDataHandlerTypedef(CorrectScreen));

            //初始化控制请求处理委托
            Datas.CtrlHandlerInit(new UserDatas.CtrlHandlerTypedef(GetCtrlData));

            //测试绘图
            TestBoard.ShowString("Test_ GDI+ World.\nTest... GDI+ Word.", new Point(14, 10));

            //测试清除
            //TestBoard.ClearScreenPartialy(new Point(14, 10), 20, 60);
        }

        //校准坐标
        unsafe private void CorrectScreen(UserDatas.SampleData Data)
        {
            if (this.InvokeRequired)
            {
                //在此使窗体主线程异步地重新调用委托
                BeginInvoke(Datas.CorrectDataHandler, Data);
            }
            else
            {
                //在此写入实际修改窗体控件的操作
                //函数返回校准计数器的值，代表校准进程
                switch (TestBoard.GetTouchCorrectData(Data.SampleFIFO[0], Data.SampleFIFO[1]))
                {
                    //显示下一个色块
                    case ShootBoard.CorrectProcess.Wait_Point1:

                        if(ShowConfirmDialog())
                        {
                            //切换到下一状态
                            TestBoard.TouchCorrectDelayComplet();
                            TestBoard.CleanScreen();
                            TestBoard.DrawCorrectBlock(1);
                        }
                        
                        break;

                    //结束校准，清屏、给出提示并恢复按钮未选中状态
                    case ShootBoard.CorrectProcess.Wait_Point2:

                        TestBoard.CleanScreen();

                        MessageBox.Show("校准结束！", "提示");

                        TestBoard.TouchCorrectDelayComplet();

                        TSMI_Correct.BackColor = SystemColors.Control;

                        //发送结束校准指令
                        Datas.Tx_CtrlCMD.CMD = (int)BoardCMD.Ctrl_ExitCorrect;

                        //请求发送初始化数据
                        TempSendID = (byte)UserDatas.MsgTypesID.Tx_CtrlCMD;
                        break;

                    default:
                        break;
                }
            }
            
        }

        //显示确认对话框
        bool ShowConfirmDialog()
        {
            //默认情况下，如MessageBox.Show("下一步？")只显示一个“确定”按钮。
            DialogResult dr = MessageBox.Show("下一步?", "校准提示", MessageBoxButtons.OK);

            //如果点击“确定”按钮
            return (dr == DialogResult.OK);
        }

        //获取击中坐标
        unsafe private void GetShootPoint(UserDatas.SampleData Data)
        {
            if (this.InvokeRequired)
            {
                //在此使窗体主线程异步地重新调用委托
                BeginInvoke(Datas.SampleDataHandler, Data);
            }
            else
            {
                int Cnt = 0;
                //在此写入实际修改窗体控件的操作
                //保存击中点
                for(Cnt = 0; Cnt < Data.DataSize; Cnt++)
                {
                    //第一个数据代表X坐标，第二个数据代表Y坐标
                    TestBoard.GetShootPoint(Data.SampleFIFO[Cnt * 2 + 0], Data.SampleFIFO[Cnt * 2 + 1]);
                }
            }
        }

        //获取控制请求
        private void GetCtrlData(UserDatas.CtrlCMD Data)
        {
            if (this.InvokeRequired)
            {
                //在此使窗体主线程异步地重新调用委托
                BeginInvoke(Datas.CtrlHandler, Data);
            }
            else
            {
                //在此写入实际修改窗体控件的操作
                switch ((BoardCMD)Data.CMD)
                {
                    case BoardCMD.Board_Start:
                        //写入阈值电压并发出
                        Datas.Tx_InitCMD.X_Max_Voltage = TestBoard.MaxVoletage_X;
                        Datas.Tx_InitCMD.X_Min_Voltage = TestBoard.MinVoletage_X;
                        Datas.Tx_InitCMD.Y_Max_Voltage = TestBoard.MaxVoletage_Y;
                        Datas.Tx_InitCMD.Y_Min_Voltage = TestBoard.MinVoletage_Y;
                        Datas.Tx_InitCMD.X_TrigValue = TestBoard.TrigValue_X;
                        Datas.Tx_InitCMD.Y_TrigValue = TestBoard.TrigValue_Y;

                        //请求发送初始化数据
                        TempSendID = (byte)UserDatas.MsgTypesID.Tx_InitCMD;
                        break;

                    default: break;
                }
            }
        }

        //鼠标进入“连接下位机”时
        private void TSMI_COM_MouseEnter(object sender, EventArgs e)
        {
            //如果串口被打开，不允许刷新列表
            if (!COM.State_Series.isopen)
            {
                ToolStripMenuItem tsmi = (ToolStripMenuItem)sender;

                tsmi.DropDownItems.Clear();                 //清除原有项

                //检查是否含有串口
                string[] str = System.IO.Ports.SerialPort.GetPortNames();

                if (str.Length == 0)
                {
                    ToolStripMenuItem NewItem = new ToolStripMenuItem("(无可用串口)");

                    tsmi.DropDownItems.Add(NewItem);
                    return;
                }

                //添加串口项目并注册点击事件
                foreach (string s in System.IO.Ports.SerialPort.GetPortNames())
                {
                    ToolStripMenuItem NewItem = new ToolStripMenuItem(s, null, TSMI_COM_Click);

                    tsmi.DropDownItems.Add(NewItem);      //添加现有项
                }
            }
            else
            {
                ToolStripMenuItem tsmi = (ToolStripMenuItem)sender;

                tsmi.DropDownItems.Clear();                 //清除原有项
            }
        }

        //点击连接下位机选项，用于关闭串口或重新召出选项菜单
        private void TSMI_COM_Click_1(object sender, EventArgs e)
        {
            //如果串口被打开了，尝试关闭串口
            if (COM.State_Series.isopen)
            {
                try
                {
                    COM.CloseSerialPort();

                    //取消选中
                    TSMI_COM.BackColor = SystemColors.Control;
                    TSMI_COM.Text = "连接下位机";

                    //清除链接标志
                    foreach (ToolStripMenuItem tsmi in TSMI_COM.DropDownItems)
                    {
                        tsmi.Checked = false;
                    }
                }
                catch (System.Exception ex)
                {
                    MessageBox.Show(ex.Message);

                    return;
                }
            }
            else
            {
                ToolStripMenuItem tsmi = (ToolStripMenuItem)sender;

                tsmi.DropDownItems.Clear();                 //清除原有项

                //检查是否含有串口
                string[] str = System.IO.Ports.SerialPort.GetPortNames();

                if (str.Length == 0)
                {
                    ToolStripMenuItem NewItem = new ToolStripMenuItem("(无可用串口)");

                    tsmi.DropDownItems.Add(NewItem);
                    return;
                }

                //添加串口项目并注册点击事件
                foreach (string s in System.IO.Ports.SerialPort.GetPortNames())
                {
                    ToolStripMenuItem NewItem = new ToolStripMenuItem(s, null, TSMI_COM_Click);

                    tsmi.DropDownItems.Add(NewItem);      //添加现有项
                }
            }
        }

        //点击串口号之后的操作
        private void TSMI_COM_Click(object sender, EventArgs e)
        {
            //如果串口没有被打开
            if (!COM.State_Series.isopen)
            {
                try
                {
                    //打开串口
                    COM.OpenSerialPort(((ToolStripMenuItem)sender).Text);

                    //标记选中的串口
                    ((ToolStripMenuItem)sender).Checked = true;

                    //标记串口已链接
                    TSMI_COM.BackColor = SystemColors.ControlDark;
                    TSMI_COM.Text = ("下位机" + "(" + ((ToolStripMenuItem)sender).Text + ")");
                }
                catch (System.Exception ex)
                {
                    MessageBox.Show("Error:" + ex.Message, "Error");

                    ((ToolStripMenuItem)sender).Checked = false;

                    return;
                }
            }
            else
            {
                try
                {
                    COM.CloseSerialPort();

                    //取消选中
                    TSMI_COM.BackColor = SystemColors.Control;
                    TSMI_COM.Text = "连接下位机";
                }
                catch (System.Exception ex)
                {
                    MessageBox.Show(ex.Message);

                    return;
                }

            }
        }

        //帮助菜单
        private void TSMI_SysScrnData_Click(object sender, EventArgs e)
        {
            TestBoard.ShowSystemScrrenPrams();
        }

        //屏幕设置
        private void TSMI_ScreenSettings_Click(object sender, EventArgs e)
        {
            Screen_Settings cfg = new Screen_Settings(ref this.TestBoard);

            cfg.ShowDialog();
        }

        //通信设置
        private void TSMI_ComSet_Click(object sender, EventArgs e)
        {
            ComSettings cfg = new ComSettings(ref this.COM);

            cfg.ShowDialog();
        }

        //点击“触点校准”时
        private void TSMI_Correct_Click(object sender, EventArgs e)
        {
            //检查校准状态并启动校准流程
            if (!isTouchCorrect)
            {
                TestBoard.StartTouchCorrect();

                MessageBox.Show("接下来将在屏幕两角依次显示色块，请依次点击色块以完成校准操作", "提示信息");

                //标记选中
                ((ToolStripMenuItem)sender).BackColor = SystemColors.ButtonShadow;

                //发送校准指令
                Datas.Tx_CtrlCMD.CMD = (int)BoardCMD.Ctrl_EnterCorrect;

                //请求发送初始化数据
                TempSendID = (byte)UserDatas.MsgTypesID.Tx_InitCMD;

                //显示第一个校准色块
                TestBoard.DrawCorrectBlock(0);

                isTouchCorrect = true;
            }
            else
            {
                MessageBox.Show("请先完成本轮校准操作！", "Wanning!");
            }
        }

        //重绘事件，以防止拖动框出显示范围时出现图片残缺的情况
        private void groupBox1_Paint(object sender, PaintEventArgs e)
        {
            TestBoard.RePiantGraphics();
        }

        UInt64 TestCnt = 0;
        //定时请求发送并更新文本
        private void timer_Tick(object sender, EventArgs e)
        {
            //更新频率计数器
            lb_TestCnt.Text = TestCnt.ToString();

            //显示捕获点数量
            lb_CaptureCnt.Text = TestBoard.ShootPointCount.ToString() + ".";

            if (COM.State_Series.isopen)
            {
                //2ms请求一次发送
                try
                {
                    COM.SendData(TempSendID);
                    TestCnt++;
                }
                catch(Exception ex)
                {
                    //终止连接
                    TSMI_COM_Click_1(null, null);
                    MessageBox.Show(ex.Message + " 已断开链接。", "出错了！");
                }

                //如果本条消息顺利地发出去了，就将发送请求清空为空指令
                if (0 == COM.RequestErrorCounter)
                {
                    TempSendID = (byte)UserDatas.MsgTypesID.Tx_EmptyCMD;

                    //标记通信恢复正常
                    TestBoard.ShowCOM_State(true);
                }
                else if (COM.RequestErrorCounter > 25)
                {
                    //如果连续50ms都请求失败了，那么开始请求重连
                    try
                    {
                        COM.ReConnectSlave();
                    }
                    catch (Exception ex)
                    {
                        //终止连接
                        TSMI_COM_Click_1(null, null);
                        MessageBox.Show(ex.Message + " 已断开链接", "出错了！");
                    }
                    //清空计数器
                    COM.RequestErrorCounter = 0;

                    //标记通信异常
                    TestBoard.ShowCOM_State(false);
                }
            }

            string Temp = COM.InfoStrBuffer;

            if ("" != Temp)
            {
                rTB_Info.Text += Temp;

                rTB_Info.SelectAll();
                rTB_Info.SelectionColor = Color.Blue;         //改变字体的颜色

                rTB_Info.SelectionStart = this.rTB_Info.TextLength;
                rTB_Info.ScrollToCaret();
            }
        }

        //点击清屏选项
        private void TSMI_Clear_Click(object sender, EventArgs e)
        {
            TestBoard.CleanScreen();
            TestBoard.StartCollectPoints();
            //测试绘图
            TestBoard.ShowString("Test_ GDI+ World.\nTest... GDI+ Word.", new Point(14, 10));

        }

        //点击清空文本
        private void btn__CleanText_Click(object sender, EventArgs e)
        {
            rTB_Info.Text = "";
        }

        //全屏显示按钮
        private void TSMI_FullScreen_Click(object sender, EventArgs e)
        {
            //将窗体最大化
            this.WindowState = FormWindowState.Maximized;

            //设置窗体为无边框样式
            this.FormBorderStyle = FormBorderStyle.None;

            //启用键盘控制，因为要使用ESC键离开全屏模式
            this.KeyPreview = true;

            //隐藏菜单栏
            menuStrip.Visible = false;

            //隐藏通知栏
            groupBox2.Visible = false;

            //隐藏测试栏
            groupBox3.Visible = false;

            //将显示区域铺满整个区域
            groupBox1.Location = new Point(8, 8);
            groupBox1.Width = this.Width - 32;
            groupBox1.Height = this.Height - 55;

            isFullScreen = true;

        }

        //当大小发生变化时
        private void groupBox1_SizeChanged(object sender, EventArgs e)
        {
            //重新初始化画板
            TestBoard = new ShootBoard(ref groupBox1);
        }

        //测试输入100个屏幕范围内的随机点
        private unsafe void btn_InputData_Click(object sender, EventArgs e)
        {
            //随机数生成器
            Random rand = new Random();

            //中间变量
            UserDatas.SampleData TempVoltage = new UserDatas.SampleData();

            //随机数生成范围
            double[] ShowBoundary = TestBoard.ShootVoltageRange;

            double Range_X = (ShowBoundary[1] - ShowBoundary[0])*0.55;
            double Range_Y = (ShowBoundary[3] - ShowBoundary[2])*0.55;

            for(int i = 0; i < 100; i++)
            {
                //NextDouble()生成0~1之间的随机数
                TempVoltage.SampleFIFO[0] = (ushort)(250 + ShowBoundary[0] + Range_X * rand.NextDouble());
                TempVoltage.SampleFIFO[1] = (ushort)(700 + ShowBoundary[1] + Range_Y * rand.NextDouble());

                ////按x线性规则生成点集
                //TempVoltage.SampleFIFO[0] = (ushort)(ShowBoundary[0] + Range_X * i*0.02);
                //TempVoltage.SampleFIFO[1] = (ushort)(ShowBoundary[1] + Range_Y * i*0.02);

                TempVoltage.DataSize = 1;

                GetShootPoint(TempVoltage);
            }            
        }
        
        //分析测试结果按钮
        private void btn_AnalyseData_Click(object sender, EventArgs e)
        {
            //求出最小包络圆
            string Str = TestBoard.EndCollectPoints();

            rTB_Info.Text += Str;

            rTB_Info.SelectAll();
            rTB_Info.SelectionColor = Color.Blue;         //改变字体的颜色

            rTB_Info.SelectionStart = this.rTB_Info.TextLength;
            rTB_Info.ScrollToCaret();
        }

        //窗体第一次显示时发生
        private void MainForm_Shown(object sender, EventArgs e)
        {
            //初始即进入全屏状态
            TSMI_FullScreen_Click(null, null);

        }

        //当按下按键时。
        //考虑不会同时有多组按键同时按下（除非疯了），因此只做一对互斥锁（防止动作重复触发）。
        bool isKeyDown = false;
        int RecordCnt = 0;
        private void MainForm_KeyDown(object sender, KeyEventArgs e)
        {
            if(!isKeyDown)
            {
                //Escape：退出触摸校准/全屏模式
                if (Keys.Escape == e.KeyCode)
                {
                    //优先取消触摸校准
                    if (isTouchCorrect)
                    {
                        //中止校准进程
                        TestBoard.AbortTouchCorrect();

                        MessageBox.Show("校准进程已终止,本次采集数据未生效！", "提示");

                        TestBoard.CleanScreen();

                        TSMI_Correct.BackColor = SystemColors.Control;

                        //发送结束校准指令
                        Datas.Tx_CtrlCMD.CMD = (int)BoardCMD.Ctrl_ExitCorrect;

                        isTouchCorrect = false;

                        isKeyDown = true;
                    }
                    else if (isFullScreen)
                    {
                        //恢复正常大小
                        this.WindowState = FormWindowState.Normal;

                        //恢复边框样式
                        this.FormBorderStyle = FormBorderStyle.FixedDialog;

                        //显示菜单栏
                        menuStrip.Visible = true;

                        //复位菜单栏位置
                        menuStrip.Location = new Point(0, 0);

                        //显示通知栏
                        groupBox2.Visible = true;

                        //显示测试栏
                        groupBox3.Visible = true;

                        //复位显示区域
                        groupBox1.Location = new Point(14, 35);
                        groupBox1.Width = 864;
                        groupBox1.Height = 540;

                        isFullScreen = false;

                        isKeyDown = true;
                    }
                }

                //Enter:结束采样并分析结果/重新开始采样
                if (Keys.Enter == e.KeyCode)
                {
                    switch (RecordCnt)
                    {
                        case 0: //结束采样，进行分析

                            btn_AnalyseData_Click(null, null);

                            if (TestBoard.ShootPointCount == 0)
                            {
                                RecordCnt = 0;
                            }
                            else
                            {
                                RecordCnt++;
                            }
                            break;

                        case 1: //清空记录，重新开始

                            TSMI_Clear_Click(null, null);
                            RecordCnt = 0;
                            break;

                        default: break;
                    }

                    isKeyDown = true;
                }

                //空格键：清空界面和采样点记录
                if (Keys.Space == e.KeyCode)
                {
                    TSMI_Clear_Click(null, null);

                    RecordCnt = 0;

                    isKeyDown = true;
                }

                //全屏下的功能键
                if (isFullScreen)
                {
                    //按下Ctrl + C,召唤屏幕参数框
                    if (e.Modifiers == Keys.Control && e.KeyCode == Keys.C)
                    {
                        TSMI_ScreenSettings_Click(null, null);

                        isKeyDown = true;
                    }

                    //按下Ctrl + T,测试输入100个随机点
                    if (e.Modifiers == Keys.Control && e.KeyCode == Keys.T)
                    {
                        btn_InputData_Click(null, null);

                        isKeyDown = true;
                    }

                    //按下Ctrl + H,开始触摸校准
                    if (e.Modifiers == Keys.Control && e.KeyCode == Keys.H)
                    {
                        TSMI_Correct_Click(TSMI_Correct, null);

                        isKeyDown = true;
                    }

                    //按下Ctrl + M,召唤菜单栏
                    if (e.Modifiers == Keys.Control && e.KeyCode == Keys.M)
                    {
                        
                        if (menuStrip.Visible)
                        {
                            //复位菜单栏位置
                            menuStrip.Location = new Point(0, 0);

                            //将显示区域铺满整个区域
                            groupBox1.Location = new Point(8, 8);
                            groupBox1.Width = this.Width - 32;
                            groupBox1.Height = this.Height - 55;
                        }
                        else
                        {
                            //将菜单栏位置移动到合适位置
                             menuStrip.Location = new Point(8, 8);

                            //复位显示区域
                            groupBox1.Location = new Point(14, 44);
                            groupBox1.Width = this.Width - 32;
                            groupBox1.Height = this.Height - 55;
                        }

                        //改变菜单栏显示状态
                        menuStrip.Visible = !menuStrip.Visible;

                        isKeyDown = true;
                    }
                }
                else //非全屏下的功能键
                {
                    //按下Ctrl + A,进入全屏
                    if (e.Modifiers == Keys.Control && e.KeyCode == Keys.A)
                    {
                        TSMI_FullScreen_Click(null, null);

                        isKeyDown = true;
                    }
                }
            }
        }

        //按键弹起，释放互斥锁。
        private void MainForm_KeyUp(object sender, KeyEventArgs e)
        {
            isKeyDown = false;
        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void lb_TestCnt_Click(object sender, EventArgs e)
        {

        }

        private void groupBox1_Enter(object sender, EventArgs e)
        {

        }
    }
}
