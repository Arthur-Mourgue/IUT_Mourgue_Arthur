using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Arthur_Mourgue_RobotInterface
{
    public class Robot
    {
        public string recievedText = "";
        public float distanceTelemetreDroit;
        public float distanceTelemetreCentre;
        public float distanceTelemetreGauche;
        
        Queue<byte> byteListeReceived = new Queue<byte>();

        public Robot()
        {

        }

    }
}
