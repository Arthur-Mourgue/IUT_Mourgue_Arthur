
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

namespace InterfaceRobot
{
    /// <summary>
    /// Logique d'interaction pour MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        ReliableSerialPort serialPort1;
        DispatcherTimer timerAffichage;
        Robot robot = new Robot();

        public MainWindow()
        {
            InitializeComponent();
            serialPort1 = new ReliableSerialPort("COM3", 115200, Parity.None, 8, StopBits.One);
            serialPort1.Open();

            serialPort1.DataReceived += SerialPort1_DataReceived;

            timerAffichage = new DispatcherTimer();
            timerAffichage.Interval = new TimeSpan(0, 0, 0, 0, 100);
            timerAffichage.Tick += TimerAffichage_Tick; ;
            timerAffichage.Start();



        }
        

        private void SerialPort1_DataReceived(object sender, DataReceivedArgs e)
        {
            robot.receivedText += Encoding.UTF8.GetString(e.Data, 0, e.Data.Length);
            foreach (var c in e.Data)
            {
                robot.byteListReceived.Enqueue(c);
            }
        }

        private void TimerAffichage_Tick(object sender, EventArgs e)
        {
            while(robot.byteListReceived.Count() > 0)
            {
                var c = robot.byteListReceived.Dequeue();
                receptionTextBox.Text += "0x" + c.ToString("X2") + " ";    
                DecodeMessage(c);            
            }
            //if (robot.receivedText != "")
            //{
            //    receptionTextBox.Text += robot.receivedText;                
            //    robot.receivedText = "";
            //}
        }

        int couleur = 0;
        private void buttonEnvoyer_Click(object sender, RoutedEventArgs e)
        {
            couleur++; couleur = couleur % 2;

            switch (couleur)
            {
                case (0): buttonEnvoyer.Background = Brushes.White; break;
                case (1): buttonEnvoyer.Background = Brushes.Beige; break;
            }

            SendMessage();
            robot.receivedText = "\n";
        }

