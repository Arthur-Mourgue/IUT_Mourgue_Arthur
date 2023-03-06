
#include "asservissement.h"
#include "Robot.h"
#include "UART_Protocol.h"
#include "Utilities.h"
#define ASSERVICEMENT 0x0063



void SetupPidAsservissement(volatile PidCorrector* PidCorr, float Kp, float Ki, float Kd, float proportionelleMax, float integralMax, float deriveeMax){
PidCorr->Kp = Kp;
PidCorr->erreurProportionelleMax = proportionelleMax; //On limite la correction due au Kp
PidCorr->Ki = Ki;
PidCorr->erreurIntegraleMax = integralMax; //On limite la correction due au Ki
PidCorr->Kd = Kd;
PidCorr->erreurDeriveeMax = deriveeMax;

SendAsservissement(PidCorr);
}

void SendAsservissement(volatile PidCorrector* PidCorr){
    
   unsigned char AsservicementPayload[25];
   
    getBytesFromFloat(AsservicementPayload, 0, (float) (PidCorr->Kp));
    getBytesFromFloat(AsservicementPayload, 4, (float) (PidCorr->Ki));
    getBytesFromFloat(AsservicementPayload, 8, (float) (PidCorr->Kd));
    getBytesFromFloat(AsservicementPayload, 12, (float) (PidCorr->erreurProportionelleMax));
    getBytesFromFloat(AsservicementPayload, 16, (float)(PidCorr->erreurIntegraleMax));
    getBytesFromFloat(AsservicementPayload, 20, (float)(PidCorr->erreurDeriveeMax));

    UartEncodeAndSendMessage(ASSERVICEMENT, 25, AsservicementPayload);

}