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

        float Tilt1X, Tilt1Y, Gyro1;
        float Tilt2X, Tilt2Y, Gyro2;
        float Tilt3X, Tilt3Y, Gyro3;
        float Tilt4X, Tilt4Y, Gyro4;
        float Tilt5X, Tilt5Y, Gyro5;
        float Tilt6X, Tilt6Y, Gyro6;
        float Tilt7X, Tilt7Y, Gyro7;
        ushort KGyro1;

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

        const byte DATA_PACKET = 1;
        const byte DATA_PACKETTst = 2;

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
                case DATA_PACKET:

                    label38.Text = String.Format("{0:0.000}", Tilt1XList[Tilt1XList.Count - 1]);
                    label33.Text = String.Format("{0:0.000}", Tilt1YList[Tilt1XList.Count - 1]);
                    label31.Text = Gyro1List[Tilt1XList.Count - 1].ToString();

                    label894.Text = String.Format("{0:0.000}", Tilt2XList[Tilt1XList.Count - 1]);  //Tilt2XList[Tilt1XList.Count - 1].ToString();
                    label7.Text = String.Format("{0:0.000}", Tilt2YList[Tilt1XList.Count - 1]);//.ToString();
                    label9.Text = Gyro2List[Tilt1XList.Count - 1].ToString();

                    label18.Text = String.Format("{0:0.000}", Tilt3XList[Tilt1XList.Count - 1]);
                    label13.Text = String.Format("{0:0.000}", Tilt3YList[Tilt1XList.Count - 1]);
                    label11.Text = Gyro3List[Tilt1XList.Count - 1].ToString();

                    label28.Text = String.Format("{0:0.000}", Tilt4XList[Tilt1XList.Count - 1]);
                    label23.Text = String.Format("{0:0.000}", Tilt4YList[Tilt1XList.Count - 1]);
                    label21.Text = Gyro4List[Tilt1XList.Count - 1].ToString();

                    label68.Text = String.Format("{0:0.000}", Tilt5XList[Tilt1XList.Count - 1]);
                    label63.Text = String.Format("{0:0.000}", Tilt5YList[Tilt1XList.Count - 1]);
                    label61.Text = Gyro5List[Tilt1XList.Count - 1].ToString();

                    label58.Text = String.Format("{0:0.000}", Tilt6XList[Tilt1XList.Count - 1]);
                    label53.Text = String.Format("{0:0.000}", Tilt6YList[Tilt1XList.Count - 1]);
                    label51.Text = Gyro6.ToString();

                    label48.Text = String.Format("{0:0.000}", Tilt7XList[Tilt1XList.Count - 1]);
                    label43.Text = String.Format("{0:0.000}", Tilt7YList[Tilt1XList.Count - 1]);
                    label41.Text = Gyro7List[Tilt1XList.Count - 1].ToString();

                    label81.Text = KGyro1.ToString();


                    int i;
                    for (i = 0; i < Tilt1XList.Count; i++)
                    {
                        if (!StopCharts)
                        {
                            if (checkBox2.Checked)
                            {
                                switch (comboBox1.SelectedIndex + 1)
                                {
                                    case 1:
                                        chart1.Series[0].Points.Add(Tilt1XList[i]);
                                        break;
                                    case 2:
                                        chart1.Series[0].Points.Add(Tilt1YList[i]);
                                        break;
                                    case 3:
                                        chart1.Series[0].Points.Add(Gyro1List[i]);
                                        break;
                                    case 4:
                                        chart1.Series[0].Points.Add(Tilt2XList[i]);
                                        break;
                                    case 5:
                                        chart1.Series[0].Points.Add(Tilt2YList[i]);
                                        break;
                                    case 6:
                                        chart1.Series[0].Points.Add(Gyro2List[i]);
                                        break;
                                    case 7:
                                        chart1.Series[0].Points.Add(Tilt3XList[i]);
                                        break;
                                    case 8:
                                        chart1.Series[0].Points.Add(Tilt3YList[i]);
                                        break;
                                    case 9:
                                        chart1.Series[0].Points.Add(Gyro3List[i]);
                                        break;
                                    case 10:
                                        chart1.Series[0].Points.Add(Tilt4XList[i]);
                                        break;
                                    case 11:
                                        chart1.Series[0].Points.Add(Tilt4YList[i]);
                                        break;
                                    case 12:
                                        chart1.Series[0].Points.Add(Gyro4List[i]);
                                        break;
                                    case 13:
                                        chart1.Series[0].Points.Add(Tilt5XList[i]);
                                        break;
                                    case 14:
                                        chart1.Series[0].Points.Add(Tilt5YList[i]);
                                        break;
                                    case 15:
                                        chart1.Series[0].Points.Add(Gyro5List[i]);
                                        break;
                                    case 16:
                                        chart1.Series[0].Points.Add(Tilt6XList[i]);
                                        break;
                                    case 17:
                                        chart1.Series[0].Points.Add(Tilt6YList[i]);
                                        break;
                                    case 18:
                                        chart1.Series[0].Points.Add(Gyro6List[i]);
                                        break;
                                    case 19:
                                        chart1.Series[0].Points.Add(Tilt7XList[i]);
                                        break;
                                    case 20:
                                        chart1.Series[0].Points.Add(Tilt7YList[i]);
                                        break;
                                    case 21:
                                        chart1.Series[0].Points.Add(Gyro7List[i]);
                                        break;
                                }
                            }


                            if (checkBox3.Checked)
                            {
                                switch (comboBox2.SelectedIndex + 1)
                                {
                                    case 1:
                                        chart1.Series[1].Points.Add(Tilt1XList[i]);
                                        break;
                                    case 2:
                                        chart1.Series[1].Points.Add(Tilt1YList[i]);
                                        break;
                                    case 3:
                                        chart1.Series[1].Points.Add(Gyro1List[i]);
                                        break;
                                    case 4:
                                        chart1.Series[1].Points.Add(Tilt2XList[i]);
                                        break;
                                    case 5:
                                        chart1.Series[1].Points.Add(Tilt2YList[i]);
                                        break;
                                    case 6:
                                        chart1.Series[1].Points.Add(Gyro2List[i]);
                                        break;
                                    case 7:
                                        chart1.Series[1].Points.Add(Tilt3XList[i]);
                                        break;
                                    case 8:
                                        chart1.Series[1].Points.Add(Tilt3YList[i]);
                                        break;
                                    case 9:
                                        chart1.Series[1].Points.Add(Gyro3List[i]);
                                        break;
                                    case 10:
                                        chart1.Series[1].Points.Add(Tilt4XList[i]);
                                        break;
                                    case 11:
                                        chart1.Series[1].Points.Add(Tilt4YList[i]);
                                        break;
                                    case 12:
                                        chart1.Series[1].Points.Add(Gyro4List[i]);
                                        break;
                                    case 13:
                                        chart1.Series[1].Points.Add(Tilt5XList[i]);
                                        break;
                                    case 14:
                                        chart1.Series[1].Points.Add(Tilt5YList[i]);
                                        break;
                                    case 15:
                                        chart1.Series[1].Points.Add(Gyro5List[i]);
                                        break;
                                    case 16:
                                        chart1.Series[1].Points.Add(Tilt6XList[i]);
                                        break;
                                    case 17:
                                        chart1.Series[1].Points.Add(Tilt6YList[i]);
                                        break;
                                    case 18:
                                        chart1.Series[1].Points.Add(Gyro6List[i]);
                                        break;
                                    case 19:
                                        chart1.Series[1].Points.Add(Tilt7XList[i]);
                                        break;
                                    case 20:
                                        chart1.Series[1].Points.Add(Tilt7YList[i]);
                                        break;
                                    case 21:
                                        chart1.Series[1].Points.Add(Gyro7List[i]);
                                        break;
                                }
                            }



                            if (checkBox4.Checked)
                            {
                                switch (comboBox3.SelectedIndex + 1)
                                {
                                    case 1:
                                        chart1.Series[2].Points.Add(Tilt1XList[i]);
                                        break;
                                    case 2:
                                        chart1.Series[2].Points.Add(Tilt1YList[i]);
                                        break;
                                    case 3:
                                        chart1.Series[2].Points.Add(Gyro1List[i]);
                                        break;
                                    case 4:
                                        chart1.Series[2].Points.Add(Tilt2XList[i]);
                                        break;
                                    case 5:
                                        chart1.Series[2].Points.Add(Tilt2YList[i]);
                                        break;
                                    case 6:
                                        chart1.Series[2].Points.Add(Gyro2List[i]);
                                        break;
                                    case 7:
                                        chart1.Series[2].Points.Add(Tilt3XList[i]);
                                        break;
                                    case 8:
                                        chart1.Series[2].Points.Add(Tilt3YList[i]);
                                        break;
                                    case 9:
                                        chart1.Series[2].Points.Add(Gyro3List[i]);
                                        break;
                                    case 10:
                                        chart1.Series[2].Points.Add(Tilt4XList[i]);
                                        break;
                                    case 11:
                                        chart1.Series[2].Points.Add(Tilt4YList[i]);
                                        break;
                                    case 12:
                                        chart1.Series[2].Points.Add(Gyro4List[i]);
                                        break;
                                    case 13:
                                        chart1.Series[2].Points.Add(Tilt5XList[i]);
                                        break;
                                    case 14:
                                        chart1.Series[2].Points.Add(Tilt5YList[i]);
                                        break;
                                    case 15:
                                        chart1.Series[2].Points.Add(Gyro5List[i]);
                                        break;
                                    case 16:
                                        chart1.Series[2].Points.Add(Tilt6XList[i]);
                                        break;
                                    case 17:
                                        chart1.Series[2].Points.Add(Tilt6YList[i]);
                                        break;
                                    case 18:
                                        chart1.Series[2].Points.Add(Gyro6List[i]);
                                        break;
                                    case 19:
                                        chart1.Series[2].Points.Add(Tilt7XList[i]);
                                        break;
                                    case 20:
                                        chart1.Series[2].Points.Add(Tilt7YList[i]);
                                        break;
                                    case 21:
                                        chart1.Series[2].Points.Add(Gyro7List[i]);
                                        break;
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
            int Cnt = 2;

            switch (PacketRec[1])
            {
                case DATA_PACKET:

                    fixed (byte* p = &PacketRec[Cnt])
                    { Tilt1X = *(float*)p; }
                    Cnt = Cnt + sizeof(float);

                    fixed (byte* p = &PacketRec[Cnt])
                    { Tilt1Y = *(float*)p; }
                    Cnt = Cnt + sizeof(float);

                    fixed (byte* p = &PacketRec[Cnt])
                    { Gyro1 = *(float*)p; }
                    Cnt = Cnt + sizeof(float);



                    fixed (byte* p = &PacketRec[Cnt])
                    { Tilt2X = *(float*)p; }
                    Cnt = Cnt + sizeof(float);

                    fixed (byte* p = &PacketRec[Cnt])
                    { Tilt2Y = *(float*)p; }
                    Cnt = Cnt + sizeof(float);

                    fixed (byte* p = &PacketRec[Cnt])
                    { Gyro2 = *(float*)p; }
                    Cnt = Cnt + sizeof(float);



                    fixed (byte* p = &PacketRec[Cnt])
                    { Tilt3X = *(float*)p; }
                    Cnt = Cnt + sizeof(float);

                    fixed (byte* p = &PacketRec[Cnt])
                    { Tilt3Y = *(float*)p; }
                    Cnt = Cnt + sizeof(float);

                    fixed (byte* p = &PacketRec[Cnt])
                    { Gyro3 = *(float*)p; }
                    Cnt = Cnt + sizeof(float);




                    fixed (byte* p = &PacketRec[Cnt])
                    { Tilt4X = *(float*)p; }
                    Cnt = Cnt + sizeof(float);

                    fixed (byte* p = &PacketRec[Cnt])
                    { Tilt4Y = *(float*)p; }
                    Cnt = Cnt + sizeof(float);

                    fixed (byte* p = &PacketRec[Cnt])
                    { Gyro4 = *(float*)p; }
                    Cnt = Cnt + sizeof(float);



                    fixed (byte* p = &PacketRec[Cnt])
                    { Tilt5X = *(float*)p; }
                    Cnt = Cnt + sizeof(float);

                    fixed (byte* p = &PacketRec[Cnt])
                    { Tilt5Y = *(float*)p; }
                    Cnt = Cnt + sizeof(float);

                    fixed (byte* p = &PacketRec[Cnt])
                    { Gyro5 = *(float*)p; }
                    Cnt = Cnt + sizeof(float);



                    fixed (byte* p = &PacketRec[Cnt])
                    { Tilt6X = *(float*)p; }
                    Cnt = Cnt + sizeof(float);

                    fixed (byte* p = &PacketRec[Cnt])
                    { Tilt6Y = *(float*)p; }
                    Cnt = Cnt + sizeof(float);

                    fixed (byte* p = &PacketRec[Cnt])
                    { Gyro6 = *(float*)p; }
                    Cnt = Cnt + sizeof(float);



                    fixed (byte* p = &PacketRec[Cnt])
                    { Tilt7X = *(float*)p; }
                    Cnt = Cnt + sizeof(float);

                    fixed (byte* p = &PacketRec[Cnt])
                    { Tilt7Y = *(float*)p; }
                    Cnt = Cnt + sizeof(float);

                    fixed (byte* p = &PacketRec[Cnt])
                    { Gyro7 = *(float*)p; }
                    Cnt = Cnt + sizeof(float);


                    fixed (byte* p = &PacketRec[Cnt])
                    { KGyro1 = *(ushort*)p; }
                    Cnt = Cnt + sizeof(ushort);


                    Tilt1XList.Add(Tilt1X);
                    Tilt1YList.Add(Tilt1Y);
                    Gyro1List.Add(Gyro1);

                    Tilt2XList.Add(Tilt2X);
                    Tilt2YList.Add(Tilt2Y);
                    Gyro2List.Add(Gyro2);

                    Tilt3XList.Add(Tilt3X);
                    Tilt3YList.Add(Tilt3Y);
                    Gyro3List.Add(Gyro3);

                    Tilt4XList.Add(Tilt4X);
                    Tilt4YList.Add(Tilt4Y);
                    Gyro4List.Add(Gyro4);

                    Tilt5XList.Add(Tilt5X);
                    Tilt5YList.Add(Tilt5Y);
                    Gyro5List.Add(Gyro5);

                    Tilt6XList.Add(Tilt6X);
                    Tilt6YList.Add(Tilt6Y);
                    Gyro6List.Add(Gyro6);

                    Tilt7XList.Add(Tilt7X);
                    Tilt7YList.Add(Tilt7Y);
                    Gyro7List.Add(Gyro7);


                    if (Gyro7List.Count == ListCount)
                    {
                        backgroundWorker1.ReportProgress(DATA_PACKET);
                        while (Gyro7List.Count != 0) ;
                    }
                        


                    break;

            }

        }
    }

}