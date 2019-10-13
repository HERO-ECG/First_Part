using System;
using System.Collections.Generic;
using System.Windows;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Management;

using System.Windows.Forms;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;

using Communication;



namespace HERO_ShootTest
{
    //此类以GroupBox为基础进行绘图
    public class ShootBoard
    {
        [System.Runtime.InteropServices.DllImport("User32.dll")]
        //其在MSDN中原型为HDC GetDC(HWND hWnd),HDC和HWND都是驱动器句柄（长指针），
        //在C#中只能用IntPtr代替了
        static extern IntPtr GetDC(IntPtr Hwnd);
        [System.Runtime.InteropServices.DllImport("User32.dll")]
        static extern int ReleaseDC(IntPtr hWnd, IntPtr hDC);

        [System.Runtime.InteropServices.DllImport("User32.dll")]
        private extern static bool SetProcessDPIAware();

        [System.Runtime.InteropServices.DllImport("gdi32.dll")]
        public static extern int GetDeviceCaps(
           IntPtr hdc, // handle to DC
           int nIndex // index of capability
           );

        //保存的引用
        private GroupBox GrpBxRef;

        //GDI+图像缓存，操作此缓冲区可以解决频闪问题
        private BufferedGraphics GrphcBuf;

        //创建存储序列
        List<ShootPoint> ShootPoints;

        //GetDeviceCaps函数的参数，用于指定获取关于显示的什么参数
        //You can use the following enumeration for passing in as nIndex:
        public enum DeviceCap

        {
            /// <summary>
            /// Device driver version
            /// </summary>
            DRIVERVERSION = 0,
            /// <summary>
            /// Device classification
            /// </summary>
            TECHNOLOGY = 2,
            /// <summary>
            /// Horizontal size in millimeters
            /// </summary>
            HORZSIZE = 4,
            /// <summary>
            /// Vertical size in millimeters
            /// </summary>
            VERTSIZE = 6,
            /// <summary>
            /// Horizontal width in pixels
            /// </summary>
            HORZRES = 8,
            /// <summary>
            /// Vertical height in pixels
            /// </summary>
            VERTRES = 10,
            /// <summary>
            /// Number of bits per pixel
            /// </summary>
            BITSPIXEL = 12,
            /// <summary>
            /// Number of planes
            /// </summary>
            PLANES = 14,
            /// <summary>
            /// Number of brushes the device has
            /// </summary>
            NUMBRUSHES = 16,
            /// <summary>
            /// Number of pens the device has
            /// </summary>
            NUMPENS = 18,
            /// <summary>
            /// Number of markers the device has
            /// </summary>
            NUMMARKERS = 20,
            /// <summary>
            /// Number of fonts the device has
            /// </summary>
            NUMFONTS = 22,
            /// <summary>
            /// Number of colors the device supports
            /// </summary>
            NUMCOLORS = 24,
            /// <summary>
            /// Size required for device descriptor
            /// </summary>
            PDEVICESIZE = 26,
            /// <summary>
            /// Curve capabilities
            /// </summary>
            CURVECAPS = 28,
            /// <summary>
            /// Line capabilities
            /// </summary>
            LINECAPS = 30,
            /// <summary>
            /// Polygonal capabilities
            /// </summary>
            POLYGONALCAPS = 32,
            /// <summary>
            /// Text capabilities
            /// </summary>
            TEXTCAPS = 34,
            /// <summary>
            /// Clipping capabilities
            /// </summary>
            CLIPCAPS = 36,
            /// <summary>
            /// Bitblt capabilities
            /// </summary>
            RASTERCAPS = 38,
            /// <summary>
            /// Length of the X leg
            /// </summary>
            ASPECTX = 40,
            /// <summary>
            /// Length of the Y leg
            /// </summary>
            ASPECTY = 42,
            /// <summary>
            /// Length of the hypotenuse
            /// </summary>
            ASPECTXY = 44,
            /// <summary>
            /// Shading and Blending caps
            /// </summary>
            SHADEBLENDCAPS = 45,

            /// <summary>
            /// Logical pixels inch in X
            /// </summary>
            LOGPIXELSX = 88,
            /// <summary>
            /// Logical pixels inch in Y
            /// </summary>
            LOGPIXELSY = 90,

            /// <summary>
            /// Number of entries in physical palette
            /// </summary>
            SIZEPALETTE = 104,
            /// <summary>
            /// Number of reserved entries in palette
            /// </summary>
            NUMRESERVED = 106,
            /// <summary>
            /// Actual color resolution
            /// </summary>
            COLORRES = 108,

            // Printing related DeviceCaps. These replace the appropriate Escapes
            /// <summary>
            /// Physical Width in device units
            /// </summary>
            PHYSICALWIDTH = 110,
            /// <summary>
            /// Physical Height in device units
            /// </summary>
            PHYSICALHEIGHT = 111,
            /// <summary>
            /// Physical Printable Area x margin
            /// </summary>
            PHYSICALOFFSETX = 112,
            /// <summary>
            /// Physical Printable Area y margin
            /// </summary>
            PHYSICALOFFSETY = 113,
            /// <summary>
            /// Scaling factor x
            /// </summary>
            SCALINGFACTORX = 114,
            /// <summary>
            /// Scaling factor y
            /// </summary>
            SCALINGFACTORY = 115,

            /// <summary>
            /// Current vertical refresh rate of the display device (for displays only) in Hz
            /// </summary>
            VREFRESH = 116,
            /// <summary>
            /// Horizontal width of entire desktop in pixels
            /// </summary>
            DESKTOPVERTRES = 117,
            /// <summary>
            /// Vertical height of entire desktop in pixels
            /// </summary>
            DESKTOPHORZRES = 118,
            /// <summary>
            /// Preferred blt alignment
            /// </summary>
            BLTALIGNMENT = 119
        }

        //校准流程
        public enum CorrectProcess
        {
            Start_Point1 = 0x00, //开始校准左上角
            Wait_Point1,         //等待左上角校准结束
            Start_Point2,        //开始校准右下角
            Wait_Point2,         //等待右下角校准结束
            End                  //校准结束
        }

