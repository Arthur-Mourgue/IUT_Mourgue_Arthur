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

        public PID pidLin = new PID();
        public PID pidAng = new PID();

        public Queue<byte> byteListReceived = new Queue<byte>();
        public Robot()
        {
            
        }
    }

    public class PID
    {
        public float Kp;
        public float Ki;
        public float Kd;
        public float erreurProportionelleMax;
        public float erreurIntegraleMax;
        public float erreurDeriveeMax;

    }
}
