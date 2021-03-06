using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace RobotPC
{
    public partial class Form1 : Form
    {
        byte[] PacketRec = new byte[2000];
        byte[] TrmMass = new byte[2000];


        IPAddress IPaddr;
        IPEndPoint ep;
        int TrmPort = 51731;
        public Socket s;
        String IPStr;
        bool StopCharts = false;
        int CntUDPArray;
        ushort KGyro1, WaitForCfm;


        List<float> Tilt1XList = new List<float>();
        List<float> Tilt1YList = new List<float>();
        List<float> Gyro1List = new List<float>();

        List<float> Tilt2XList = new List<float>();
        List<float> Tilt2YList = new List<float>();
        List<float> Gyro2List = new List<float>();

        List<float> Tilt3XList = new List<float>();
        List<float> Tilt3YList = new List<float>();
        List<float> Gyro3List = new List<float>();

        List<float> Tilt4XList = new List<float>();
        List<float> Tilt4YList = new List<float>();
        List<float> Gyro4List = new List<float>();

        List<float> Tilt5XList = new List<float>();
        List<float> Tilt5YList = new List<float>();
        List<float> Gyro5List = new List<float>();

        List<float> Tilt6XList = new List<float>();
        List<float> Tilt6YList = new List<float>();
        List<float> Gyro6List = new List<float>();

        List<float> Tilt7XList = new List<float>();
        List<float> Tilt7YList = new List<float>();
        List<float> Gyro7List = new List<float>();

        List<uint> CntSamplesList = new List<uint>();


        List<float> GyroHipJointRList = new List<float>();
        List<float> GyroHipJointLList = new List<float>();
        List<float> GyroHipRList = new List<float>();
        List<float> GyroHipLList = new List<float>();
        List<float> GyroLowerlegRList = new List<float>();
        List<float> GyroLowerlegLList = new List<float>();
        List<float> GyroFootRList = new List<float>();
        List<float> GyroFootLList = new List<float>();

        List<float> YHipJointRList = new List<float>();
        List<float> YHipJointLList = new List<float>();
        List<float> XHipRList = new List<float>();
        List<float> XHipLList = new List<float>();
        List<float> XLowerLegRList = new List<float>();
        List<float> XLowerLegLList = new List<float>();
        List<float> XFootRList = new List<float>();
        List<float> XFootLList = new List<float>();

        


        const int ListCount = 50;


        const byte SET_Addr = 1;
        const byte CALIBR1 = 2;
        const byte CALIBR2 = 3;
        const byte CALIBR3 = 4;
        const byte CALIBR4 = 5;
        const byte CALIBR5 = 6;
        const byte CALIBR6 = 7;
        const byte CALIBR7 = 8;
        const byte SET_KGyro = 9;
        const byte SET_PWM_Motor_L1 = 10;
        const byte SET_PWM_Motor_L2 = 11;
        const byte SET_PWM_Motor_L3 = 12;
        const byte SET_PWM_Motor_L4 = 13;
        const byte SET_PWM_Motor_R1 = 14;
        const byte SET_PWM_Motor_R2 = 15;
        const byte SET_PWM_Motor_R3 = 16;
        const byte SET_PWM_Motor_R4 = 17;
        const byte GET_OPTIONS = 18;
        const byte WRITE_OPTIONS = 19;
        const byte CLEAR_ALL = 20;



        const byte TRM_DATA_PACKET = 1;
        const byte TRM_CFM_PACKET = 2;

        public Form1()
        {
            InitializeComponent();

            backgroundWorker1.RunWorkerAsync();
            s = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            chart1.Series[0].Color = Color.Blue;
            chart1.Series[1].Color = Color.Red;
            chart1.Series[2].Color = Color.Green;

            comboBox1.SelectedIndex = 0;
            comboBox2.SelectedIndex = 0;
            comboBox3.SelectedIndex = 0;


            //        dataGridView1.Rows.Add();

            //dataGridView1.Rows[0].Cells[0].Value = "названи";
    //        dataGridView1.Rows[0].HeaderCell.Value = "sdfsdfsdf";
   //         dataGridView1["Column1", dataGridView1.Rows.Count - 1].Value = "Пример 1: " + 3;

        }

        private void button1_Click(object sender, EventArgs e)
        {

        }

        public byte CalcCheckSummTrm()
        {
            uint Summ = 0, j;
            byte n = (byte)(TrmMass[2] + 3);
            for (j = 2; j < n; j++)
                Summ = Summ + TrmMass[j];

            Summ = ~Summ;

            return (byte)Summ;

        }



        public byte CalcCheckSummRec()
        {
            uint Summ = 0, j;
            byte n = (byte)(PacketRec[0] + 1);
            for (j = 0; j < n; j++)
                Summ = Summ + PacketRec[j];

            Summ = ~Summ;

            return (byte)Summ;

        }

        private void buttonstop_Click(object sender, EventArgs e)
        {
            if (StopCharts)
            {
                StopCharts = false;
                buttonstop.Text = "Stop";
            }
            else
            {
                StopCharts = true;
                buttonstop.Text = "Continue";
            }

        }

        private void button10_Click(object sender, EventArgs e)
        {
            chart1.Series[0].Points.Clear();
            chart1.Series[1].Points.Clear();
            chart1.Series[2].Points.Clear();
        }

        private void checkBox2_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox2.Checked)
                chart1.Series[0].Enabled = true;
            else
                chart1.Series[0].Enabled = false;
        }

        private void checkBox3_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox3.Checked)
                chart1.Series[1].Enabled = true;
            else
                chart1.Series[1].Enabled = false;
        }

        private void checkBox4_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox4.Checked)
                chart1.Series[2].Enabled = true;
            else
                chart1.Series[2].Enabled = false;
        }

        private void button1_Click_1(object sender, EventArgs e)
        {
            short Trmm = Decimal.ToInt16(numericUpDown1.Value);

            TrmMass[0] = 0xff;
            TrmMass[1] = 0xff;
            TrmMass[2] = 3;//N
            TrmMass[3] = SET_KGyro;
            TrmMass[4] = (byte)Trmm;
            TrmMass[5] = (byte)(Trmm >> 8);
            TrmMass[6] = CalcCheckSummTrm();
            Trm();


        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void numericUpDown1_ValueChanged(object sender, EventArgs e)
        {

        }

        private void numericUpDown2_ValueChanged(object sender, EventArgs e)
        {
            short Trmm = Decimal.ToInt16(numericUpDown2.Value);

            TrmMass[0] = 0xff;
            TrmMass[1] = 0xff;
            TrmMass[2] = 3;//N
            TrmMass[3] = SET_PWM_Motor_L1;
            TrmMass[4] = (byte)Trmm;
            TrmMass[5] = (byte)(Trmm >> 8);
            TrmMass[6] = CalcCheckSummTrm();
            Trm();


        }

        private void numericUpDown3_ValueChanged(object sender, EventArgs e)
        {
            short Trmm = Decimal.ToInt16(numericUpDown3.Value);

            TrmMass[0] = 0xff;
            TrmMass[1] = 0xff;
            TrmMass[2] = 3;//N
            TrmMass[3] = SET_PWM_Motor_L2;
            TrmMass[4] = (byte)Trmm;
            TrmMass[5] = (byte)(Trmm >> 8);
            TrmMass[6] = CalcCheckSummTrm();
            Trm();

        }

        private void numericUpDown4_ValueChanged(object sender, EventArgs e)
        {
            short Trmm = Decimal.ToInt16(numericUpDown4.Value);

            TrmMass[0] = 0xff;
            TrmMass[1] = 0xff;
            TrmMass[2] = 3;//N
            TrmMass[3] = SET_PWM_Motor_L3;
            TrmMass[4] = (byte)Trmm;
            TrmMass[5] = (byte)(Trmm >> 8);
            TrmMass[6] = CalcCheckSummTrm();
            Trm();

        }

        private void numericUpDown5_ValueChanged(object sender, EventArgs e)
        {
            short Trmm = Decimal.ToInt16(numericUpDown5.Value);

            TrmMass[0] = 0xff;
            TrmMass[1] = 0xff;
            TrmMass[2] = 3;//N
            TrmMass[3] = SET_PWM_Motor_L4;
            TrmMass[4] = (byte)Trmm;
            TrmMass[5] = (byte)(Trmm >> 8);
            TrmMass[6] = CalcCheckSummTrm();
            Trm();

        }

        private void numericUpDown6_ValueChanged(object sender, EventArgs e)
        {
            short Trmm = Decimal.ToInt16(numericUpDown6.Value);

            TrmMass[0] = 0xff;
            TrmMass[1] = 0xff;
            TrmMass[2] = 3;//N
            TrmMass[3] = SET_PWM_Motor_R1;
            TrmMass[4] = (byte)Trmm;
            TrmMass[5] = (byte)(Trmm >> 8);
            TrmMass[6] = CalcCheckSummTrm();
            Trm();

        }

        private void numericUpDown7_ValueChanged(object sender, EventArgs e)
        {
            short Trmm = Decimal.ToInt16(numericUpDown7.Value);

            TrmMass[0] = 0xff;
            TrmMass[1] = 0xff;
            TrmMass[2] = 3;//N
            TrmMass[3] = SET_PWM_Motor_R2;
            TrmMass[4] = (byte)Trmm;
            TrmMass[5] = (byte)(Trmm >> 8);
            TrmMass[6] = CalcCheckSummTrm();
            Trm();

        }

        private void numericUpDown8_ValueChanged(object sender, EventArgs e)
        {
            short Trmm = Decimal.ToInt16(numericUpDown8.Value);

            TrmMass[0] = 0xff;
            TrmMass[1] = 0xff;
            TrmMass[2] = 3;//N
            TrmMass[3] = SET_PWM_Motor_R3;
            TrmMass[4] = (byte)Trmm;
            TrmMass[5] = (byte)(Trmm >> 8);
            TrmMass[6] = CalcCheckSummTrm();
            Trm();

        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            if (WaitForCfm == 1)
            {
                Trm();
            }
        }

        private void button11_Click(object sender, EventArgs e)
        {
            TrmMass[0] = 0xff;
            TrmMass[1] = 0xff;
            TrmMass[2] = 1;//N
            TrmMass[3] = GET_OPTIONS;
            TrmMass[4] = CalcCheckSummTrm();
            Trm();
        }

        private void button12_Click(object sender, EventArgs e)
        {
            TrmMass[0] = 0xff;
            TrmMass[1] = 0xff;
            TrmMass[2] = 1;//N
            TrmMass[3] = WRITE_OPTIONS;
            TrmMass[4] = CalcCheckSummTrm();
            Trm();

        }

        private void button13_Click(object sender, EventArgs e)
        {
            TrmMass[0] = 0xff;
            TrmMass[1] = 0xff;
            TrmMass[2] = 1;//N
            TrmMass[3] = CLEAR_ALL;
            TrmMass[4] = CalcCheckSummTrm();
            Trm();
            chart1.Series[0].Points.Clear();
            chart1.Series[1].Points.Clear();
            chart1.Series[2].Points.Clear();

        }

        private void numericUpDown9_ValueChanged(object sender, EventArgs e)
        {
            short Trmm = Decimal.ToInt16(numericUpDown9.Value);

            TrmMass[0] = 0xff;
            TrmMass[1] = 0xff;
            TrmMass[2] = 3;//N
            TrmMass[3] = SET_PWM_Motor_R4;
            TrmMass[4] = (byte)Trmm;
            TrmMass[5] = (byte)(Trmm >> 8);
            TrmMass[6] = CalcCheckSummTrm();
            Trm();


        }

        void Trm()
        {
            if (IPaddr != null)
            {
                WaitForCfm = 1;

                ep = new IPEndPoint(IPaddr, TrmPort);

                byte[] sendbuf = new byte[TrmMass[2] + 4];
                int i;
                for (i = 0; i < TrmMass[2] + 4; i++)
                    sendbuf[i] = TrmMass[i];

                s.SendTo(sendbuf, ep);
            }

        }


        private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            int i;
            int listenPort = 51722;

            UdpClient listener = new UdpClient(listenPort);
            IPEndPoint EP = new IPEndPoint(IPAddress.Any, listenPort);

            //      listener.Client.ReceiveTimeout = 1;

            while (true)
            {
                byte[] bytes = listener.Receive(ref EP);

                for (i = 0; i < bytes.Length - 2; i++)
                    PacketRec[i] = bytes[i + 2];

                IPaddr = EP.Address;

                if (backgroundWorker1.CancellationPending)
                {
                    e.Cancel = true;
                    break;
                }

                IPStr = EP.Address.ToString();


                if (CalcCheckSummRec() == PacketRec[PacketRec[0] + 1])
                    ProcessPacket();

            }


        }

        private void backgroundWorker1_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            label2.Text = IPStr;

            switch (e.ProgressPercentage)
            {
                case TRM_DATA_PACKET:

                        label38.Text = String.Format("{0:0.000}", Tilt1XList[Tilt1XList.Count - 1]);
                        label33.Text = String.Format("{0:0.000}", Tilt1YList[Tilt1YList.Count - 1]);
                        label31.Text = GyroHipJointRList[GyroHipJointRList.Count - 1].ToString();
                        label46.Text = GyroHipJointLList[GyroHipJointLList.Count - 1].ToString();

                        label894.Text = String.Format("{0:0.000}", Tilt2XList[Tilt2XList.Count - 1]);
                        label7.Text = String.Format("{0:0.000}", Tilt2YList[Tilt2YList.Count - 1]);
                        label9.Text = GyroHipLList[GyroHipLList.Count - 1].ToString();

                        label18.Text = String.Format("{0:0.000}", Tilt3XList[Tilt3XList.Count - 1]);
                        label13.Text = String.Format("{0:0.000}", Tilt3YList[Tilt3YList.Count - 1]);
                        label11.Text = GyroLowerlegLList[GyroLowerlegLList.Count - 1].ToString();

                        label28.Text = String.Format("{0:0.000}", Tilt4XList[Tilt4XList.Count - 1]);
                        label23.Text = String.Format("{0:0.000}", Tilt4YList[Tilt4YList.Count - 1]);
                        label21.Text = GyroFootLList[GyroFootLList.Count - 1].ToString();

                        label68.Text = String.Format("{0:0.000}", Tilt5XList[Tilt5XList.Count - 1]);
                        label63.Text = String.Format("{0:0.000}", Tilt5YList[Tilt5YList.Count - 1]);
                        label61.Text = GyroHipRList[GyroHipRList.Count - 1].ToString();

                        label58.Text = String.Format("{0:0.000}", Tilt6XList[Tilt6XList.Count - 1]);
                        label53.Text = String.Format("{0:0.000}", Tilt6YList[Tilt6YList.Count - 1]);
                        label51.Text = GyroLowerlegRList[Tilt1XList.Count - 1].ToString();

                        label48.Text = String.Format("{0:0.000}", Tilt7XList[Tilt7XList.Count - 1]);
                        label43.Text = String.Format("{0:0.000}", Tilt7YList[Tilt7YList.Count - 1]);
                        label41.Text = GyroFootRList[GyroFootRList.Count - 1].ToString();



                    label66.Text = String.Format("{0:0.000}", YHipJointRList[YHipJointRList.Count - 1]);
                    label56.Text = String.Format("{0:0.000}", YHipJointLList[YHipJointLList.Count - 1]);
                    label85.Text = String.Format("{0:0.000}", XHipRList[XHipRList.Count - 1]);
                    label83.Text = String.Format("{0:0.000}", XHipLList[XHipLList.Count - 1]);

                    label89.Text = String.Format("{0:0.000}", XLowerLegRList[XLowerLegRList.Count - 1]);
                    label87.Text = String.Format("{0:0.000}", XLowerLegLList[XLowerLegLList.Count - 1]);
                    label93.Text = String.Format("{0:0.000}", XFootRList[XFootRList.Count - 1]);
                    label91.Text = String.Format("{0:0.000}", XFootLList[XFootLList.Count - 1]);





                    label81.Text = KGyro1.ToString();

                    int i;
                    for (i = 0; i < Tilt1XList.Count; i++)
                    {
                        if (!StopCharts)
                        {
                            if (checkBox2.Checked)
                            {
                                if ((i < Tilt1XList.Count)&&(i< CntSamplesList.Count))
                                {
                                    switch (comboBox1.SelectedIndex + 1)
                                    {
                                        case 1:
                                            chart1.Series[0].Points.AddXY(CntSamplesList[i], Tilt1XList[i]);
                                            break;
                                        case 2:
                                            chart1.Series[0].Points.AddXY(CntSamplesList[i], Tilt1YList[i]);
                                            break;
                                        case 3:
                                            chart1.Series[0].Points.AddXY(CntSamplesList[i], Gyro1List[i]);
                                            break;
                                        case 4:
                                            chart1.Series[0].Points.AddXY(CntSamplesList[i], Tilt2XList[i]);
                                            break;
                                        case 5:
                                            chart1.Series[0].Points.AddXY(CntSamplesList[i], Tilt2YList[i]);
                                            break;
                                        case 6:
                                            chart1.Series[0].Points.AddXY(CntSamplesList[i], Gyro2List[i]);
                                            break;
                                        case 7:
                                            chart1.Series[0].Points.AddXY(CntSamplesList[i], Tilt3XList[i]);
                                            break;
                                        case 8:
                                            chart1.Series[0].Points.AddXY(CntSamplesList[i], Tilt3YList[i]);
                                            break;
                                        case 9:
                                            chart1.Series[0].Points.AddXY(CntSamplesList[i], Gyro3List[i]);
                                            break;
                                        case 10:
                                            chart1.Series[0].Points.AddXY(CntSamplesList[i], Tilt4XList[i]);
                                            break;
                                        case 11:
                                            chart1.Series[0].Points.AddXY(CntSamplesList[i], Tilt4YList[i]);
                                            break;
                                        case 12:
                                            chart1.Series[0].Points.AddXY(CntSamplesList[i], Gyro4List[i]);
                                            break;
                                        case 13:
                                            chart1.Series[0].Points.AddXY(CntSamplesList[i], Tilt5XList[i]);
                                            break;
                                        case 14:
                                            chart1.Series[0].Points.AddXY(CntSamplesList[i], Tilt5YList[i]);
                                            break;
                                        case 15:
                                            chart1.Series[0].Points.AddXY(CntSamplesList[i], Gyro5List[i]);
                                            break;
                                        case 16:
                                            chart1.Series[0].Points.AddXY(CntSamplesList[i], Tilt6XList[i]);
                                            break;
                                        case 17:
                                            chart1.Series[0].Points.AddXY(CntSamplesList[i], Tilt6YList[i]);
                                            break;
                                        case 18:
                                            chart1.Series[0].Points.AddXY(CntSamplesList[i], Gyro6List[i]);
                                            break;
                                        case 19:
                                            chart1.Series[0].Points.AddXY(CntSamplesList[i], Tilt7XList[i]);
                                            break;
                                        case 20:
                                            chart1.Series[0].Points.AddXY(CntSamplesList[i], Tilt7YList[i]);
                                            break;
                                        case 21:
                                            chart1.Series[0].Points.AddXY(CntSamplesList[i], Gyro7List[i]);
                                            break;
                                        case 22:
                                            chart1.Series[0].Points.AddXY(CntSamplesList[i], GyroHipJointRList[i]);
                                            break;
                                        case 23:
                                            chart1.Series[0].Points.AddXY(CntSamplesList[i], GyroHipJointLList[i]);
                                            break;
                                        case 24:
                                            chart1.Series[0].Points.AddXY(CntSamplesList[i], GyroHipRList[i]);
                                            break;
                                        case 25:
                                            chart1.Series[0].Points.AddXY(CntSamplesList[i], GyroHipLList[i]);
                                            break;
                                        case 26:
                                            chart1.Series[0].Points.AddXY(CntSamplesList[i], GyroLowerlegRList[i]);
                                            break;
                                        case 27:
                                            chart1.Series[0].Points.AddXY(CntSamplesList[i], GyroLowerlegLList[i]);
                                            break;
                                        case 28:
                                            chart1.Series[0].Points.AddXY(CntSamplesList[i], GyroFootRList[i]);
                                            break;
                                        case 29:
                                            chart1.Series[0].Points.AddXY(CntSamplesList[i], GyroFootLList[i]);
                                            break;
                                        case 30:
                                            chart1.Series[0].Points.AddXY(CntSamplesList[i], YHipJointRList[i]);
                                            break;
                                        case 31:
                                            chart1.Series[0].Points.AddXY(CntSamplesList[i], YHipJointLList[i]);
                                            break;
                                        case 32:
                                            chart1.Series[0].Points.AddXY(CntSamplesList[i], XHipRList[i]);
                                            break;
                                        case 33:
                                            chart1.Series[0].Points.AddXY(CntSamplesList[i], XHipLList[i]);
                                            break;
                                        case 34:
                                            chart1.Series[0].Points.AddXY(CntSamplesList[i], XLowerLegRList[i]);
                                            break;
                                        case 35:
                                            chart1.Series[0].Points.AddXY(CntSamplesList[i], XLowerLegLList[i]);
                                            break;
                                        case 36:
                                            chart1.Series[0].Points.AddXY(CntSamplesList[i], XFootRList[i]);
                                            break;
                                        case 37:
                                            chart1.Series[0].Points.AddXY(CntSamplesList[i], XFootLList[i]);
                                            break;









                                    }
                                }
                            }


                            if (checkBox3.Checked)
                            {
                                if (i < Tilt1XList.Count)
                                {
                                    switch (comboBox2.SelectedIndex + 1)
                                    {
                                        case 1:
                                            chart1.Series[1].Points.AddXY(CntSamplesList[i], Tilt1XList[i]);
                                            break;
                                        case 2:
                                            chart1.Series[1].Points.AddXY(CntSamplesList[i], Tilt1YList[i]);
                                            break;
                                        case 3:
                                            chart1.Series[1].Points.AddXY(CntSamplesList[i], Gyro1List[i]);
                                            break;
                                        case 4:
                                            chart1.Series[1].Points.AddXY(CntSamplesList[i], Tilt2XList[i]);
                                            break;
                                        case 5:
                                            chart1.Series[1].Points.AddXY(CntSamplesList[i], Tilt2YList[i]);
                                            break;
                                        case 6:
                                            chart1.Series[1].Points.AddXY(CntSamplesList[i], Gyro2List[i]);
                                            break;
                                        case 7:
                                            chart1.Series[1].Points.AddXY(CntSamplesList[i], Tilt3XList[i]);
                                            break;
                                        case 8:
                                            chart1.Series[1].Points.AddXY(CntSamplesList[i], Tilt3YList[i]);
                                            break;
                                        case 9:
                                            chart1.Series[1].Points.AddXY(CntSamplesList[i], Gyro3List[i]);
                                            break;
                                        case 10:
                                            chart1.Series[1].Points.AddXY(CntSamplesList[i], Tilt4XList[i]);
                                            break;
                                        case 11:
                                            chart1.Series[1].Points.AddXY(CntSamplesList[i], Tilt4YList[i]);
                                            break;
                                        case 12:
                                            chart1.Series[1].Points.AddXY(CntSamplesList[i], Gyro4List[i]);
                                            break;
                                        case 13:
                                            chart1.Series[1].Points.AddXY(CntSamplesList[i], Tilt5XList[i]);
                                            break;
                                        case 14:
                                            chart1.Series[1].Points.AddXY(CntSamplesList[i], Tilt5YList[i]);
                                            break;
                                        case 15:
                                            chart1.Series[1].Points.AddXY(CntSamplesList[i], Gyro5List[i]);
                                            break;
                                        case 16:
                                            chart1.Series[1].Points.AddXY(CntSamplesList[i], Tilt6XList[i]);
                                            break;
                                        case 17:
                                            chart1.Series[1].Points.AddXY(CntSamplesList[i], Tilt6YList[i]);
                                            break;
                                        case 18:
                                            chart1.Series[1].Points.AddXY(CntSamplesList[i], Gyro6List[i]);
                                            break;
                                        case 19:
                                            chart1.Series[1].Points.AddXY(CntSamplesList[i], Tilt7XList[i]);
                                            break;
                                        case 20:
                                            chart1.Series[1].Points.AddXY(CntSamplesList[i], Tilt7YList[i]);
                                            break;
                                        case 21:
                                            chart1.Series[1].Points.AddXY(CntSamplesList[i], Gyro7List[i]);
                                            break;
                                        case 22:
                                            chart1.Series[1].Points.AddXY(CntSamplesList[i], GyroHipJointRList[i]);
                                            break;
                                        case 23:
                                            chart1.Series[1].Points.AddXY(CntSamplesList[i], GyroHipJointLList[i]);
                                            break;
                                        case 24:
                                            chart1.Series[1].Points.AddXY(CntSamplesList[i], GyroHipRList[i]);
                                            break;
                                        case 25:
                                            chart1.Series[1].Points.AddXY(CntSamplesList[i], GyroHipLList[i]);
                                            break;
                                        case 26:
                                            chart1.Series[1].Points.AddXY(CntSamplesList[i], GyroLowerlegRList[i]);
                                            break;
                                        case 27:
                                            chart1.Series[1].Points.AddXY(CntSamplesList[i], GyroLowerlegLList[i]);
                                            break;
                                        case 28:
                                            chart1.Series[1].Points.AddXY(CntSamplesList[i], GyroFootRList[i]);
                                            break;
                                        case 29:
                                            chart1.Series[1].Points.AddXY(CntSamplesList[i], GyroFootLList[i]);
                                            break;
                                        case 30:
                                            chart1.Series[1].Points.AddXY(CntSamplesList[i], YHipJointRList[i]);
                                            break;
                                        case 31:
                                            chart1.Series[1].Points.AddXY(CntSamplesList[i], YHipJointLList[i]);
                                            break;
                                        case 32:
                                            chart1.Series[1].Points.AddXY(CntSamplesList[i], XHipRList[i]);
                                            break;
                                        case 33:
                                            chart1.Series[1].Points.AddXY(CntSamplesList[i], XHipLList[i]);
                                            break;
                                        case 34:
                                            chart1.Series[1].Points.AddXY(CntSamplesList[i], XLowerLegRList[i]);
                                            break;
                                        case 35:
                                            chart1.Series[1].Points.AddXY(CntSamplesList[i], XLowerLegLList[i]);
                                            break;
                                        case 36:
                                            chart1.Series[1].Points.AddXY(CntSamplesList[i], XFootRList[i]);
                                            break;
                                        case 37:
                                            chart1.Series[1].Points.AddXY(CntSamplesList[i], XFootLList[i]);
                                            break;



                                    }
                                }
                            }



                            if (checkBox4.Checked)
                            {
                                if (i < Tilt1XList.Count)
                                {
                                    switch (comboBox3.SelectedIndex + 1)
                                    {
                                        case 1:
                                            chart1.Series[2].Points.AddXY(CntSamplesList[i], Tilt1XList[i]);
                                            break;
                                        case 2:
                                            chart1.Series[2].Points.AddXY(CntSamplesList[i], Tilt1YList[i]);
                                            break;
                                        case 3:
                                            chart1.Series[2].Points.AddXY(CntSamplesList[i], Gyro1List[i]);
                                            break;
                                        case 4:
                                            chart1.Series[2].Points.AddXY(CntSamplesList[i], Tilt2XList[i]);
                                            break;
                                        case 5:
                                            chart1.Series[2].Points.AddXY(CntSamplesList[i], Tilt2YList[i]);
                                            break;
                                        case 6:
                                            chart1.Series[2].Points.AddXY(CntSamplesList[i], Gyro2List[i]);
                                            break;
                                        case 7:
                                            chart1.Series[2].Points.AddXY(CntSamplesList[i], Tilt3XList[i]);
                                            break;
                                        case 8:
                                            chart1.Series[2].Points.AddXY(CntSamplesList[i], Tilt3YList[i]);
                                            break;
                                        case 9:
                                            chart1.Series[2].Points.AddXY(CntSamplesList[i], Gyro3List[i]);
                                            break;
                                        case 10:
                                            chart1.Series[2].Points.AddXY(CntSamplesList[i], Tilt4XList[i]);
                                            break;
                                        case 11:
                                            chart1.Series[2].Points.AddXY(CntSamplesList[i], Tilt4YList[i]);
                                            break;
                                        case 12:
                                            chart1.Series[2].Points.AddXY(CntSamplesList[i], Gyro4List[i]);
                                            break;
                                        case 13:
                                            chart1.Series[2].Points.AddXY(CntSamplesList[i], Tilt5XList[i]);
                                            break;
                                        case 14:
                                            chart1.Series[2].Points.AddXY(CntSamplesList[i], Tilt5YList[i]);
                                            break;
                                        case 15:
                                            chart1.Series[2].Points.AddXY(CntSamplesList[i], Gyro5List[i]);
                                            break;
                                        case 16:
                                            chart1.Series[2].Points.AddXY(CntSamplesList[i], Tilt6XList[i]);
                                            break;
                                        case 17:
                                            chart1.Series[2].Points.AddXY(CntSamplesList[i], Tilt6YList[i]);
                                            break;
                                        case 18:
                                            chart1.Series[2].Points.AddXY(CntSamplesList[i], Gyro6List[i]);
                                            break;
                                        case 19:
                                            chart1.Series[2].Points.AddXY(CntSamplesList[i], Tilt7XList[i]);
                                            break;
                                        case 20:
                                            chart1.Series[2].Points.AddXY(CntSamplesList[i], Tilt7YList[i]);
                                            break;
                                        case 21:
                                            chart1.Series[2].Points.AddXY(CntSamplesList[i], Gyro7List[i]);
                                            break;
                                        case 22:
                                            chart1.Series[2].Points.AddXY(CntSamplesList[i], GyroHipJointRList[i]);
                                            break;
                                        case 23:
                                            chart1.Series[2].Points.AddXY(CntSamplesList[i], GyroHipJointLList[i]);
                                            break;
                                        case 24:
                                            chart1.Series[2].Points.AddXY(CntSamplesList[i], GyroHipRList[i]);
                                            break;
                                        case 25:
                                            chart1.Series[2].Points.AddXY(CntSamplesList[i], GyroHipLList[i]);
                                            break;
                                        case 26:
                                            chart1.Series[2].Points.AddXY(CntSamplesList[i], GyroLowerlegRList[i]);
                                            break;
                                        case 27:
                                            chart1.Series[2].Points.AddXY(CntSamplesList[i], GyroLowerlegLList[i]);
                                            break;
                                        case 28:
                                            chart1.Series[2].Points.AddXY(CntSamplesList[i], GyroFootRList[i]);
                                            break;
                                        case 29:
                                            chart1.Series[2].Points.AddXY(CntSamplesList[i], GyroFootLList[i]);
                                            break;
                                        case 30:
                                            chart1.Series[2].Points.AddXY(CntSamplesList[i], YHipJointRList[i]);
                                            break;
                                        case 31:
                                            chart1.Series[2].Points.AddXY(CntSamplesList[i], YHipJointLList[i]);
                                            break;
                                        case 32:
                                            chart1.Series[2].Points.AddXY(CntSamplesList[i], XHipRList[i]);
                                            break;
                                        case 33:
                                            chart1.Series[2].Points.AddXY(CntSamplesList[i], XHipLList[i]);
                                            break;
                                        case 34:
                                            chart1.Series[2].Points.AddXY(CntSamplesList[i], XLowerLegRList[i]);
                                            break;
                                        case 35:
                                            chart1.Series[2].Points.AddXY(CntSamplesList[i], XLowerLegLList[i]);
                                            break;
                                        case 36:
                                            chart1.Series[2].Points.AddXY(CntSamplesList[i], XFootRList[i]);
                                            break;
                                        case 37:
                                            chart1.Series[2].Points.AddXY(CntSamplesList[i], XFootLList[i]);
                                            break;





                                    }
                                }
                            }
                        }

                        if ((chart1.Series[0].Points.Count >= 1000) && !checkBox1.Checked)
                            chart1.Series[0].Points.RemoveAt(0);
                        if ((chart1.Series[1].Points.Count >= 1000) && !checkBox1.Checked)
                            chart1.Series[1].Points.RemoveAt(0);
                        if ((chart1.Series[2].Points.Count >= 1000) && !checkBox1.Checked)
                            chart1.Series[2].Points.RemoveAt(0);

                        if (!backgroundWorker1.CancellationPending)
                            chart1.ChartAreas[0].RecalculateAxesScale();
                    }

                    Tilt1XList.Clear();
                    Tilt1YList.Clear();
                    Gyro1List.Clear();
                    Tilt2XList.Clear();
                    Tilt2YList.Clear();
                    Gyro2List.Clear();
                    Tilt3XList.Clear();
                    Tilt3YList.Clear();
                    Gyro3List.Clear();
                    Tilt4XList.Clear();
                    Tilt4YList.Clear();
                    Gyro4List.Clear();
                    Tilt5XList.Clear();
                    Tilt5YList.Clear();
                    Gyro5List.Clear();
                    Tilt6XList.Clear();
                    Tilt6YList.Clear();
                    Gyro6List.Clear();
                    Tilt7XList.Clear();
                    Tilt7YList.Clear();
                    Gyro7List.Clear();
                    GyroHipJointRList.Clear();
                    GyroHipJointLList.Clear();
                    GyroHipRList.Clear();
                    GyroHipLList.Clear();
                    GyroLowerlegRList.Clear();
                    GyroLowerlegLList.Clear();
                    GyroFootRList.Clear();
                    GyroFootLList.Clear();
                    CntSamplesList.Clear();
                    YHipJointRList.Clear();
                    YHipJointLList.Clear();
                    XHipRList.Clear();
                    XHipLList.Clear();
                    XLowerLegRList.Clear();
                    XLowerLegLList.Clear();
                    XFootRList.Clear();
                    XFootLList.Clear();




                    break;
                default:
                    break;

            }
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            backgroundWorker1.CancelAsync();
            GC.Collect();
        }

        private void numericUpDown42_ValueChanged(object sender, EventArgs e)
        {

            short Trmm = Decimal.ToInt16(numericUpDown42.Value);

            TrmMass[0] = 0xff;
            TrmMass[1] = 0xff;
            TrmMass[2] = 3;//N
            TrmMass[3] = SET_Addr;
            TrmMass[4] = (byte)Trmm;
            TrmMass[5] = (byte)(Trmm >> 8);
            TrmMass[6] = CalcCheckSummTrm();
            Trm();
        }

        private void button9_Click(object sender, EventArgs e)
        {
            short Trmm = Decimal.ToInt16(numericUpDown42.Value);

            TrmMass[0] = 0xff;
            TrmMass[1] = 0xff;
            TrmMass[2] = 3;//N
            TrmMass[3] = SET_Addr;
            TrmMass[4] = (byte)Trmm;
            TrmMass[5] = (byte)(Trmm >> 8);
            TrmMass[6] = CalcCheckSummTrm();
            Trm();

        }

        private void button2_Click(object sender, EventArgs e)
        {
            TrmMass[0] = 0xff;
            TrmMass[1] = 0xff;
            TrmMass[2] = 1;//N
            TrmMass[3] = CALIBR1;
            TrmMass[4] = CalcCheckSummTrm();
            Trm();

        }

        private void button3_Click(object sender, EventArgs e)
        {
            TrmMass[0] = 0xff;
            TrmMass[1] = 0xff;
            TrmMass[2] = 1;//N
            TrmMass[3] = CALIBR2;
            TrmMass[4] = CalcCheckSummTrm();
            Trm();

        }

        private void button4_Click(object sender, EventArgs e)
        {
            TrmMass[0] = 0xff;
            TrmMass[1] = 0xff;
            TrmMass[2] = 1;//N
            TrmMass[3] = CALIBR3;
            TrmMass[4] = CalcCheckSummTrm();
            Trm();

        }

        private void button5_Click(object sender, EventArgs e)
        {
            TrmMass[0] = 0xff;
            TrmMass[1] = 0xff;
            TrmMass[2] = 1;//N
            TrmMass[3] = CALIBR4;
            TrmMass[4] = CalcCheckSummTrm();
            Trm();

        }

        private void button6_Click(object sender, EventArgs e)
        {
            TrmMass[0] = 0xff;
            TrmMass[1] = 0xff;
            TrmMass[2] = 1;//N
            TrmMass[3] = CALIBR5;
            TrmMass[4] = CalcCheckSummTrm();
            Trm();

        }

        private void button7_Click(object sender, EventArgs e)
        {
            TrmMass[0] = 0xff;
            TrmMass[1] = 0xff;
            TrmMass[2] = 1;//N
            TrmMass[3] = CALIBR6;
            TrmMass[4] = CalcCheckSummTrm();
            Trm();

        }

        private void button8_Click(object sender, EventArgs e)
        {
            TrmMass[0] = 0xff;
            TrmMass[1] = 0xff;
            TrmMass[2] = 1;//N
            TrmMass[3] = CALIBR7;
            TrmMass[4] = CalcCheckSummTrm();
            Trm();

        }

        unsafe void ProcessPacket()
        {
            CntUDPArray = 2;

            switch (PacketRec[1])
            {
                case TRM_DATA_PACKET:



                    fixed (byte* p = &PacketRec[CntUDPArray])
                    { CntSamplesList.Add(*(uint*)p); }
                    CntUDPArray = CntUDPArray + sizeof(uint);


                    GetUDPArrayFloat(Tilt1XList);
                    GetUDPArrayFloat(Tilt1YList);
                    GetUDPArrayFloat(Gyro1List);
                    GetUDPArrayFloat(Tilt2XList);
                    GetUDPArrayFloat(Tilt2YList);
                    GetUDPArrayFloat(Gyro2List);
                    GetUDPArrayFloat(Tilt3XList);
                    GetUDPArrayFloat(Tilt3YList);
                    GetUDPArrayFloat(Gyro3List);

                    GetUDPArrayFloat(Tilt4XList);
                    GetUDPArrayFloat(Tilt4YList);
                    GetUDPArrayFloat(Gyro4List);
                    GetUDPArrayFloat(Tilt5XList);
                    GetUDPArrayFloat(Tilt5YList);
                    GetUDPArrayFloat(Gyro5List);
                    GetUDPArrayFloat(Tilt6XList);
                    GetUDPArrayFloat(Tilt6YList);
                    GetUDPArrayFloat(Gyro6List);
                    GetUDPArrayFloat(Tilt7XList);
                    GetUDPArrayFloat(Tilt7YList);
                    GetUDPArrayFloat(Gyro7List);

                    fixed (byte* p = &PacketRec[CntUDPArray])
                    { KGyro1 = *(ushort*)p; }
                    CntUDPArray = CntUDPArray + sizeof(ushort);

                    GetUDPArrayFloat(GyroHipJointRList);
                    GetUDPArrayFloat(GyroHipJointLList);
                    GetUDPArrayFloat(GyroHipRList);
                    GetUDPArrayFloat(GyroHipLList);
                    GetUDPArrayFloat(GyroLowerlegRList);
                    GetUDPArrayFloat(GyroLowerlegLList);
                    GetUDPArrayFloat(GyroFootRList);
                    GetUDPArrayFloat(GyroFootLList);

                    GetUDPArrayFloat(YHipJointRList);
                    GetUDPArrayFloat(YHipJointLList);
                    GetUDPArrayFloat(XHipRList);
                    GetUDPArrayFloat(XHipLList);
                    GetUDPArrayFloat(XLowerLegRList);
                    GetUDPArrayFloat(XLowerLegLList);
                    GetUDPArrayFloat(XFootRList);
                    GetUDPArrayFloat(XFootLList);



                    if (Gyro7List.Count == ListCount)
                    {
                        backgroundWorker1.ReportProgress(TRM_DATA_PACKET);
                        while (Gyro7List.Count != 0) ;
                    }



                    break;
                case TRM_CFM_PACKET:
                    WaitForCfm = 0;
                    break;

            }

        }

       
        unsafe void GetUDPArrayFloat(List<float> Lst)
        {
            fixed (byte* p = &PacketRec[CntUDPArray])
            {
                Lst.Add(*(float*)p);
            }
            CntUDPArray = CntUDPArray + sizeof(float);
        }
    }

}