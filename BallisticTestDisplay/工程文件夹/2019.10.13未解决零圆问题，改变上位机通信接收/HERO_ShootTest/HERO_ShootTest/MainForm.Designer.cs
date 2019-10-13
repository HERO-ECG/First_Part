namespace HERO_ShootTest
{
    partial class MainForm
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.menuStrip = new System.Windows.Forms.MenuStrip();
            this.TSMI_COM = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMI_FullScreen = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMI_ComSet = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMI_ScreenSettings = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMI_Correct = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMI_Clear = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMI_Help = new System.Windows.Forms.ToolStripMenuItem();
            this.TSMI_SysScrnData = new System.Windows.Forms.ToolStripMenuItem();
            this.SPort = new System.IO.Ports.SerialPort(this.components);
            this.timer = new System.Windows.Forms.Timer(this.components);
            this.label1 = new System.Windows.Forms.Label();
            this.lb_CaptureCnt = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.btn__CleanText = new System.Windows.Forms.Button();
            this.rTB_Info = new System.Windows.Forms.RichTextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.lb_TestCnt = new System.Windows.Forms.Label();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.btn_AnalyseData = new System.Windows.Forms.Button();
            this.btn_InputData = new System.Windows.Forms.Button();
            this.menuStrip.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Location = new System.Drawing.Point(19, 44);
            this.groupBox1.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Padding = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.groupBox1.Size = new System.Drawing.Size(1152, 675);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "PaintBox";
            this.groupBox1.SizeChanged += new System.EventHandler(this.groupBox1_SizeChanged);
            this.groupBox1.Paint += new System.Windows.Forms.PaintEventHandler(this.groupBox1_Paint);
            this.groupBox1.Enter += new System.EventHandler(this.groupBox1_Enter);
            // 
            // menuStrip
            // 
            this.menuStrip.BackColor = System.Drawing.SystemColors.Control;
            this.menuStrip.Dock = System.Windows.Forms.DockStyle.None;
            this.menuStrip.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.menuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.TSMI_COM,
            this.TSMI_FullScreen,
            this.TSMI_ComSet,
            this.TSMI_ScreenSettings,
            this.TSMI_Correct,
            this.TSMI_Clear,
            this.TSMI_Help});
            this.menuStrip.Location = new System.Drawing.Point(0, 0);
            this.menuStrip.Name = "menuStrip";
            this.menuStrip.Padding = new System.Windows.Forms.Padding(8, 2, 0, 2);
            this.menuStrip.Size = new System.Drawing.Size(562, 28);
            this.menuStrip.TabIndex = 1;
            this.menuStrip.Text = "menuStrip";
            // 
            // TSMI_COM
            // 
            this.TSMI_COM.Name = "TSMI_COM";
            this.TSMI_COM.Size = new System.Drawing.Size(96, 24);
            this.TSMI_COM.Text = "连接下位机";
            this.TSMI_COM.Click += new System.EventHandler(this.TSMI_COM_Click_1);
            this.TSMI_COM.MouseEnter += new System.EventHandler(this.TSMI_COM_MouseEnter);
            // 
            // TSMI_FullScreen
            // 
            this.TSMI_FullScreen.Name = "TSMI_FullScreen";
            this.TSMI_FullScreen.Size = new System.Drawing.Size(81, 24);
            this.TSMI_FullScreen.Text = "全屏显示";
            this.TSMI_FullScreen.Click += new System.EventHandler(this.TSMI_FullScreen_Click);
            // 
            // TSMI_ComSet
            // 
            this.TSMI_ComSet.Name = "TSMI_ComSet";
            this.TSMI_ComSet.Size = new System.Drawing.Size(81, 24);
            this.TSMI_ComSet.Text = "通信设置";
            this.TSMI_ComSet.Click += new System.EventHandler(this.TSMI_ComSet_Click);
            // 
            // TSMI_ScreenSettings
            // 
            this.TSMI_ScreenSettings.Name = "TSMI_ScreenSettings";
            this.TSMI_ScreenSettings.Size = new System.Drawing.Size(81, 24);
            this.TSMI_ScreenSettings.Text = "屏幕设置";
            this.TSMI_ScreenSettings.Click += new System.EventHandler(this.TSMI_ScreenSettings_Click);
            // 
            // TSMI_Correct
            // 
            this.TSMI_Correct.Name = "TSMI_Correct";
            this.TSMI_Correct.Size = new System.Drawing.Size(81, 24);
            this.TSMI_Correct.Text = "触点校准";
            this.TSMI_Correct.Click += new System.EventHandler(this.TSMI_Correct_Click);
            // 
            // TSMI_Clear
            // 
            this.TSMI_Clear.Name = "TSMI_Clear";
            this.TSMI_Clear.Size = new System.Drawing.Size(81, 24);
            this.TSMI_Clear.Text = "清空捕获";
            this.TSMI_Clear.Click += new System.EventHandler(this.TSMI_Clear_Click);
            // 
            // TSMI_Help
            // 
            this.TSMI_Help.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.TSMI_SysScrnData});
            this.TSMI_Help.Name = "TSMI_Help";
            this.TSMI_Help.Size = new System.Drawing.Size(51, 24);
            this.TSMI_Help.Text = "帮助";
            // 
            // TSMI_SysScrnData
            // 
            this.TSMI_SysScrnData.Name = "TSMI_SysScrnData";
            this.TSMI_SysScrnData.Size = new System.Drawing.Size(174, 26);
            this.TSMI_SysScrnData.Text = "系统屏幕信息";
            this.TSMI_SysScrnData.Click += new System.EventHandler(this.TSMI_SysScrnData_Click);
            // 
            // SPort
            // 
            this.SPort.BaudRate = 115200;
            // 
            // timer
            // 
            this.timer.Enabled = true;
            this.timer.Interval = 20;
            this.timer.Tick += new System.EventHandler(this.timer_Tick);
            // 
            // label1
            // 
            this.label1.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(333, 21);
            this.label1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(97, 15);
            this.label1.TabIndex = 2;
            this.label1.Text = "捕获点计数：";
            // 
            // lb_CaptureCnt
            // 
            this.lb_CaptureCnt.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.lb_CaptureCnt.AutoSize = true;
            this.lb_CaptureCnt.Location = new System.Drawing.Point(444, 21);
            this.lb_CaptureCnt.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lb_CaptureCnt.Name = "lb_CaptureCnt";
            this.lb_CaptureCnt.Size = new System.Drawing.Size(23, 15);
            this.lb_CaptureCnt.TabIndex = 3;
            this.lb_CaptureCnt.Text = "0.";
            // 
            // groupBox2
            // 
            this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox2.Controls.Add(this.btn__CleanText);
            this.groupBox2.Controls.Add(this.rTB_Info);
            this.groupBox2.Location = new System.Drawing.Point(1179, 44);
            this.groupBox2.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Padding = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.groupBox2.Size = new System.Drawing.Size(283, 675);
            this.groupBox2.TabIndex = 4;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "通知消息";
            // 
            // btn__CleanText
            // 
            this.btn__CleanText.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btn__CleanText.Location = new System.Drawing.Point(8, 639);
            this.btn__CleanText.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.btn__CleanText.Name = "btn__CleanText";
            this.btn__CleanText.Size = new System.Drawing.Size(267, 29);
            this.btn__CleanText.TabIndex = 1;
            this.btn__CleanText.Text = "清空";
            this.btn__CleanText.UseVisualStyleBackColor = true;
            this.btn__CleanText.Click += new System.EventHandler(this.btn__CleanText_Click);
            // 
            // rTB_Info
            // 
            this.rTB_Info.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.rTB_Info.BackColor = System.Drawing.SystemColors.Control;
            this.rTB_Info.Location = new System.Drawing.Point(8, 25);
            this.rTB_Info.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.rTB_Info.Name = "rTB_Info";
            this.rTB_Info.ReadOnly = true;
            this.rTB_Info.Size = new System.Drawing.Size(265, 605);
            this.rTB_Info.TabIndex = 0;
            this.rTB_Info.Text = "";
            // 
            // label2
            // 
            this.label2.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(843, 21);
            this.label2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(119, 15);
            this.label2.TabIndex = 5;
            this.label2.Text = "TestSendCount:";
            this.label2.Click += new System.EventHandler(this.label2_Click);
            // 
            // lb_TestCnt
            // 
            this.lb_TestCnt.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.lb_TestCnt.AutoSize = true;
            this.lb_TestCnt.Location = new System.Drawing.Point(969, 21);
            this.lb_TestCnt.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lb_TestCnt.Name = "lb_TestCnt";
            this.lb_TestCnt.Size = new System.Drawing.Size(23, 15);
            this.lb_TestCnt.TabIndex = 6;
            this.lb_TestCnt.Text = "0.";
            this.lb_TestCnt.Click += new System.EventHandler(this.lb_TestCnt_Click);
            // 
            // groupBox3
            // 
            this.groupBox3.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox3.Controls.Add(this.btn_AnalyseData);
            this.groupBox3.Controls.Add(this.btn_InputData);
            this.groupBox3.Controls.Add(this.lb_TestCnt);
            this.groupBox3.Controls.Add(this.label1);
            this.groupBox3.Controls.Add(this.lb_CaptureCnt);
            this.groupBox3.Controls.Add(this.label2);
            this.groupBox3.Location = new System.Drawing.Point(19, 735);
            this.groupBox3.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Padding = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.groupBox3.Size = new System.Drawing.Size(1443, 51);
            this.groupBox3.TabIndex = 7;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "测试区";
            // 
            // btn_AnalyseData
            // 
            this.btn_AnalyseData.Location = new System.Drawing.Point(1169, 15);
            this.btn_AnalyseData.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.btn_AnalyseData.Name = "btn_AnalyseData";
            this.btn_AnalyseData.Size = new System.Drawing.Size(100, 29);
            this.btn_AnalyseData.TabIndex = 8;
            this.btn_AnalyseData.Text = "分析结果";
            this.btn_AnalyseData.UseVisualStyleBackColor = true;
            this.btn_AnalyseData.Click += new System.EventHandler(this.btn_AnalyseData_Click);
            // 
            // btn_InputData
            // 
            this.btn_InputData.Location = new System.Drawing.Point(1277, 15);
            this.btn_InputData.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.btn_InputData.Name = "btn_InputData";
            this.btn_InputData.Size = new System.Drawing.Size(157, 29);
            this.btn_InputData.TabIndex = 7;
            this.btn_InputData.Text = "输入100个随机点";
            this.btn_InputData.UseVisualStyleBackColor = true;
            this.btn_InputData.Click += new System.EventHandler(this.btn_InputData_Click);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoScroll = true;
            this.ClientSize = new System.Drawing.Size(1477, 801);
            this.Controls.Add(this.menuStrip);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.KeyPreview = true;
            this.MainMenuStrip = this.menuStrip;
            this.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.MaximizeBox = false;
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "HERO_ShootTest 32/20";
            this.Shown += new System.EventHandler(this.MainForm_Shown);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.MainForm_KeyDown);
            this.KeyUp += new System.Windows.Forms.KeyEventHandler(this.MainForm_KeyUp);
            this.menuStrip.ResumeLayout(false);
            this.menuStrip.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.MenuStrip menuStrip;
        private System.Windows.Forms.ToolStripMenuItem TSMI_COM;
        private System.IO.Ports.SerialPort SPort;
        private System.Windows.Forms.ToolStripMenuItem TSMI_ComSet;
        private System.Windows.Forms.ToolStripMenuItem TSMI_Help;
        private System.Windows.Forms.ToolStripMenuItem TSMI_Correct;
        private System.Windows.Forms.ToolStripMenuItem TSMI_ScreenSettings;
        private System.Windows.Forms.Timer timer;
        private System.Windows.Forms.ToolStripMenuItem TSMI_Clear;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label lb_CaptureCnt;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.RichTextBox rTB_Info;
        private System.Windows.Forms.Button btn__CleanText;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label lb_TestCnt;
        private System.Windows.Forms.ToolStripMenuItem TSMI_FullScreen;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.ToolStripMenuItem TSMI_SysScrnData;
        private System.Windows.Forms.Button btn_InputData;
        private System.Windows.Forms.Button btn_AnalyseData;
    }
}