        //颜色表枚举类型，配合色表使用，按顺序一一对应
        private enum Colors
        {
            Red = 0,
            Green,
            Blue,
            Brown,
            Gray,
            ColorsNum
        }
        //颜色表，配合色表枚举使用，按顺序一一对应
        private static int[] MyColors = new int[(int)Colors.ColorsNum]
        {
            (255 << 24) + (244 << 16) + (96 << 8) + 108 , //好看红
            (255 << 24) + (25 << 16) + (202 << 8) + 173 , //好看绿
            (255 << 24) + (190 << 16) + (231 << 8) + 233, //好看蓝
            (255 << 24) + (209 << 16) + (186 << 8) + 116, //好看棕
            (255 << 24) + (214 << 16) + (213 << 8) + 183, //好看灰
        };

        //屏幕物理长度信息
        double ScreenInch = 1;              //屏幕对角线长，单位为英寸
        double ScreenLengthWidthRatio = 1;  //屏幕长宽比（长：宽）
        const double mmInchRatio = 25.4;    //毫米英尺换算比(毫米：英寸)

        //击中点图标大小
        const int PointLength = 30;
        int CenterSize;

        //触发捕获电压采样值
        public ushort TrigValue_X;
        public ushort TrigValue_Y;

        //电压校准值
        public ushort MaxVoletage_X = 0;
        public ushort MinVoletage_X = 0;
        public ushort MaxVoletage_Y = 0;
        public ushort MinVoletage_Y = 0;

        //校准计数器
        CorrectProcess CorrectCnt = CorrectProcess.End;

        //映射关系：像素点与电压值的比值,线性映射关系
        double Point_VolRate_X = 1;
        double X_Offset = 0;

        double Point_VolRate_Y = 1;
        double Y_Offset = 0;

        //映射关系：毫米与像素点的比值,类初始化时将该比例初始化
        double mm_PixelRate_X = 1;
        double mm_PixelRate_Y = 1;

        //校准块位置偏移
        int StartOffset = 50;

        //测得的校准块对应采样值
        ushort HighVoletage_X = 0;
        ushort LowVoletage_X = 0;
        ushort HighVoletage_Y = 0;
        ushort LowVoletage_Y = 0;

        //字体大小
        const int InfoFontSize = 16;
        //显示通知的内容
        string InfoMsg = "";
        //记录显示字体的初始位置
        Point ShowFontLocation = new Point();

        //预置颜色·好看红
        public static Color Red
        {
            get
            {
                return Color.FromArgb(MyColors[(int)Colors.Red]);
            }
        }

        //预置颜色·好看绿
        public static Color Green
        {
            get
            {
                return Color.FromArgb(MyColors[(int)Colors.Green]);
            }
        }

        //预置颜色·好看蓝
        public static Color Blue
        {
            get
            {
                return Color.FromArgb(MyColors[(int)Colors.Blue]);
            }
        }

        //预置颜色·好看棕
        public static Color Brown
        {
            get
            {
                return Color.FromArgb(MyColors[(int)Colors.Brown]);
            }
        }

        //预置颜色·好看灰
        public static Color Gray
        {
            get
            {
                return Color.FromArgb(MyColors[(int)Colors.Gray]);
            }
        }

        /// <summary>
        /// 获取宽度缩放百分比 （**当获取的DPI的值一直是96的时候，可以通过用此方法获取的值转化为DPI，ScaleX * 96**）
        /// </summary>
        public static float ScaleX
        {
            get
            {
                IntPtr hdc = GetDC(IntPtr.Zero);
                int t = GetDeviceCaps(hdc, (int)DeviceCap.DESKTOPHORZRES);
                int d = GetDeviceCaps(hdc, (int)DeviceCap.HORZRES);
                float ScaleX = (float)GetDeviceCaps(hdc, (int)DeviceCap.DESKTOPHORZRES) / (float)GetDeviceCaps(hdc, (int)DeviceCap.HORZRES);
                ReleaseDC(IntPtr.Zero, hdc);
                return ScaleX;
            }
        }

        /// <summary>
        /// 获取高度缩放百分比
        /// </summary>
        public static float ScaleY
        {
            get
            {
                IntPtr hdc = GetDC(IntPtr.Zero);
                float ScaleY = (float)(float)GetDeviceCaps(hdc, (int)DeviceCap.DESKTOPVERTRES) / (float)GetDeviceCaps(hdc, (int)DeviceCap.VERTRES);
                ReleaseDC(IntPtr.Zero, hdc);
                return ScaleY;
            }
        }

        //获取当前显示区能容纳的击中点输入电压采样值范围(0:X小；1:X大，2:Y小；3:Y大)
        public double[] ShootVoltageRange
        {
            get
            {
                double[] Range = new double[4];

                //获得显示范围
                int Height = GrpBxRef.Height;
                int Width = GrpBxRef.Width;

                //计算X范围
                Range[0] = (0 - X_Offset) / Point_VolRate_X;
                Range[1] = (Width - X_Offset) / Point_VolRate_X;

                //计算Y范围
                Range[2] = (0 - Y_Offset) / Point_VolRate_Y;
                Range[3] = (Height - Y_Offset) / Point_VolRate_Y;

                return Range;
            }
        }

        //获取当前已经记录的点个数
        public int ShootPointCount
        {
            get
            {
                return ShootPoints.Count;
            }
        }
        
        //构造函数
        public ShootBoard(ref GroupBox GrpBx)
        {
            GrpBxRef = GrpBx;

            //创建图像并保存其引用
            GrphcBuf = (new BufferedGraphicsContext()).Allocate(GrpBx.CreateGraphics(), GrpBx.ClientRectangle);

            //加载校准值
            LoadCorrectData();

            //加载屏幕信息
            LoadScreenData();

            //清屏
            GrphcBuf.Graphics.Clear(Gray);

            //计算击中符号圈直径
            CenterSize = (int)(PointLength / 1.5);

            //初始化像素和毫米之间的映射比例
            GetScreenData(ScreenInch, ScreenLengthWidthRatio);

            //创建一个数据源
            ShootPoints = new List<ShootPoint>();
        }

        ~ShootBoard()
        {
            //关闭前保存校准值
            SaveCorrectData();
            SaveScreenData();
        }

        #region 测试板参数获取及处理

