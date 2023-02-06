
#include "asservissement.h"




void SetupPidAsservissement(volatile PidCorrector* PidCorr, float Kp, float Ki, float Kd, double pro, double proportionelleMax, double integralMax, double deriveeMax){
PidCorr->Kp = Kp;
PidCorr->erreurProportionelleMax = proportionelleMax; //On limite la correction due au Kp
PidCorr->Ki = Ki;
PidCorr->erreurIntegraleMax = integralMax; //On limite la correction due au Ki
PidCorr->Kd = Kd;
PidCorr->erreurDeriveeMax = deriveeMax;
}