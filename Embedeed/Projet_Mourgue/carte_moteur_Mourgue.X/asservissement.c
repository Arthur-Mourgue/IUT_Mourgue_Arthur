
#include "asservissement.h"
#include "Robot.h"
#include "UART_Protocol.h"
#include "Utilities.h"
#define ASSERVICEMENT 0x0063

float Error,Consigne,Corr_P,Corr_I,Corr_D,Command;

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

    Error = 1;
    Consigne = 2 ;
    Corr_P = 3;
    Corr_I = 4;
    Corr_D = 5;
    Command = 6;
    
   unsigned char AsservicementPayload[40];
   
    getBytesFromFloat(AsservicementPayload, 0, (float) (PidCorr->Kp));
    getBytesFromFloat(AsservicementPayload, 4, (float) (PidCorr->Ki));
    getBytesFromFloat(AsservicementPayload, 8, (float) (PidCorr->Kd));
    getBytesFromFloat(AsservicementPayload, 12, (float) (PidCorr->erreurProportionelleMax));
    getBytesFromFloat(AsservicementPayload, 16, (float)(PidCorr->erreurIntegraleMax));
    getBytesFromFloat(AsservicementPayload, 20, (float)(PidCorr->erreurDeriveeMax));
    getBytesFromFloat(AsservicementPayload, 24, (float)(Error));
    getBytesFromFloat(AsservicementPayload, 28, (float)(Consigne));
    getBytesFromFloat(AsservicementPayload, 32, (float)(Corr_P));
    getBytesFromFloat(AsservicementPayload, 36, (float)(Corr_I));
    getBytesFromFloat(AsservicementPayload, 40, (float)(Corr_D));
    getBytesFromFloat(AsservicementPayload, 44, (float)(Command));
    
    

    UartEncodeAndSendMessage(ASSERVICEMENT, 50, AsservicementPayload);

}