        //显示系统屏幕参数
        public void ShowSystemScrrenPrams()
        {
            IntPtr hdc = GrphcBuf.Graphics.GetHdc();

            if (Environment.OSVersion.Version.Major >= 6)
                SetProcessDPIAware();

            int Px = (int)(ScaleX * GetDeviceCaps(hdc, (int)DeviceCap.LOGPIXELSX)); //每英寸有几个像素点
            int Py = (int)(ScaleY * GetDeviceCaps(hdc, (int)DeviceCap.LOGPIXELSY));

            GrphcBuf.Graphics.ReleaseHdc(hdc);

            ShowString("从系统获取的数据如下：", new Point(350, 10));

            ShowString("每英寸像素点数：Px = " + Px.ToString(), new Point(350, 50));
            ShowString("每英寸像素点数：Py = " + Py.ToString(), new Point(350, 70));

            int test1 = System.Windows.Forms.Screen.PrimaryScreen.Bounds.Width;
            int test2 = System.Windows.Forms.Screen.PrimaryScreen.Bounds.Height;

            ShowString("当前显示器像素点数：PrimaryScreen.Bounds.Width = " + test1.ToString(), new Point(350, 110));
            ShowString("当前显示器像素点数：PrimaryScreen.Bounds.Height = " + test2.ToString(), new Point(350, 130));

            int test3 = System.Windows.Forms.Screen.PrimaryScreen.WorkingArea.Height;
            int test4 = System.Windows.Forms.Screen.PrimaryScreen.WorkingArea.Width; ;

            ShowString("显示器工作区像素点数：PrimaryScreen.WorkingArea.Width = " + test4.ToString(), new Point(350, 170));
            ShowString("显示器工作区像素点数：PrimaryScreen.WorkingArea.Height = " + test3.ToString(), new Point(350, 190));

            Graphics currentGraphics = Graphics.FromHwnd(GrpBxRef.Handle);
            double dpixRatio = currentGraphics.DpiX / 96;
            double dpiyRatio = currentGraphics.DpiY / 96;

            ShowString("显示器放缩比率：dpixRatio = " + dpixRatio.ToString(), new Point(350, 230));
            ShowString("显示器放缩比率：dpiyRatio = " + dpiyRatio.ToString(), new Point(350, 250));

            using (ManagementClass mc = new ManagementClass("Win32_DesktopMonitor"))
            {
                using (ManagementObjectCollection moc = mc.GetInstances())
                {
                    int PixelsPerXLogicalInch = 0; // dpi for x
                    int PixelsPerYLogicalInch = 0; // dpi for y
                    foreach (ManagementObject each in moc)
                    {
                        PixelsPerXLogicalInch = int.Parse((each.Properties["PixelsPerXLogicalInch"].Value.ToString()));
                        PixelsPerYLogicalInch = int.Parse((each.Properties["PixelsPerYLogicalInch"].Value.ToString()));
                    }
                }
            }
        }

        #region 存取

        //保存校准值
        private void SaveCorrectData()
        {
            DataTools.WriteIni("CORRECT", "MaxVoletage_X", MaxVoletage_X.ToString());
            DataTools.WriteIni("CORRECT", "MinVoletage_X", MinVoletage_X.ToString());
            DataTools.WriteIni("CORRECT", "MaxVoletage_Y", MaxVoletage_Y.ToString());
            DataTools.WriteIni("CORRECT", "MinVoletage_Y", MinVoletage_Y.ToString());

            DataTools.WriteIni("TRIGGER", "TrigValue_X", TrigValue_X.ToString());
            DataTools.WriteIni("TRIGGER", "TrigValue_Y", TrigValue_Y.ToString());

        }

        //保存屏幕信息
        private void SaveScreenData()
        {
            DataTools.WriteIni("SCREENDATA", "ScreenInch", ScreenInch.ToString());
            DataTools.WriteIni("SCREENDATA", "ScreenLengthWidthRatio", ScreenLengthWidthRatio.ToString());

            DataTools.WriteIni("SCREENDATA", "Point_VolRate_X", Point_VolRate_X.ToString());
            DataTools.WriteIni("SCREENDATA", "X_Offset", X_Offset.ToString());
            DataTools.WriteIni("SCREENDATA", "Point_VolRate_Y", Point_VolRate_Y.ToString());
            DataTools.WriteIni("SCREENDATA", "Y_Offset", Y_Offset.ToString());
        }

        //加载校准值
        private void LoadCorrectData()
        {
            TrigValue_X = Convert.ToUInt16(DataTools.ReadIni("TRIGGER", "TrigValue_X", "0"));
            TrigValue_Y = Convert.ToUInt16(DataTools.ReadIni("TRIGGER", "TrigValue_Y", "0"));

            MaxVoletage_X = Convert.ToUInt16(DataTools.ReadIni("CORRECT", "MaxVoletage_X", "0"));
            MinVoletage_X = Convert.ToUInt16(DataTools.ReadIni("CORRECT", "MinVoletage_X", "0"));
            MaxVoletage_Y = Convert.ToUInt16(DataTools.ReadIni("CORRECT", "MaxVoletage_Y", "0"));
            MinVoletage_Y = Convert.ToUInt16(DataTools.ReadIni("CORRECT", "MinVoletage_Y", "0"));
        }

        //加载屏幕信息
        private void LoadScreenData()
        {
            ScreenInch = Convert.ToDouble(DataTools.ReadIni("SCREENDATA", "ScreenInch", "1"));
            ScreenLengthWidthRatio = Convert.ToDouble(DataTools.ReadIni("SCREENDATA", "ScreenLengthWidthRatio", "1"));

            Point_VolRate_X = Convert.ToDouble(DataTools.ReadIni("SCREENDATA", "Point_VolRate_X", "1"));
            X_Offset = Convert.ToDouble(DataTools.ReadIni("SCREENDATA", "X_Offset", "0"));
            Point_VolRate_Y = Convert.ToDouble(DataTools.ReadIni("SCREENDATA", "Point_VolRate_Y", "1"));
            Y_Offset = Convert.ToDouble(DataTools.ReadIni("SCREENDATA", "Y_Offset", "0"));
        }
        #endregion

        #region 校准

        //从测量结果中计算像素点与实际物理单位的映射，输入参数单位为毫米
        public void GetScreenData(double Inch, double Ratio)
        {
            //直角三角形斜边（对角线长度）
            double Hypotenuse = Inch * mmInchRatio;

            //Ratio相当于角Theta的正切值
            double Cos_2 = 1 / (1 + Ratio * Ratio);
            double CosTheta = Math.Sqrt(Cos_2);
            double SinTheta = Math.Sqrt(1 - Cos_2);

            //求得屏幕长宽
            double ScreenWidth = Hypotenuse * SinTheta;
            double ScreenHeight = Hypotenuse * CosTheta;

            //保存参数
            ScreenInch = Inch;
            ScreenLengthWidthRatio = Ratio;

            //毫米/像素点
            mm_PixelRate_X = ScreenWidth / System.Windows.Forms.Screen.PrimaryScreen.Bounds.Width;
            mm_PixelRate_Y = ScreenHeight / System.Windows.Forms.Screen.PrimaryScreen.Bounds.Height;
        }

