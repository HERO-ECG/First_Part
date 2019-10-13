using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace HERO_ShootTest
{
    public partial class Screen_Settings : Form
    {
        //保存被修改对象的引用
        ShootBoard BoardRef;

        public Screen_Settings(ref ShootBoard ScreenRef)
        {
            InitializeComponent();

            //初始化引用
            BoardRef = ScreenRef;
        }

        //更新参数按钮被按下
        private void btn_UpdateScreenSettings_Click(object sender, EventArgs e)
        {
            double Inch = 1;
            double WidthRatio = 1;
            double HeightRatio = 1;

            try
            {
                Inch = Convert.ToDouble(tb_ScreenLength.Text);
                WidthRatio = Convert.ToDouble(tb_ScreenRate_W.Text);
                HeightRatio = Convert.ToDouble(tb_ScreenRate_H.Text);
            }
            catch(Exception ex)
            {
                MessageBox.Show(ex.Message, "出错了！");
            }

            //将测得的数据交由测量板模块处理
            BoardRef.GetScreenData(Inch, WidthRatio / HeightRatio);

            MessageBox.Show("参数更新成功！", "提示");

            //关闭窗体
            this.Close();
        }

        //输入数字后，就让光标跳到后一个输入框里去
        private void tb_ScreenRate_H_PreviewKeyDown(object sender, PreviewKeyDownEventArgs e)
        {
            if(e.KeyCode == Keys.Enter)
            {
                this.ActiveControl = btn_UpdateScreenSettings;
            }
        }

        //输入数字后，就让光标跳到后一个输入框里去
        private void tb_ScreenRate_W_PreviewKeyDown(object sender, PreviewKeyDownEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                this.ActiveControl = tb_ScreenRate_H;
            }
        }
    }
}
