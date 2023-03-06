/* 
 * File:   asservissement.h
 * Author: GEII Robot
 *
 * Created on 6 février 2023, 14:21
 */

#ifndef ASSERVISSEMENT_H
#define	ASSERVISSEMENT_H

typedef struct _PidCorrector {
    float Kp;
    float Ki;
    float Kd;
    float erreurProportionelleMax;
    float erreurIntegraleMax;
    float erreurDeriveeMax;
    float erreurIntegrale;
    float epsilon_1;
    float erreur;
    //For Debug only
    float corrP;
    float corrI;
    float corrD;
} PidCorrector;

void SetupPidAsservissement(volatile PidCorrector* PidCorr, float Kp, float Ki, float Kd, float proportionelleMax, float integralMax, float deriveeMax);
void SendAsservissement(volatile PidCorrector* PidCorr);
#endif	/* ASSERVISSEMENT_H */

