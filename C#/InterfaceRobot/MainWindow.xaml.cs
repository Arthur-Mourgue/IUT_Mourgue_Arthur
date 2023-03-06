
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
using MouseKeyboardActivityMonitor.WinApi;
using MouseKeyboardActivityMonitor;
using SciChart.Charting.Visuals;
using WpfAsservissementDisplay;
using Utilities;
using Constants;

namespace InterfaceRobot
{
    /// <summary>
    /// Logique d'interaction pour MainWindow.xaml
    /// </summary>
    /// 
    public partial class MainWindow : Window
    {
        ReliableSerialPort serialPort1;
        DispatcherTimer timerAffichage;
        Robot robot = new Robot();
        private readonly KeyboardHookListener m_KeyboardHookManager;


        public MainWindow()
        {
            SciChartSurface.SetRuntimeLicenseKey("3veHyjwEIEQaDV8qd5l2Fi1mSsLic+DqdTuOS3i9xeOMjpqiWBdgmhBRMwHu9/5w2wXOHVOTWENlMvOMQjVNxg47zsEngF97+UN0UurixjoLk08HXwVNN2y2nYEhXWz63hbqsRmKHBDmtV95oK/rMODj+9r53ZO2qNGEHaR6InmuW3v9OjooV64GqiZXLYICip7TQR/f6sy3gEkVX4hYFJiWjBo7KEuIRbOXelmTRC0y2YYzDgOJacDbZ20LZdAhzt+zFY/aDjmqN3MXMqaZ8cAwWjx35KYKNJ02jvElvjD4T9wwM1wuuKEg9kvDmMvTWJjSzIOdGNB1vzmwNHkK4WRuoK2x5rsqZsWhTGsp728XR/xJoxAUrd71laBiwobq+BC5SaE1hufOgoH4Bh3tMs4c9tKo2TZ+j2gv1cIEEfYDQlUH0aLoJGDSc3EWoOCmXQ9MkXH+irfMwxlHBa0GrFLoLl8AjHJ7iNuX7g7gu1ceGXFZ6A88tMse2ohAS2Eibw==");

            InitializeComponent();
            serialPort1 = new ReliableSerialPort("COM16", 115200, Parity.None, 8, StopBits.One);
            serialPort1.Open();

            serialPort1.DataReceived += SerialPort1_DataReceived;

            oscilloSpeed.AddOrUpdateLine(1, 200, "Ligne1");
            oscilloPos.AddOrUpdateLine(2, 200, "Ligne1");


            m_KeyboardHookManager = new KeyboardHookListener(new GlobalHooker());
            m_KeyboardHookManager.Enabled = true;
            


            timerAffichage = new DispatcherTimer();
            timerAffichage.Interval = new TimeSpan(0, 0, 0, 0, 100);
            timerAffichage.Tick += TimerAffichage_Tick;
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

            oscilloSpeed.AddPointToLine(1, robot.TimerOdo , robot.VitesseLOdo);
            oscilloSpeed.ChangeLineColor(1, Color.FromRgb(0, 255, 0));

            oscilloPos.AddPointToLine(2, robot.positionXOdo, robot.positionYOdo);
            oscilloPos.ChangeLineColor(2, Color.FromRgb(0, 0, 255));


            asservSpeedDisplay.UpdatePolarOdometrySpeed(robot.positionXOdo, robot.AngleROdo);
            asservSpeedDisplay.UpdateDisplay();

            while (robot.byteListReceived.Count() > 0)
            {
                var c = robot.byteListReceived.Dequeue();
                //receptionTextBox.Text += "0x" + c.ToString("X2") + " ";    
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



        bool mode = true;
        private void buttonModeAuto_Click(object sender, RoutedEventArgs e)
        {
            switch (mode)
            {
                case (false): buttonAuto.Background = Brushes.Green; break;
                case (true): buttonAuto.Background = Brushes.Red; break;
            }

            byte[] payload = new byte[1];

            if (mode == false)
            {
                // Send mode auto
                payload[0] = 1;
                UartEncodeAndSendMessage((int)MessageFunctions.SetRobotMode, 1, payload);
                mode = true;
            }
            
            else
            {
                // Send mode manu
                payload[0] = 0;
                UartEncodeAndSendMessage((int)MessageFunctions.SetRobotMode, 1, payload);
                mode = false;
            }

        }

        
        private void buttonAsserv_Click(object sender, RoutedEventArgs e)
        {
            robot.pidLin.Kp = 1;
            robot.pidLin.Ki = 2;
            robot.pidLin.Kd = 3;
            robot.pidLin.erreurProportionelleMax = 4;
            robot.pidLin.erreurIntegraleMax = 5;
            robot.pidLin.erreurDeriveeMax = 6;

            byte[] tableauAsserv = new byte[25];

            Array.Copy(BitConverter.GetBytes(robot.pidLin.Kp), 0, tableauAsserv, 0 ,4);
            Array.Copy(BitConverter.GetBytes(robot.pidLin.Ki), 0, tableauAsserv, 4, 4);
            Array.Copy(BitConverter.GetBytes(robot.pidLin.Kd), 0, tableauAsserv, 8, 4);
            Array.Copy(BitConverter.GetBytes(robot.pidLin.erreurProportionelleMax), 0, tableauAsserv, 12, 4);
            Array.Copy(BitConverter.GetBytes(robot.pidLin.erreurIntegraleMax), 0, tableauAsserv, 16, 4);
            Array.Copy(BitConverter.GetBytes(robot.pidLin.erreurDeriveeMax), 0, tableauAsserv, 20, 4);
            tableauAsserv[24] = 0;


            UartEncodeAndSendMessage((int)MessageFunctions.PIDAsservicement, 25,tableauAsserv);
        }

        private void buttonTest_Click(object sender, RoutedEventArgs e)
        {
            //string s = "Transmission";
            //byte[] payload = Encoding.ASCII.GetBytes(s);
            //UartEncodeAndSendMessage((int)MessageFunctions.TextMessage, payload.Length, payload);

            //payload = new byte[] { 0, 0, 0 };
            //UartEncodeAndSendMessage((int)MessageFunctions.LEDValues, 3, payload);

            //payload = new byte[3];
            //payload[0] = 20;
            //payload[1] = 30;
            //payload[2] = 40;
            //UartEncodeAndSendMessage((int)MessageFunctions.DistancesTelemetre, 3, payload);

            //payload = new byte[2];
            //payload[0] = 100;
            //payload[1] = 90;
            //UartEncodeAndSendMessage((int)MessageFunctions.MotorSpeed, 2, payload);

            byte[] payload = new byte[1];
            payload[0] = 0;
            UartEncodeAndSendMessage((int)MessageFunctions.SetRobotMode, 1, payload);

            payload = new byte[1];
            payload[0] = 34;
            UartEncodeAndSendMessage((int)MessageFunctions.SetRobotState, 1, payload);

            



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
            for (i = 0; i < msgPayloadLength; i++)
            {
                checksum ^= msgPayload[i];
            }
            return checksum;

        }

        void UartEncodeAndSendMessage(int msgFunction, int msgPayloadLength, byte[] msgPayload)
        {
            int pos = 0;
            byte[] encodeMsg = new byte[msgPayloadLength + 6];

            encodeMsg[pos++] = 0xFE;
            encodeMsg[pos++] = (byte)(msgFunction >> 8);
            encodeMsg[pos++] = (byte)(msgFunction >> 0);
            encodeMsg[pos++] = (byte)(msgPayloadLength >> 8);
            encodeMsg[pos++] = (byte)(msgPayloadLength >> 0);
            for (int i = 0; i < msgPayloadLength; i++)
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
                        ProcessDecodedMessage(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
                    }
                    else
                    {
                        receptionTextBox.Text += "/n ERREUR /n";
                    }
                    rcvState = StateReception.Waiting;
                    break;
                default:
                    rcvState = StateReception.Waiting;
                    break;
            }
        }

        bool autoControlActivated;

        void ProcessDecodedMessage(int msgFunction, int msgPayloadLength, byte[] msgPayload)
        {
            switch ((MessageFunctions)msgFunction)
            {
                case MessageFunctions.TextMessage:
                    receptionTextBox.Text += Encoding.ASCII.GetString(msgPayload);
                    break;

                case MessageFunctions.DistancesTelemetre:
                    IR_gauche.Content = msgPayload[0];
                    IR_centre.Content = msgPayload[1];
                    IR_droit.Content = msgPayload[2];
                    break;

                case MessageFunctions.MotorSpeed:
                    Vitesse_gauche.Content = msgPayload[0];
                    Vitesse_droit.Content = msgPayload[1];
                    break;

                case MessageFunctions.LEDValues:
                    if (msgPayload[0] == 1) Led_1.IsChecked = true;
                    else Led_1.IsChecked = false;

                    if (msgPayload[1] == 1) Led_2.IsChecked = true;
                    else Led_2.IsChecked = false;

                    if (msgPayload[2] == 1) Led_3.IsChecked = true;
                    else Led_3.IsChecked = false;
                    break;

                case MessageFunctions.RobotState:
                    int instant = (((int)msgPayload[1]) << 24) + (((int)msgPayload[2]) << 16)
                    + (((int)msgPayload[3]) << 8) + ((int)msgPayload[4]);
                    receptionTextBox.Text += "\nRobot␣State␣:␣" +
                    ((StateRobot)(msgPayload[0])).ToString() +
                    "␣-␣" + instant.ToString() + "␣ms";
                    break;


                case MessageFunctions.PIDAsservicement:

                    var tabPID = BitConverter.ToSingle(msgPayload, 0);
                    robot.pidLin.Kp = tabPID;
                    tabPID = BitConverter.ToSingle(msgPayload, 4);
                    robot.pidLin.Ki = tabPID;
                    tabPID = BitConverter.ToSingle(msgPayload, 8);
                    robot.pidLin.Kd = tabPID;
                    tabPID = BitConverter.ToSingle(msgPayload, 12);
                    robot.pidLin.erreurProportionelleMax = tabPID;
                    tabPID = BitConverter.ToSingle(msgPayload, 16);
                    robot.pidLin.erreurIntegraleMax = tabPID;
                    tabPID = BitConverter.ToSingle(msgPayload, 20);
                    robot.pidLin.erreurDeriveeMax = tabPID;
                    int PidCor = msgPayload[24];



                    break;

                case MessageFunctions.PositionData:
                    robot.TimerOdo = (((int)msgPayload[0]) << 24) + (((int)msgPayload[1]) << 16)
                    + (((int)msgPayload[2]) << 8) + ((int)msgPayload[3]);
                    var tabPos = BitConverter.ToSingle(msgPayload, 4);
                    robot.positionXOdo = tabPos;
                    tabPos = BitConverter.ToSingle(msgPayload, 8);
                    robot.positionYOdo = tabPos;
                    tabPos = BitConverter.ToSingle(msgPayload, 12);
                    robot.AngleROdo = tabPos;
                    tabPos = BitConverter.ToSingle(msgPayload, 16);
                    robot.VitesseAOdo = tabPos;
                    tabPos = BitConverter.ToSingle(msgPayload, 20);
                    robot.VitesseLOdo = tabPos;
                    TimerOdo.Content = robot.TimerOdo;
                    Position_X.Content = robot.positionXOdo;
                    Position_Y.Content = robot.positionYOdo;
                    Vitesse_L.Content = robot.VitesseLOdo;
                    Vitesse_A.Content = robot.VitesseAOdo;
                    AngleOdo.Content = robot.AngleROdo;

                    break;


            }
        }


        public enum MessageFunctions
        {
            TextMessage = 0x0080,
            LEDValues = 0x0020,
            DistancesTelemetre = 0x0030,
            MotorSpeed = 0x0040,
            RobotState = 0x0050,
            SetRobotState = 0x0051,
            SetRobotMode = 0x0052,
            PositionData = 0x0061,
            PIDAsservicement = 0x0063,
        }

        public enum StateRobot
        {
            STATE_ATTENTE = 0,
            STATE_ATTENTE_EN_COURS = 1,
            STATE_AVANCE = 2,
            STATE_AVANCE_EN_COURS = 3,
            STATE_TOURNE_GAUCHE = 4,
            STATE_TOURNE_GAUCHE_EN_COURS = 5,
            STATE_TOURNE_DROITE = 6,
            STATE_TOURNE_DROITE_EN_COURS = 7,
            STATE_TOURNE_SUR_PLACE_GAUCHE = 8,
            STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS = 9,
            STATE_TOURNE_SUR_PLACE_DROITE = 10,
            STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS = 11,
            STATE_RECULE_PAR_GAUCHE = 16,
            STATE_RECULE_PAR_GAUCHE_EN_COURS = 17,
            STATE_RECULE_PAR_DROITE = 18,
            STATE_RECULE_PAR_DROITE_EN_COURS = 19,
            STATE_ARRET = 12,
            STATE_ARRET_EN_COURS = 13,
            STATE_RECULE = 14,
            STATE_RECULE_EN_COURS = 15,

            STATE_RECULE_UN_PEU_GAUCHE = 20,
            STATE_RECULE_UN_PEU_GAUCHE_EN_COURS = 21,
            STATE_RECULE_UN_PEU_DROITE = 22,
            STATE_RECULE_UN_PEU_DROITE_EN_COURS = 23,
            STATE_TOURNE_UN_PEU_GAUCHE = 24,
            STATE_TOURNE_UN_PEU_GAUCHE_EN_COURS = 25,
            STATE_TOURNE_UN_PEU_DROITE = 26,
            STATE_TOURNE_UN_PEU_DROITE_EN_COURS = 27,

            STATE_TOURNE_BCP_GAUCHE = 28,
            STATE_TOURNE_BCP_GAUCHE_EN_COURS = 29,
            STATE_TOURNE_BCP_DROITE = 30,
            STATE_TOURNE_BCP_DROITE_EN_COURS = 31,
            STATE_180_DROITE = 32,
            STATE_180_DROITE_EN_COURS = 33,
            STATE_180_GAUCHE = 40,
            STATE_180_GAUCHE_EN_COURS = 41,
            STATE_180 = 42,
            STATE_180_EN_COURS = 43,
            STATE_90_DROITE = 44,
            STATE_90_DROITE_EN_COURS = 45,
            STATE_90_GAUCHE = 46,
            STATE_90_GAUCHE_EN_COURS = 47,
            STATE_TOURNE_MINI_GAUCHE = 38,
            STATE_TOURNE_MINI_GAUCHE_EN_COURS = 39,
            STATE_TOURNE_MINI_DROITE = 36,
            STATE_TOURNE_MINI_DROITE_EN_COURS = 37,

            STATE_CELEBRATION = 34,
            STATE_CELEBRATION_EN_COURS = 35,
        }

        
    }
}