        private void emissionTextBox_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                SendMessage();
            }
        }

        private void SendMessage()
        {
            //receptionTextBox.Text += "Recu : " + emissionTextBox.Text + "\n";
            serialPort1.Write(emissionTextBox.Text);    //ecris sur le port1
            emissionTextBox.Text = "";  //clear la text box emission
        }

        private void buttonClear_Click(object sender, RoutedEventArgs e)
        {
            receptionTextBox.Text = "";
        }

        private void test_Click(object sender, RoutedEventArgs e)
        {
            //string s = "Transmittion";
            //byte[] payload = Encoding.ASCII.GetBytes(s);
            //UartEncodeAndSendMessage((int)MessageFunctions.TextMessage, payload.Length, payload);

            string s = "1:1";
            byte[] payload = Encoding.ASCII.GetBytes(s);
            UartEncodeAndSendMessage((int)MessageFunctions.LEDValues, 2, payload);


            //payload = new byte[3];
            //payload[0] = 20;
            //payload[1] = 30;
            //payload[2] = 40;
            //UartEncodeAndSendMessage((int)MessageFunctions.DistancesTelemetre, 3, payload);

            //payload = new byte[2];
            //payload[0] = 20;
            //payload[1] = 30;
            //UartEncodeAndSendMessage((int)MessageFunctions.LEDValues, 2, payload);

            //s = "Consigne Vitesse";
            //array = Encoding.ASCII.GetBytes(s);
            //UartEncodeAndSendMessage(0x0020, 2, array);


            /*
            byte[] byteList = new byte[20];
            for (int i = 0; i < 20; i++)
            {
                byteList[i] = (byte)(2 * i);
            }
            serialPort1.Write(byteList,0,20);
            robot.receivedText += "\n";
            */
        }

        byte CalculateChecksum(int msgFunction, int msgPayloadLength, byte[] msgPayload)
        {
            byte checksum = 0;

            checksum ^= 0xFE;
            checksum ^= (byte)(msgFunction >> 8);
            checksum ^= (byte)(msgFunction >> 0);
            checksum ^= (byte)(msgPayloadLength >> 8);
            checksum ^= (byte)(msgPayloadLength >> 0);
            int i;
            for (i = 0; i < msgPayloadLength ; i++)
            {
                checksum ^= msgPayload[i];
            }
            return checksum;

        }

        void UartEncodeAndSendMessage(int msgFunction, int msgPayloadLength, byte[] msgPayload)
        { 
            int pos = 0;
            byte[] encodeMsg = new byte[msgPayloadLength + 6];

            encodeMsg[pos++] += 0xFE;
            encodeMsg[pos++] += (byte)(msgFunction >> 8);
            encodeMsg[pos++] += (byte)(msgFunction >> 0);
            encodeMsg[pos++] += (byte)(msgPayloadLength >> 8);
            encodeMsg[pos++] += (byte)(msgPayloadLength >> 0);
            for (int i = 0 ; i < msgPayloadLength; i++)
            {
                encodeMsg[pos++] ^= msgPayload[i];
            }

            encodeMsg[pos++] += (byte)(CalculateChecksum(msgFunction, msgPayloadLength, msgPayload));

            serialPort1.Write(encodeMsg, 0, pos);
        }

        public enum StateReception
        {
            Waiting,
            FunctionMSB,
            FunctionLSB,
            PayloadLengthMSB,
            PayloadLengthLSB,
            Payload,
            CheckSum
        }
        StateReception rcvState = StateReception.Waiting;
        int msgDecodedFunction = 0;
        int msgDecodedPayloadLength = 0;
        byte[] msgDecodedPayload;
        int msgDecodedPayloadIndex = 0;
        byte calculatedChecksum = 0;

        private void DecodeMessage(byte c)
        {
            switch (rcvState)
            {
                case StateReception.Waiting:
                    if (c == 0xFE)
                        rcvState = StateReception.FunctionMSB;
                    break;
                case StateReception.FunctionMSB:
                    msgDecodedFunction = c << 8;
                    rcvState = StateReception.FunctionLSB;
                    break;
                case StateReception.FunctionLSB:
                    msgDecodedFunction += c << 0;
                    rcvState = StateReception.PayloadLengthMSB;
                    break;
                case StateReception.PayloadLengthMSB:
                    msgDecodedPayloadLength = c << 8;
                    rcvState = StateReception.PayloadLengthLSB;
                    break;
                case StateReception.PayloadLengthLSB:
                    msgDecodedPayloadLength += c << 0;
                    if (msgDecodedPayloadLength == 0)
                        rcvState = StateReception.CheckSum;
                    else if (msgDecodedPayloadLength >= 1024)
                        rcvState = StateReception.Waiting;
                    else
                    {
                        msgDecodedPayload = new byte[msgDecodedPayloadLength];
                        msgDecodedPayloadIndex = 0;
                        rcvState = StateReception.Payload;
                    }
                    break;
                case StateReception.Payload:
                    msgDecodedPayload[msgDecodedPayloadIndex] = c;
                    msgDecodedPayloadIndex++;
                    if (msgDecodedPayloadIndex >= msgDecodedPayloadLength)
                    {
                        rcvState = StateReception.CheckSum;
                    }
                    break;
                case StateReception.CheckSum:
                    calculatedChecksum = CalculateChecksum(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
                    if (calculatedChecksum == c)
                    {
                        //Success, on a un message valide
                        receptionTextBox.Text += " OK \n";
                        ProcessDecodedMessage(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
                    }
                    else
                    {
                        receptionTextBox.Text += " Pas OK \n";
                    }
                    rcvState = StateReception.Waiting;
                    break;
                default:
                    rcvState = StateReception.Waiting;
                    break;
            }
        }

        int[] stateLed;
        void ProcessDecodedMessage(int msgFunction, int msgPayloadLength, byte[] msgPayload)
        {
            switch((MessageFunctions)msgFunction)
            {
                case MessageFunctions.TextMessage:
                    receptionTextBox.Text += Encoding.ASCII.GetString(msgPayload);
                    break;
                case MessageFunctions.DistancesTelemetre:
                    receptionTextBox.Text = " Distance telemetre  \n";
                    stateLed[msgPayload[0] - 1] = (int)msgPayload[1]; 
                    break;
                case MessageFunctions.LEDValues:
                    receptionTextBox.Text = " LED  \n";
                    Led_1.IsChecked = true;
                    break;
            }
        }

    }

    public enum MessageFunctions
    {
        TextMessage = 0x0080,
        LEDValues = 0x0020,
        DistancesTelemetre = 0x0030,
        MotorSpeed = 0x0040,
    }
}