        //开始校准操作
        public void StartTouchCorrect()
        {
            //复位校准计数器
            CorrectCnt = CorrectProcess.Start_Point1;
        }

        //中止校准操作
        public void AbortTouchCorrect()
        {
            CorrectCnt = CorrectProcess.End;
        }

        //延时结束，此函数仅在校准进程在等待时有效
        public void TouchCorrectDelayComplet()
        {
            if(CorrectProcess.Wait_Point1 == CorrectCnt || CorrectProcess.Wait_Point2 == CorrectCnt)
            {
                //使校准进程移动到下一状态
                CorrectCnt++;
            }
        }

        //获取校准值
        public CorrectProcess GetTouchCorrectData(ushort X_Voltage, ushort Y_Voltage)
        {
            switch(CorrectCnt)
            {
                //左上角校准
                case CorrectProcess.Start_Point1:
                    LowVoletage_X = X_Voltage;

                    LowVoletage_Y = Y_Voltage;
                    
                    CorrectCnt++;
                    break;

                case CorrectProcess.Wait_Point1:
                    break;

                //右下角校准
                case CorrectProcess.Start_Point2:
                    HighVoletage_X = X_Voltage;
                    HighVoletage_Y = Y_Voltage;

                    GetRate_Offset();
                    CorrectCnt++;
                    break;

                case CorrectProcess.Wait_Point2:
                    break;

                default: break;
            }
            return CorrectCnt;
        }

        //计算拟合公式
        private void GetRate_Offset()
        {
            //计算Y方向斜率
            Point_VolRate_Y = ((GrpBxRef.Height - StartOffset) - StartOffset) / (HighVoletage_Y - LowVoletage_Y);

            //Y方向截距
            Y_Offset = StartOffset - (int)(Point_VolRate_Y * LowVoletage_Y);

            //计算X方向斜率
            Point_VolRate_X = ((GrpBxRef.Width - StartOffset) - StartOffset) / (HighVoletage_X - LowVoletage_X);

            //X方向截距
            X_Offset = StartOffset - (int)(Point_VolRate_X * LowVoletage_X);

            //获得新的边界数据
            MaxVoletage_X = (ushort)((GrpBxRef.Width - X_Offset) / Point_VolRate_X);
            MinVoletage_X = (ushort)((0 - X_Offset) / Point_VolRate_X);

            MaxVoletage_Y = (ushort)((GrpBxRef.Height - Y_Offset) / Point_VolRate_Y);
            MinVoletage_Y = (ushort)((0 - Y_Offset) / Point_VolRate_Y);

        }
        #endregion
        #endregion

        //清屏
        public void CleanScreen()
        {
            //清屏
            GrphcBuf.Graphics.Clear(Gray);

            GrphcBuf.Render();
        }

        //局部清屏
        //用底色覆盖原先的显示,用矩形的左上角和右下角指定擦除区域
        public void ClearScreenPartialy(Point Start, int Width, int Hight)
        {
            //申请一个底色画刷
            Pen pen = new Pen(ShootBoard.Gray);
            Graphics Grphc = this.GrphcBuf.Graphics;

            //指定方形区域
            Rectangle Local = new Rectangle(Start.X, Start.Y, Width, Hight);

            //覆盖待清除区域
            Grphc.FillRectangle(pen.Brush, Local);

            //写入缓存
            GrphcBuf.Render();
        }

        //重绘图形时调用，用于避免重绘时丢失图形
        public void RePiantGraphics()
        {
            this.GrphcBuf.Render();
        }

        #region 画图（图标、符号等）

        //画一个点(此单位为mm，将在内部进行转换)
        private void DrawPoint(ShootPoint P, Color color)
        {
            //申请一个蓝画刷
            Pen pen = new Pen(color);
            Graphics Grphc = this.GrphcBuf.Graphics;

            //反向映射
            int Point_X = (int)(P.X_Pos / mm_PixelRate_X);
            int Point_Y = (int)(P.Y_Pos / mm_PixelRate_Y);

            //获得外框矩形参数
            Rectangle RecCenter = new Rectangle(Point_X - 4, Point_Y - 4, 8, 8);

            //画圆心
            Grphc.FillEllipse(pen.Brush, RecCenter);

            //从缓冲区输出至图框
            this.GrphcBuf.Render();
        }

        //画一个圆(此单位为mm，将在内部进行转换)
        private void DrawCircle(Circle C, Color color)
        {
            //申请一个画刷
            Pen pen = new Pen(color);
            Graphics Grphc = this.GrphcBuf.Graphics;

            //反向映射
            double Point_X = C.Center.X_Pos / mm_PixelRate_X;
            double Point_Y = C.Center.Y_Pos / mm_PixelRate_Y;

            //获得外框矩形参数
            Rectangle Rec = new Rectangle((int)(Point_X - C.Radius / mm_PixelRate_X), (int)(Point_Y - C.Radius / mm_PixelRate_Y),
                                          (int)(C.Radius * 2 / mm_PixelRate_X), (int)(C.Radius * 2 / mm_PixelRate_Y));

            pen.Width = 4;

            pen.DashStyle = DashStyle.Dot;

            //画圆
            Grphc.DrawEllipse(pen, Rec);

            //从缓冲区输出至图框
            this.GrphcBuf.Render();

            //画圆心
            DrawPoint(C.Center, color);
        }

