using ExtendedSerialPort;
using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;

namespace Arthur_Mourgue_RobotInterface
{
    /// <summary>
    /// Logique d'interaction pour MainWindow.xaml
    /// </summary>
    
    public partial class MainWindow : Window
    {
        ReliableSerialPort serialPort1;
        DispatcherTimer timerAfficage;
        bool toggle = false;
        Robot robot = new Robot();

        public MainWindow()
        {
            InitializeComponent();

            

            serialPort1 = new ReliableSerialPort("COM3", 115200, Parity.None, 8, StopBits.One);
            serialPort1.DataReceived += SerialPort1_DataReceived;
            serialPort1.Open();

            timerAfficage = new DispatcherTimer();
            timerAfficage.Interval = new TimeSpan(0, 0, 0, 0, 100);
            timerAfficage.Tick += TimerAfficage_Tick;
            timerAfficage.Start();
        }

        private void TimerAfficage_Tick(object sender, EventArgs e)
        {
            //throw new NotImplementedException();
            if (robot.recievedText != "")
            {
                TextBoxReception.Text += robot.recievedText;
                robot.recievedText = "";
            }
        }

        private void SerialPort1_DataReceived(object sender, DataReceivedArgs e)
        {
            robot.recievedText += Encoding.UTF8.GetString(e.Data, 0, e.Data.Length);
        }

        private void ButtonEnvoyer_Click(object sender, RoutedEventArgs e)
        {
            
            if (toggle == false)
            {
                ButtonEnvoyer.Background = Brushes.RoyalBlue;
            }
            else
            {
                ButtonEnvoyer.Background = Brushes.Beige;
            }
            toggle = !toggle;
            SendMessage();

        }

        private void TextBoxEmission_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                SendMessage();
            }
        }


        private void SendMessage()
        {
            serialPort1.WriteLine(TextBoxEmission.Text);
            TextBoxEmission.Text = "";
        }

        private void ButtonClear_Click(object sender, RoutedEventArgs e)
        {
            TextBoxReception.Text = "";
        }

        private void ButtonTest_Click(object sender, RoutedEventArgs e)
        {
            byte[] byteList = new byte [20];
            int i;
            for (i = 0; i < 20; i++)
            {
                byteList[i] = (byte)(2 * i);
            }
            serialPort1.Write(byteList, 0, 20);

        }
    }
}
