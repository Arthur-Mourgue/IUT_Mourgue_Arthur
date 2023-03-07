/* 
 * File:   asservissement.h
 * Author: GEII Robot
 *
 * Created on 6 février 2023, 14:21
 */

#ifndef ASSERVISSEMENT_H
#define	ASSERVISSEMENT_H

#define PID_LINEAIRE 0
#define PID_ANGULAIRE 1

typedef struct _PidCorrector {
    float Kp;
    float Ki;
    float Kd;
    float erreurProportionelleMax;
    float erreurIntegraleMax;
    float erreurDeriveeMax;
    float erreurIntegrale;
    float epsilon_1;
    float consigne;
    float erreur;
    //For Debug only
    float corrP;
    float corrI;
    float corrD;
    
     
} PidCorrector;

void SetupPidAsservissement(volatile PidCorrector* PidCorr, float Kp, float Ki, float Kd, float proportionelleMax, float integralMax, float deriveeMax);
void SendAsservissementVariables(volatile PidCorrector* PidCorr, unsigned char PIDdescriptor);
void SendAsservissementConstant(volatile PidCorrector* PidCorr, unsigned char PIDdescriptor);
double Correcteur(volatile PidCorrector* PidCorr, double erreur);
void UpdateAsservissement();
#endif	/* ASSERVISSEMENT_H */

