namespace HERO_ShootTest
{
    partial class Screen_Settings
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.btn_UpdateScreenSettings = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.tb_ScreenLength = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.tb_ScreenRate_W = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.tb_ScreenRate_H = new System.Windows.Forms.TextBox();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(24, 32);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(59, 12);
            this.label1.TabIndex = 0;
            this.label1.Text = "屏幕尺寸:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(24, 67);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(47, 12);
            this.label2.TabIndex = 1;
            this.label2.Text = "长宽比:";
            // 
            // btn_UpdateScreenSettings
            // 
            this.btn_UpdateScreenSettings.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btn_UpdateScreenSettings.Location = new System.Drawing.Point(121, 101);
            this.btn_UpdateScreenSettings.Name = "btn_UpdateScreenSettings";
            this.btn_UpdateScreenSettings.Size = new System.Drawing.Size(75, 23);
            this.btn_UpdateScreenSettings.TabIndex = 2;
            this.btn_UpdateScreenSettings.Text = "更新参数";
            this.btn_UpdateScreenSettings.UseVisualStyleBackColor = true;
            this.btn_UpdateScreenSettings.Click += new System.EventHandler(this.btn_UpdateScreenSettings_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.tb_ScreenRate_H);
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.tb_ScreenRate_W);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.tb_ScreenLength);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.btn_UpdateScreenSettings);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(214, 140);
            this.groupBox1.TabIndex = 3;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "长度参数";
            // 
            // tb_ScreenLength
            // 
            this.tb_ScreenLength.Location = new System.Drawing.Point(89, 29);
            this.tb_ScreenLength.Name = "tb_ScreenLength";
            this.tb_ScreenLength.Size = new System.Drawing.Size(68, 21);
            this.tb_ScreenLength.TabIndex = 3;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(163, 32);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(35, 12);
            this.label3.TabIndex = 4;
            this.label3.Text = "inch.";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(163, 67);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(11, 12);
            this.label4.TabIndex = 6;
            this.label4.Text = ".";
            // 
            // tb_ScreenRate_W
            // 
            this.tb_ScreenRate_W.Location = new System.Drawing.Point(89, 64);
            this.tb_ScreenRate_W.Name = "tb_ScreenRate_W";
            this.tb_ScreenRate_W.Size = new System.Drawing.Size(28, 21);
            this.tb_ScreenRate_W.TabIndex = 5;
            this.tb_ScreenRate_W.PreviewKeyDown += new System.Windows.Forms.PreviewKeyDownEventHandler(this.tb_ScreenRate_W_PreviewKeyDown);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("宋体", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label5.Location = new System.Drawing.Point(119, 67);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(12, 12);
            this.label5.TabIndex = 7;
            this.label5.Text = ":";
            // 
            // tb_ScreenRate_H
            // 
            this.tb_ScreenRate_H.Location = new System.Drawing.Point(129, 64);
            this.tb_ScreenRate_H.Name = "tb_ScreenRate_H";
            this.tb_ScreenRate_H.Size = new System.Drawing.Size(28, 21);
            this.tb_ScreenRate_H.TabIndex = 8;
            this.tb_ScreenRate_H.PreviewKeyDown += new System.Windows.Forms.PreviewKeyDownEventHandler(this.tb_ScreenRate_H_PreviewKeyDown);
            // 
            // Screen_Settings
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(238, 164);
            this.Controls.Add(this.groupBox1);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Screen_Settings";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "屏幕属性设置";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button btn_UpdateScreenSettings;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox tb_ScreenRate_W;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox tb_ScreenLength;
        private System.Windows.Forms.TextBox tb_ScreenRate_H;
        private System.Windows.Forms.Label label5;
    }
}