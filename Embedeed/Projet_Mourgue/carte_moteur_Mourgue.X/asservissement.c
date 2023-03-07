
#include "asservissement.h"
#include "Robot.h"
#include "UART_Protocol.h"
#include "Utilities.h"
#include "QEI.h"

#define ASSERVICEMENTCONSTANT 0x0064
#define ASSERVICEMENT 0x0063

float Error, Consigne, Corr_P, Corr_I, Corr_D, Command;

void SetupPidAsservissement(volatile PidCorrector* PidCorr, float Kp, float Ki, float Kd, float proportionelleMax, float integralMax, float deriveeMax) {
    PidCorr->Kp = Kp;
    PidCorr->erreurProportionelleMax = proportionelleMax; //On limite la correction due au Kp
    PidCorr->Ki = Ki;
    PidCorr->erreurIntegraleMax = integralMax; //On limite la correction due au Ki
    PidCorr->Kd = Kd;
    PidCorr->erreurDeriveeMax = deriveeMax;
}

void SendAsservissementConstant(volatile PidCorrector* PidCorr, unsigned char PIDdescriptor) {

    unsigned char AsservissementConstantPayload[25];
    AsservissementConstantPayload[0] = PIDdescriptor;
    getBytesFromFloat(AsservissementConstantPayload, 1, (float) (PidCorr->Kp));
    getBytesFromFloat(AsservissementConstantPayload, 5, (float) (PidCorr->Ki));
    getBytesFromFloat(AsservissementConstantPayload, 9, (float) (PidCorr->Kd));
    getBytesFromFloat(AsservissementConstantPayload, 13, (float) (PidCorr->erreurProportionelleMax));
    getBytesFromFloat(AsservissementConstantPayload, 17, (float) (PidCorr->erreurIntegraleMax));
    getBytesFromFloat(AsservissementConstantPayload, 21, (float) (PidCorr->erreurDeriveeMax));

    UartEncodeAndSendMessage(ASSERVICEMENTCONSTANT, 25, AsservissementConstantPayload);
}

void SendAsservissementVariables(volatile PidCorrector* PidCorr, unsigned char PIDdescriptor) {
    unsigned char AsservissementPayload[25];

    AsservissementPayload[0] = PIDdescriptor;
    getBytesFromFloat(AsservissementPayload, 1, (float) (PidCorr->erreur));
    getBytesFromFloat(AsservissementPayload, 5, (float) (PidCorr->Consigne));
    getBytesFromFloat(AsservissementPayload, 9, (float) (PidCorr->corrP));
    getBytesFromFloat(AsservissementPayload, 13, (float) (PidCorr->corrI));
    getBytesFromFloat(AsservissementPayload, 17, (float) (PidCorr->corrD));
    getBytesFromFloat(AsservissementPayload, 21, (float) (PidCorr->Command));

    UartEncodeAndSendMessage(ASSERVICEMENT, 25, AsservissementPayload);
}

double Correcteur(volatile PidCorrector* PidCorr, double erreur) {
    PidCorr->erreur = erreur;
    double erreurProportionnelle = LimitToInterval(erreur, -PidCorr->erreurProportionelleMax / PidCorr->Kp, PidCorr->erreurProportionelleMax / PidCorr->Kp);
    PidCorr->corrP =  PidCorr->Kp * erreurProportionnelle ;
    PidCorr->erreurIntegrale = LimitToInterval((PidCorr->epsilon_1 + erreur), -PidCorr->erreurIntegraleMax / PidCorr->Ki, PidCorr->erreurIntegraleMax / PidCorr->Ki);
    PidCorr->corrI = PidCorr->Ki * PidCorr->erreurIntegrale ;
    double erreurDerivee = (erreur - PidCorr->epsilon_1) * FREQ_ECH_QEI;
    double deriveeBornee = LimitToInterval(erreurDerivee, -PidCorr->erreurDeriveeMax / PidCorr->Kd, PidCorr->erreurDeriveeMax / PidCorr->Kd );
            PidCorr->epsilon_1 = erreur;
            PidCorr->corrD = deriveeBornee * PidCorr->Kd;

    return PidCorr->corrP + PidCorr->corrI + PidCorr->corrD;
}

void UpdateAsservissement() {
    robotState.PidLineaire.erreur = robotState.PidLineaire.consigne - robotState.vitesseLineaireFromOdometry; 
            robotState.PidAngulaire.erreur = ...;
            robotState.xCorrectionVitessePourcent = Correcteur(&robotState.PidLineaire, robotState.PidLineaire.erreur);
            robotState.thetaCorrectionVitessePourcent = ...;
            PWMSetSpeedConsignePolaire(robotState.xCorrectionVitessePourcent,
            robotState.thetaCorrectionVitessePourcent);
}