        //画一个击中标识,被画的对象，中心点坐标
        private void DrawShootFlag(int X, int Y)
        {
            //申请一个红画刷
            Pen pen = new Pen(ShootBoard.Red);
            Graphics Grphc = this.GrphcBuf.Graphics;

            //原点在左上角
            Rectangle TestRectangle = new Rectangle(X - (PointLength / 2), Y - (PointLength / 2),
                                                    PointLength, PointLength);

            pen.Width = 2;

            //画椭圆，基于画笔和外边框
            Grphc.DrawEllipse(pen, X - (CenterSize / 2), Y - (CenterSize / 2), CenterSize, CenterSize);

            //画两条线
            Grphc.DrawLine(pen, X - (PointLength / 2), Y - (PointLength / 2),
                                X + (PointLength / 2), Y + (PointLength / 2));
            Grphc.DrawLine(pen, X + (PointLength / 2), Y - (PointLength / 2),
                                X - (PointLength / 2), Y + (PointLength / 2));

            //画矩形
            Grphc.DrawRectangle(pen, TestRectangle);

            //从缓冲区输出至图框
            this.GrphcBuf.Render();

            #region 测试用
            ////申请一个蓝画刷
            //Pen pen = new Pen(ShootBoard.Red);
            //Graphics Grphc = this.GrphcBuf.Graphics;

            ////获得外框矩形参数
            //Rectangle RecCenter = new Rectangle(X - 4, Y - 4, 8, 8);

            ////画圆心
            //Grphc.FillEllipse(pen.Brush, RecCenter);

            ////从缓冲区输出至图框
            //this.GrphcBuf.Render();
            #endregion
        }

        //画校准块，输入为校准块编号（0代表左上角，1代表右下角)
        public void DrawCorrectBlock(int BlockNum)
        {
            //获取显示长宽
            int Height = GrpBxRef.Height;
            int Weight = GrpBxRef.Width;

            switch (BlockNum)
            {
                case 0:
                    DrawShootFlag(StartOffset, StartOffset);
                    break;

                case 1:
                    DrawShootFlag(Weight - StartOffset, Height - StartOffset);
                    break;

                default:break;
            }
            
        }

        //标记通信是否正常
        public void ShowCOM_State(bool isNormal)
        {
            //画刷线宽
            int PenWidth = 7;

            //申请一个绿画刷,宽度为5像素
            Pen pen = new Pen(ShootBoard.Red, PenWidth);
            Graphics Grphc = this.GrphcBuf.Graphics;

            //获取显示长宽
            int Height = GrpBxRef.Height;
            int Weight = GrpBxRef.Width;

            //若通信正常，则显示为绿色
            if (isNormal)
            {
                pen.Color = ShootBoard.Green;
            }

            //屏边框代表通信状态
            Grphc.DrawLine(pen, PenWidth / 2, 0, PenWidth / 2, Height);
            Grphc.DrawLine(pen, Weight - PenWidth / 2, 0, Weight - PenWidth / 2, Height);
            Grphc.DrawLine(pen, 0, PenWidth / 2, Weight, PenWidth / 2);
            Grphc.DrawLine(pen, 0, Height - PenWidth / 2, Weight, Height - PenWidth / 2);

            this.GrphcBuf.Render();
        }
        #endregion

        #region 写字

        //在屏幕某处显示一字符串
        public void ShowString(string Str, Point Location)
        {
            FontFamily fontFamily = new FontFamily("Arial");
            Font font = new Font(fontFamily, InfoFontSize, FontStyle.Regular, GraphicsUnit.Pixel);

            try
            {
                GrphcBuf.Graphics.DrawString(Str, font, new SolidBrush(Color.Red), Location.X, Location.Y);
            }
            catch (Exception ex)
            {
                MessageBox.Show("显示字符串：" + ex.Message, "出错了！");
            }

            //重绘窗体
            RePiantGraphics();
        }

        //清空通知区域
        public void ClearInfoArea()
        {
            //以换行符将显示区域分割
            string[] S = InfoMsg.Split('\n');

            //清除每一行对应的字符区域
            for(int i = 0; i < S.Length; i++)
            {
                //每行的水平起始位置是一样的
                ClearScreenPartialy(new Point(ShowFontLocation.X, ShowFontLocation.Y + i * 20), S[i].Length * 8, 20);
            }
            
        }
        #endregion

        #region 击中点数据处理

        //为测试板获取一个击中点，在此产生后续操作
        public void GetShootPoint(ushort X_Voltage, ushort Y_Voltage)
        {
            //计算映射出来的像素点坐标
            int Pixel_X = (int)(X_Voltage * Point_VolRate_X + X_Offset);
            int Pixel_Y = (int)(Y_Voltage * Point_VolRate_Y + Y_Offset);

            //计算映射出来的实际毫米单位制坐标
            double X_Pos = Pixel_X * mm_PixelRate_X;
            double Y_Pos = Pixel_Y * mm_PixelRate_Y;

            //向链表中添加元素
            ShootPoints.Add(new ShootPoint(X_Pos, Y_Pos));

            //画击中标志
            DrawShootFlag(Pixel_X, Pixel_Y);
        }

        //开始收集击中点信息
        public void StartCollectPoints()
        {
            //清空原有元素
            ShootPoints.Clear();

            ClearInfoArea();
        }

        //结束收集击中点信息，并给出运算结果,返回信息
        public string EndCollectPoints()
        {
            //求针对所有点的最小包络圆
            Circle MinCircle = GetMinimumEnvelopingCircle(ref ShootPoints);

            //遍历存储点填充所有的与圆心的距离
            ShootPoints.ForEach(
                delegate (ShootPoint P)
                {
                    Vector2 Vec = new Vector2(P, MinCircle.Center);

                    P.DistansToCenter = Vec.GetModulus();
                });

            //基于间距和的大小进行排序,P1>P2时返回正数，小于时返回负数，等于返回0
            ShootPoints.Sort(
                delegate (ShootPoint P1, ShootPoint P2)
                {
                    return P1.DistansToCenter == P2.DistansToCenter ? 0 :
                            (P1.DistansToCenter > P2.DistansToCenter ? 1 : -1);
                });

            //计算删除元素的个数
            int RemoveCnt = (int)(ShootPoints.Count * 0.1);

            //计算删除元素的起始下标
            int RemoveIndex = ShootPoints.Count - RemoveCnt;

            //舍去外围边界处10%的点
            ShootPoints.RemoveRange(RemoveIndex, RemoveCnt);

            //获得新的最小包络圆
            MinCircle = GetMinimumEnvelopingCircle(ref ShootPoints);

            //画出最小包络圆
            DrawCircle(MinCircle, Color.Green);

            //显示包络圆信息
            InfoMsg = "Circle_Center = {" + MinCircle.Center.X_Pos.ToString()
                        + ", " + MinCircle.Center.Y_Pos.ToString() + "} (mm);\nCircle_Radius = "
                        + MinCircle.Radius.ToString() + " (mm).";

            ShowFontLocation.X = 14;
            ShowFontLocation.Y = GrpBxRef.Height - 50;

            ShowString(InfoMsg, ShowFontLocation);

            return InfoMsg;
        }

