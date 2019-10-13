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
    public partial class ComSettings : Form
    {
        //声明引用
        InterfaceToSlave CommunicationRef;

        public ComSettings(ref InterfaceToSlave ComRef)
        {
            InitializeComponent();

            CommunicationRef = ComRef;

            PreLoadParams();

        }

        //预置参数选项
        private void PreLoadParams()
        {
            // 预置波特率
            switch (CommunicationRef.SP_Baudrate)
            {
                case "300":
                    cbBaudRate.SelectedIndex = 0;
                    break;
                case "600":
                    cbBaudRate.SelectedIndex = 1;
                    break;
                case "1200":
                    cbBaudRate.SelectedIndex = 2;
                    break;
                case "2400":
                    cbBaudRate.SelectedIndex = 3;
                    break;
                case "4800":
                    cbBaudRate.SelectedIndex = 4;
                    break;
                case "9600":
                    cbBaudRate.SelectedIndex = 5;
                    break;
                case "19200":
                    cbBaudRate.SelectedIndex = 6;
                    break;
                case "38400":
                    cbBaudRate.SelectedIndex = 7;
                    break;
                case "115200":
                    cbBaudRate.SelectedIndex = 8;
                    break;
                default:
                    {
                        MessageBox.Show("波特率预置参数错误！");
                        return;
                    }
            }

            //预置数据长度
            switch (CommunicationRef.SP_DataBits)
            {
                case "5":
                    cbDataBits.SelectedIndex = 0;
                    break;
                case "6":
                    cbDataBits.SelectedIndex = 1;
                    break;
                case "7":
                    cbDataBits.SelectedIndex = 2;
                    break;
                case "8":
                    cbDataBits.SelectedIndex = 3;
                    break;
                default:
                    {
                        MessageBox.Show("数据位预置参数错误!");
                        return;
                    }

            }
            //预置停止位
            switch (CommunicationRef.SP_StopBits)
            {
                case "1":
                    cbStop.SelectedIndex = 0;
                    break;
                case "1.5":
                    cbStop.SelectedIndex = 1;
                    break;
                case "2":
                    cbStop.SelectedIndex = 2;
                    break;
                default:
                    {
                        MessageBox.Show("停止位预置参数错误！");
                        return;
                    }
            }

            //预置校验位
            switch (CommunicationRef.SP_Parity)
            {
                case "NONE":
                    cbParity.SelectedIndex = 0;
                    break;
                case "ODD":
                    cbParity.SelectedIndex = 1;
                    break;
                case "EVEN":
                    cbParity.SelectedIndex = 2;
                    break;
                default:
                    {
                        MessageBox.Show("校验位预置参数错误！");
                        return;
                    }
            }

            if (CommunicationRef.State_Series.isopen)
            {
                cbBaudRate.Enabled = false;
                cbDataBits.Enabled = false;
                cbParity.Enabled = false;
                cbStop.Enabled = false;
            }
        }

        //更改波特率选择时
        private void cbBaudRate_SelectedIndexChanged(object sender, EventArgs e)
        {
            CommunicationRef.SP_Baudrate = cbBaudRate.Text;
        }

        //更改数据位选择时
        private void cbDataBits_SelectedIndexChanged(object sender, EventArgs e)
        {
            CommunicationRef.SP_DataBits =cbDataBits.Text;
        }

        //更改停止位选择时
        private void cbStop_SelectedIndexChanged(object sender, EventArgs e)
        {
            CommunicationRef.SP_StopBits = cbStop.Text;
        }

        //更改校验位选择时
        private void cbParity_SelectedIndexChanged(object sender, EventArgs e)
        {
            CommunicationRef.SP_Parity = cbParity.Text;
        }
    }
}
