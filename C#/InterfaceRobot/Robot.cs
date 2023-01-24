using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace InterfaceRobot
{
    public class Robot
    {
        public string receivedText = "";
        public float distanceTelemetreDroit;
        public float distanceTelemetreCentre ;
        public float distanceTelemetreGauche;

        public float positionXOdo;
        public float positionYOdo;
        public float AngleROdo;
        public float VitesseAOdo;
        public float VitesseLOdo;
        public float TimerOdo;

        public Queue<byte> byteListReceived = new Queue<byte>();
        public Robot()
        {
            
        }
    }
}