        #region 求最小包络圆

        //从给定点集中求出其最小包络圆
        public Circle GetMinimumEnvelopingCircle(ref List<ShootPoint> Points)
        {
            //安全保护
            if(Points.Count < 2)
            {
                return new Circle();
            }

            //获得间距最大的两点下标
            int[] Index = GetMaxDistancePointIndex(ref Points);

            //获得以此两点连线为直径的圆
            Circle MinCircle = new Circle(Points.ElementAt(Index[0]), Points.ElementAt(Index[1]));

            DrawPoint(Points.ElementAt(Index[0]), Color.Green);
            DrawPoint(Points.ElementAt(Index[1]), Color.Green);
            //DrawCircle(MinCircle, Color.Green);

            //找到与圆心相距最远的点
            int MaxIndex = GetMaxDistancePointIndex(MinCircle.Center, ref Points);

            //临时保存下标
            int TempIndex = 0;

            //看这一点在不在圆内，若在，则最小包络圆已经找到
            if (MinCircle.isInThisCircle(Points.ElementAt(MaxIndex)))
            {
                return MinCircle;
            }
            else
            {
                //若此点不在圆内，则最小包络圆至少为此三点共同确定的圆。
                MinCircle = new Circle( Points.ElementAt(Index[0]),
                                        Points.ElementAt(Index[1]),
                                        Points.ElementAt(MaxIndex) );

                DrawPoint(Points.ElementAt(MaxIndex), Color.Blue);
                //DrawCircle(MinCircle, Color.Blue);

                //记录第三点的下标
                TempIndex = MaxIndex;
            }

            //找到与圆心距离最远的点下标
            MaxIndex = GetMaxDistancePointIndex(MinCircle.Center, ref Points);

            Color TempColor = Color.Black;

            //存储上次用于生成圆的三个点
            List<ShootPoint> CirclePoints = new List<ShootPoint>();

            //存入这三个点
            CirclePoints.Add(Points.ElementAt(Index[0]));
            CirclePoints.Add(Points.ElementAt(Index[1]));
            CirclePoints.Add(Points.ElementAt(TempIndex));

            int TestCnt = 0;

            //循环得以继续的条件是，与圆心距离最远的点不在圆内
            while (!MinCircle.isInThisCircle(Points.ElementAt(MaxIndex)))
            {
                 //新点必在新构建的圆上，因此需在上次的三个点中取两点构成新圆
                MinCircle = GetNewCircle(Points.ElementAt(MaxIndex), ref CirclePoints);

                DrawPoint(Points.ElementAt(MaxIndex), TempColor);
                //DrawCircle(MinCircle, TempColor);

                //若发现了异常结果，就终止循环以防止程序陷入死循环
                if(MinCircle.Radius < 1e10 * double.Epsilon)
                {
                    TestCnt++;

                    if(TestCnt > 3)
                    {
                        MessageBox.Show("被你撞见了！计算过程中出现了意外的空圆！\n虽极小概率情况下才会影响计算结果（例如这次），但请联系开发者解决此Bug！", "OPPs!");
                        break;
                    }
                }

                //在循环末尾找到与新圆心距离最远的点的下标
                MaxIndex = GetMaxDistancePointIndex(MinCircle.Center, ref Points);
            }

            return MinCircle;
        }

        //获得三个点的最小包络圆，并将序列中不在圆上的点移除
        private Circle GetMinCircleOf3Points(ref List<ShootPoint> Points)
        {
            //判断三点构成了何种三角形
            if(isAcuteTriangle(ref Points))
            {
                //若为锐角三角形，则最小包络圆为三角形外接圆
                return new Circle( Points.ElementAt(0),
                                   Points.ElementAt(1),
                                   Points.ElementAt(2) );
            }
            else
            {
                //直角三角形和钝角三角形的最小包络圆都是以最长边为直径的圆
                int[] Index = GetMaxDistancePointIndex(ref Points);

                Circle Result = new Circle(Points.ElementAt(Index[0]), Points.ElementAt(Index[1]));

                //移除不在圆上的点
                //说明：点集中有三个点，下标分别为0，1，2.而Index中必含有其中两个。
                //      那么以0+1+2的结果减去连个下标，自然将剩下被排除在外的那个元素下标。
                Points.RemoveAt(3 - Index[0] - Index[1]);

                return Result;
            }
        }

        //获取点集中，间距最大的两点的下标
        private int[] GetMaxDistancePointIndex(ref List<ShootPoint> Points)
        {
            //构造间距（平方）矩阵
            double[] Mat = new double[Points.Count * Points.Count];
            int[] Index = new int[2];

            //单次循环中使用的指定点
            ShootPoint SpecifiedPoint;
            ShootPoint TempPoint;

            //计算用中间变量
            double DeltaX = 0;
            double DeltaY = 0;

            //填充矩阵（按行循环）
            for (int i = 0; i < Points.Count; i++)
            {
                //获取指定点（行下标对应点）
                SpecifiedPoint = Points.ElementAt(i);

                //填充此行列元素，行的列元素含义为指定点与其他各点的间距
                for(int j = 0; j < Points.Count; j++)
                {
                    //获取列下标对应的点
                    TempPoint = Points.ElementAt(j);

                    //计算坐标增量
                    DeltaX = TempPoint.X_Pos - SpecifiedPoint.X_Pos;
                    DeltaY = TempPoint.Y_Pos - SpecifiedPoint.Y_Pos;

                    //获取两点间距的平方（因为最终只是比大小，所以省去了开方这一步骤用以节省运算量）
                    Mat[i * Points.Count + j] = 
                        DeltaX * DeltaX + DeltaY * DeltaY;
                }
            }
            
            //遍历间距矩阵，找到值最大的元素
            int MaxIndex = Array.IndexOf(Mat, Mat.Max());

            //求得两点下标
            Index[0] = MaxIndex / Points.Count; //整除以获得行下标，代表一点
            Index[1] = MaxIndex % Points.Count; //取余以获得列下标，代表另一个点

            return Index;
        }   
        
