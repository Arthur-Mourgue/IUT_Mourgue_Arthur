/* 
 * File:   asservissement.h
 * Author: GEII Robot
 *
 * Created on 6 f�vrier 2023, 14:21
 */

#ifndef ASSERVISSEMENT_H
#define	ASSERVISSEMENT_H


typedef struct _PidCorrector
{
double Kp;
double Ki;
double Kd;
double erreurProportionelleMax;
double erreurIntegraleMax;
double erreurDeriveeMax;
double erreurIntegrale;
double epsilon_1;
double erreur;
//For Debug only
double corrP;
double corrI;
double corrD;
}PidCorrector;

void SetupPidAsservissement(volatile PidCorrector* PidCorr, double Kp, double Ki, double Kd, double pro, double proportionelleMax, double integralMax, double deriveeMax);

#endif	/* ASSERVISSEMENT_H */