        //获取点集中，与指定点距离最大的点下标
        private int GetMaxDistancePointIndex(ShootPoint P, ref List<ShootPoint> Points)
        {
            //构造间距（平方）序列
            double[] Mat = new double[Points.Count];

            //中间变量
            ShootPoint TempPoint;
            double DeltaX = 0;
            double DeltaY = 0;

            //填充序列
            for (int i = 0; i < Points.Count; i++)
            {
                //获取对应的点
                TempPoint = Points.ElementAt(i);

                //计算坐标增量
                DeltaX = TempPoint.X_Pos - P.X_Pos;
                DeltaY = TempPoint.Y_Pos - P.Y_Pos;

                //获取两点间距的平方（因为最终只是比大小，所以省去了开方这一步骤用以节省运算量）
                Mat[i] = DeltaX * DeltaX + DeltaY * DeltaY;
            }

            //遍历间距序列，找到值最大的元素
            return Array.IndexOf(Mat, Mat.Max());
        }     

        //从已经确定在圆上的一点和其他三点中确定最小包络圆
        private Circle GetNewCircle(ShootPoint P_Certain, ref List<ShootPoint> CirclePoints)
        {
            List<ShootPoint> Points = new List<ShootPoint>();
            List<ShootPoint> AllPoints = CirclePoints.GetRange(0, CirclePoints.Count);

            //添加必在圆上的点到点集中
            Points.Add(P_Certain);
            AllPoints.Add(P_Certain);

            //第一轮尝试
            Points.Add(CirclePoints.ElementAt(0));
            Points.Add(CirclePoints.ElementAt(1));

            //求出此三点的最小包络圆
            Circle MinCircle = GetMinCircleOf3Points(ref Points);

            //判断是否包含所有点
            if(isAllPointsInCircle(ref AllPoints, MinCircle))
            {
                //仅保留在圆上的点，用于优化算法运行速度
                CirclePoints = Points;

                return MinCircle;
            }
            else if(CirclePoints.Count < 3)
            {
                //如果在圆上的点比3小，就说明用于输入函数的点一定全拿来生成新圆了
                //如此还没通过上面的条件，那么一定是前面的运算中哪里算错了。
                return new Circle();
            }


            //第二轮尝试
            if(Points.Count  == 3)
            {
                //如果在生成上一个圆时，三个点都在圆上，拿掉最后面的点。
                Points.RemoveAt(2); 
            }
            Points.Add(CirclePoints.ElementAt(2));

            //求出此三点的最小包络圆
            MinCircle = GetMinCircleOf3Points(ref Points);

            //判断是否包含所有点
            if (isAllPointsInCircle(ref AllPoints, MinCircle))
            {
                //仅保留在圆上的点，用于优化算法运行速度
                CirclePoints = Points;

                return MinCircle;
            }

            //最后一轮尝试
            if (Points.Count == 3)
            {
                //如果在生成上一个圆时，三个点都在圆上，拿掉1点。
                Points.RemoveAt(1);
            }
            Points.Add(CirclePoints.ElementAt(1));

            //求出此三点的最小包络圆
            MinCircle = GetMinCircleOf3Points(ref Points);

            //判断是否包含所有点
            if (isAllPointsInCircle(ref AllPoints, MinCircle))
            {
                //仅保留在圆上的点，用于优化算法运行速度
                CirclePoints = Points;

                return MinCircle;
            }

            //按道理讲不会到这里，若到了这里，说明出现了输入上的问题。
            return new Circle();
        }

        //判断是否所有的点都在圆范围内。若是，则true。
        private bool isAllPointsInCircle(ref List<ShootPoint> Points, Circle C)
        {
            int index = GetMaxDistancePointIndex(C.Center, ref Points);

            return C.isInThisCircle(Points.ElementAt(index));
        }

        //判断三点构成的三角形是否为锐角三角形
        private bool isAcuteTriangle(ref List<ShootPoint> Points)
        {
            //假定三角形三个顶点分别为A、B、C。
            Vector2 AB = new Vector2(Points.ElementAt(0), Points.ElementAt(1));
            Vector2 AC = new Vector2(Points.ElementAt(0), Points.ElementAt(2));
            Vector2 BC = new Vector2(Points.ElementAt(1), Points.ElementAt(2));

            //利用向量内积定义判断三角形·内角·余弦值的正负，进而判断是否是钝角。
            double BAC = AB * AC;
            double ABC = -AB * BC;
            double ACB = AC * BC;

            //三角形中最多出现一个钝角，因此可以用此三值是否全正判断是否是锐角三角形。
            return (BAC * ABC * ACB) > 0;
        }
        #endregion
        #endregion
    }

    //坐标原点在显示区域左上角
    public class ShootPoint
    {
        //击中点X坐标(单位为mm)
        public double X_Pos;

        //击中点X坐标(单位为mm)
        public double Y_Pos;

        //与其他点之间的距离和
        public double DistansToCenter;

        //不含参数的构造函数
        public ShootPoint()
        {
            X_Pos = 0;
            Y_Pos = 0;

            DistansToCenter = 0;
        }

        //以坐标构造点
        public ShootPoint(double X, double Y)
        {
            X_Pos = X;
            Y_Pos = Y;

            DistansToCenter = 0;
        }
        //以位矢构造点
        public ShootPoint(Vector2 vec)
        {
            X_Pos = vec.X;
            Y_Pos = vec.Y;

            DistansToCenter = 0;
        }

    }

    //圆
    public class Circle
    {
        //圆心
        ShootPoint center;

        //半径
        double radius;

        //获取圆心
        public ShootPoint Center
        {
            get
            {
                return this.center;
            }
        }

        //获取半径
        public double Radius
        {
            get
            {
                return this.radius;
            }
        }

        //以圆心和半径构造圆
        public Circle(ShootPoint Center, double R)
        {
            center = Center;
            radius = R;
        }
        //以两点连线作为直径构造圆
        public Circle(ShootPoint Point1, ShootPoint Point2)
        {
            //获取圆心坐标
            double Center_X = (Point1.X_Pos + Point2.X_Pos) / 2;
            double Center_Y = (Point1.Y_Pos + Point2.Y_Pos) / 2;

            center = new ShootPoint(Center_X, Center_Y);

            //计算半径
            double DeltaX = center.X_Pos - Point2.X_Pos;
            double DeltaY = center.Y_Pos - Point2.Y_Pos;

            radius = Math.Sqrt(DeltaX * DeltaX + DeltaY * DeltaY);
        }
        //以三个不共线的点确定的三角形构造圆，此时圆为三角形的外接圆
        //若共线，则按照无参数方式构造空圆
        public Circle(ShootPoint Point1, ShootPoint Point2, ShootPoint Point3)
        {
            //求从点1出发指向点2、3的向量
            Vector2 Vec_1 = new Vector2(Point1, Point2);
            Vector2 Vec_2 = new Vector2(Point1, Point3);

            //如果斜率差小于一定阈值，则认为是一条直线
            if(Vec_1.isCollinear(Vec_2))
            {
                center = new ShootPoint(0, 0);
                radius = 0;
            }
            else
            {
                //求从点1出发指向点2、3向量的正交向量，作为其连线中垂线的方向向量
                Vector2 Vector_1 = Vec_1.GetOrthogonalVector();
                Vector2 Vector_2 = Vec_2.GetOrthogonalVector();

                //获取两直线中点
                ShootPoint CenterPoint1 = new ShootPoint((Point1.X_Pos + Point2.X_Pos) / 2, (Point1.Y_Pos + Point2.Y_Pos) / 2);
                ShootPoint CenterPoint2 = new ShootPoint((Point1.X_Pos + Point3.X_Pos) / 2, (Point1.Y_Pos + Point3.Y_Pos) / 2);

                ///则直线参数方程可以是：直线上某点P = CenterPoint + k * Vector_1.其中，k为任意实数。
                //据参数方程求中垂线交点参数
                double k1 = (Vector_2.X * (CenterPoint1.Y_Pos - CenterPoint2.Y_Pos)
                             + Vector_2.Y * (CenterPoint2.X_Pos - CenterPoint1.X_Pos)) 
                    / (Vector_1.X * Vector_2.Y - Vector_2.X * Vector_1.Y);

                //求得交点坐标,即为圆心坐标
                center = new ShootPoint(new Vector2(CenterPoint1) + k1 * Vector_1);

                ///验证正确性用
                //double k2 = (k1 * Vector_1.Y + CenterPoint1.Y_Pos - CenterPoint2.Y_Pos) / Vector_2.Y;
                //ShootPoint CheckCenter = new ShootPoint(new Vector2(CenterPoint2) + k2 * Vector_2);

                //计算半径
                double DeltaX = Point1.X_Pos - Center.X_Pos;
                double DeltaY = Point1.Y_Pos - Center.Y_Pos;

                radius = Math.Sqrt(DeltaX * DeltaX + DeltaY * DeltaY);
            }
        }
        //构造空圆
        public Circle()
        {
            center = new ShootPoint(0, 0);
            radius = 0;
        }

        //判断某点是否在圆范围内（在圆上和在圆内都算在圆范围内）
        public bool isInThisCircle(ShootPoint Point)
        {
            double DeltaX = Point.X_Pos - Center.X_Pos;
            double DeltaY = Point.Y_Pos - Center.Y_Pos;

            //忽略0.01mm以内的偏差，防止出现边界情况导致错误
            return (this.radius * this.radius + 1e-04) >=  (DeltaX * DeltaX + DeltaY * DeltaY);
        }
    }

    //二维向量
    public class Vector2
    {
        double x;
        double y;

        //获取坐标x
        public double X
        {
            get
            {
                return x;
            }
        }
        //获取坐标Y
        public double Y
        {
            get
            {
                return y;
            }
        }

        public Vector2(double in_x, double in_y)
        {
            x = in_x;
            y = in_y;
        }
        public Vector2(ShootPoint Point)
        {
            x = Point.X_Pos;
            y = Point.Y_Pos;
        }
        public Vector2(ShootPoint Src, ShootPoint Aim)
        {
            x = Aim.X_Pos - Src.X_Pos;
            y = Aim.Y_Pos - Src.Y_Pos;
        }
        public Vector2()
        {
            x = 0;
            y = 0;
        }

        //获取与本向量正交的一个向量，其X值为1。
        //这意味着其Y值将会是此向量作为方向向量时代表的直线的斜率值(若斜率存在)
        public Vector2 GetOrthogonalVector()
        {
            //与x轴重合的向量另行处理
            if(Math.Abs(this.y) < 1e10 * double.Epsilon)
                return new Vector2(0, 1);

            return new Vector2(1, -(x / y));
        }

        //判断向量是否共线
        public bool isCollinear(Vector2 Vec)
        {
            double Temp = 0;

            //零向量正交，不共线
            if(isZero() || Vec.isZero())
            {
                return false;
            }

            //取一个非零分量求取倍数
            if(Math.Abs(this.x) > 1e10 * double.Epsilon)
            {
                //求取系数
                Temp = Vec.X / this.X;

                return (Math.Abs(this.y * Temp - Vec.Y) < 1e10 * double.Epsilon);
            }
            else if(Math.Abs(this.y) > 1e10 * double.Epsilon)
            {
                //求取系数
                Temp = Vec.Y / this.Y;

                return (Math.Abs(this.X * Temp - Vec.X) < 1e10 * double.Epsilon);
            }

            return false;
        }

        //判断本向量是否是零向量
        public bool isZero()
        {
            return GetModulus() < 1e10 * double.Epsilon;
        }

        //向量求模
        public double GetModulus()
        {
            return Math.Sqrt(x * x + y * y);
        }

        //重载+运算符,向量相加
        public static Vector2 operator+(Vector2 left, Vector2 Right)
        {
            return new Vector2(left.X + Right.X, left.Y + Right.Y);
        }

        //重载-运算符，向量相减，左边减去右边。
        public static Vector2 operator-(Vector2 left, Vector2 Right)
        {
            return new Vector2(left.X - Right.X, left.Y - Right.Y);
        }

        //重载-运算符，向量取负。
        public static Vector2 operator -(Vector2 This)
        {
            return new Vector2(-1 * This.X, -1 * This.Y);
        }

        //重载*运算符，向量点乘(内积)。
        public static double operator *(Vector2 left, Vector2 Right)
        {
            return left.X * Right.X + left.Y * Right.Y;
        }

        //重载*运算符，向量数乘。
        public static Vector2 operator *(Vector2 left, double Right)
        {
            return new Vector2(left.X * Right, left.Y * Right);
        }
        public static Vector2 operator *(double left, Vector2 Right)
        {
            return new Vector2(left * Right.X, left * Right.Y);
        }
    }
}
