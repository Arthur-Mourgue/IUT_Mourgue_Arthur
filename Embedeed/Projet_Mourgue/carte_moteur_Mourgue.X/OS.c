#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"
#include "ADC.h"
#include "Robot.h"
#include "OS.h"

#define STATE_ATTENTE 0
#define STATE_ATTENTE_EN_COURS 1
#define STATE_AVANCE 2
#define STATE_AVANCE_EN_COURS 3
#define STATE_TOURNE_GAUCHE 4
#define STATE_TOURNE_GAUCHE_EN_COURS 5
#define STATE_TOURNE_DROITE 6
#define STATE_TOURNE_DROITE_EN_COURS 7
#define STATE_TOURNE_SUR_PLACE_GAUCHE 8
#define STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS 9
#define STATE_TOURNE_SUR_PLACE_DROITE 10
#define STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS 11
#define STATE_RECULE_PAR_GAUCHE 16
#define STATE_RECULE_PAR_GAUCHE_EN_COURS 17
#define STATE_RECULE_PAR_DROITE 18
#define STATE_RECULE_PAR_DROITE_EN_COURS 19
#define STATE_ARRET 12
#define STATE_ARRET_EN_COURS 13
#define STATE_RECULE 14
#define STATE_RECULE_EN_COURS 15

#define PAS_D_OBSTACLE 0
#define OBSTACLE_A_GAUCHE 1
#define OBSTACLE_A_DROITE 2
#define OBSTACLE_EN_FACE 3

unsigned long timestamp;

unsigned char stateRobot;
unsigned char state;

unsigned char AcquireTelemetreVerite(void) {
    state = 0b00000;
    if (robotState.distanceTelemetreGauche2 < 30) {
        state |= 0b10000;
    }
    if (robotState.distanceTelemetreGauche < 40) {
        state |= 0b01000;
    }
    if (robotState.distanceTelemetreCentre < 35) {
        state |= 0b00100;
    }
    if (robotState.distanceTelemetreDroit < 40) {
        state |= 0b00010;
    }
    if (robotState.distanceTelemetreDroit2 < 30) {
        state |= 0b00001;
    }
    return state;
}
void OperatingSystemLoopVerite(void) {

    switch (state) {

        case 0b00001:
            //Tourne un tout petit peu a Gauche
            PWMSetSpeedConsigne(30, MOTEUR_DROIT);
            PWMSetSpeedConsigne(20, MOTEUR_GAUCHE);
            state = AcquireTelemetreVerite();
            break;
        case 0b10000:
            //Tourne un tout petit peu a Droite
            PWMSetSpeedConsigne(20, MOTEUR_DROIT);
            PWMSetSpeedConsigne(30, MOTEUR_GAUCHE);
            state = AcquireTelemetreVerite();
            break;
        case 0b00010:
            //Tourne Gauche+++
            PWMSetSpeedConsigne(30, MOTEUR_DROIT);
            PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
            state = AcquireTelemetreVerite();
            break;
        case 0b01000:
            //Tourne Droite+++
            PWMSetSpeedConsigne(15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(30, MOTEUR_GAUCHE);
            state = AcquireTelemetreVerite();
            break;
        case 0b00011:
            //Tourne Gauche++  ???????????
            PWMSetSpeedConsigne(30, MOTEUR_DROIT);
            PWMSetSpeedConsigne(20, MOTEUR_GAUCHE);
            state = AcquireTelemetreVerite();
            break;
        case 0b11000:
            //Tourne Droite++  ???????????
            PWMSetSpeedConsigne(20, MOTEUR_DROIT);
            PWMSetSpeedConsigne(30, MOTEUR_GAUCHE);
            state = AcquireTelemetreVerite();
            break;
        case 0b00101:
            //Tourne sur place Gauche
            PWMSetSpeedConsigne(10, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-10, MOTEUR_GAUCHE);
            state = AcquireTelemetreVerite();
            break;
        case 0b10100:
            //Tourne sur place Droite
            PWMSetSpeedConsigne(-10, MOTEUR_DROIT);
            PWMSetSpeedConsigne(10, MOTEUR_GAUCHE);
            state = AcquireTelemetreVerite();
        case 0b00110:
            //Tourne Gauche++++
            PWMSetSpeedConsigne(30, MOTEUR_DROIT);
            PWMSetSpeedConsigne(5, MOTEUR_GAUCHE);
            state = AcquireTelemetreVerite();
            break;
        case 0b01100:
            //Tourne Droite++++
            PWMSetSpeedConsigne(5, MOTEUR_DROIT);
            PWMSetSpeedConsigne(30, MOTEUR_GAUCHE);
            state = AcquireTelemetreVerite();
            break;
        case 0b00111:
            //Tourne sur place Gauche
            PWMSetSpeedConsigne(10, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-10, MOTEUR_GAUCHE);
            state = AcquireTelemetreVerite();
            break;
        case 0b11100:
            //Tourne sur place Droite
            PWMSetSpeedConsigne(-10, MOTEUR_DROIT);
            PWMSetSpeedConsigne(10, MOTEUR_GAUCHE);
            state = AcquireTelemetreVerite();
            break;
        case 0b01001:
            //Tourne un peu vers la Droite
            PWMSetSpeedConsigne(25, MOTEUR_DROIT);
            PWMSetSpeedConsigne(30, MOTEUR_GAUCHE);
            state = AcquireTelemetreVerite();
            break;
        case 0b10010:
            //Tourne un peu vers la Gauche
            PWMSetSpeedConsigne(30, MOTEUR_DROIT);
            PWMSetSpeedConsigne(25, MOTEUR_GAUCHE);
            state = AcquireTelemetreVerite();
            break;
        case 0b01111:
            //Tourne sur place Gauche++
            PWMSetSpeedConsigne(-20, MOTEUR_DROIT);
            PWMSetSpeedConsigne(20, MOTEUR_GAUCHE);
            state = AcquireTelemetreVerite();
            break;
        case 0b11110:
            //Tourne sur place Droite++
            PWMSetSpeedConsigne(20, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-20, MOTEUR_GAUCHE);
            state = AcquireTelemetreVerite();
            break;
        case 0b01101:
            //Tourne sur place Gauche
            PWMSetSpeedConsigne(10, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-10, MOTEUR_GAUCHE);
            state = AcquireTelemetreVerite();
            break;
        case 0b10110:
            //Tourne sur place Droite
            PWMSetSpeedConsigne(-10, MOTEUR_DROIT);
            PWMSetSpeedConsigne(10, MOTEUR_GAUCHE);
            state = AcquireTelemetreVerite();
            break;
        case 0b10111:
            //Tourne sur place Gauche+
            PWMSetSpeedConsigne(15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-15, MOTEUR_GAUCHE);
            state = AcquireTelemetreVerite();
            break;
        case 0b11101:
            //Tourne sur place Droite+
            PWMSetSpeedConsigne(-15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
            state = AcquireTelemetreVerite();
            break;
        case 0b11001:
            //Tourne un peu a Droite
            PWMSetSpeedConsigne(25, MOTEUR_DROIT);
            PWMSetSpeedConsigne(30, MOTEUR_GAUCHE);
            state = AcquireTelemetreVerite();
            break;
        case 0b10011:
            //Tourne un peu a Gauche
            PWMSetSpeedConsigne(30, MOTEUR_DROIT);
            PWMSetSpeedConsigne(25, MOTEUR_GAUCHE);
            state = AcquireTelemetreVerite();
            break;
        case 0b10101:
            //Compare Gauche2 et Droite2
            if (robotState.distanceTelemetreDroit2 > robotState.distanceTelemetreGauche2) {
                //Tourne a sur place Droite
                PWMSetSpeedConsigne(-10, MOTEUR_DROIT);
                PWMSetSpeedConsigne(10, MOTEUR_GAUCHE);
            } else {
                //Tourne sur place a Gauche
                PWMSetSpeedConsigne(10, MOTEUR_DROIT);
                PWMSetSpeedConsigne(-10, MOTEUR_GAUCHE);
            }
            state = AcquireTelemetreVerite();
            break;
        case 0b01110:
            if (robotState.distanceTelemetreDroit2 > robotState.distanceTelemetreGauche2) {
                //Tourne a  Droite
                PWMSetSpeedConsigne(-10, MOTEUR_DROIT);
                PWMSetSpeedConsigne(10, MOTEUR_GAUCHE);
            } else {
                //Tourne a sur place Gauche
                PWMSetSpeedConsigne(-10, MOTEUR_DROIT);
                PWMSetSpeedConsigne(-10, MOTEUR_GAUCHE);
            }
            state = AcquireTelemetreVerite();
            break;
        case 0b11111:
            //Compare Tout les capteurs
            if (robotState.distanceTelemetreDroit > robotState.distanceTelemetreGauche) {
                //Tourne sur place a  Droite
                PWMSetSpeedConsigne(10, MOTEUR_DROIT);
                PWMSetSpeedConsigne(20, MOTEUR_GAUCHE);
            } else {
                //Tourne sur place a sur place Gauche
                PWMSetSpeedConsigne(20, MOTEUR_DROIT);
                PWMSetSpeedConsigne(10, MOTEUR_GAUCHE);
            }
            state = AcquireTelemetreVerite();
            break;
        case 0b10001:
            //Compare droite et gauche +2 pour le moteur le plus loins
            if (robotState.distanceTelemetreDroit2 > robotState.distanceTelemetreGauche2) {
                // +2 moteur gauche
                PWMSetSpeedConsigne(18, MOTEUR_DROIT);
                PWMSetSpeedConsigne(20, MOTEUR_GAUCHE);
            } else {
                // +2 moteur droite
                PWMSetSpeedConsigne(20, MOTEUR_DROIT);
                PWMSetSpeedConsigne(18, MOTEUR_GAUCHE);
            }
            state = AcquireTelemetreVerite();
            break;
        case 0b11011:
            //Compare tout capteur et +2 pour le moteur le plus loins
            if (robotState.distanceTelemetreDroit > robotState.distanceTelemetreGauche) {
                //+2 moteur  gauche
                PWMSetSpeedConsigne(18, MOTEUR_DROIT);
                PWMSetSpeedConsigne(20, MOTEUR_GAUCHE);
            } else {
                //+2 moteur droite
                PWMSetSpeedConsigne(20, MOTEUR_DROIT);
                PWMSetSpeedConsigne(18, MOTEUR_GAUCHE);
            }
            state = AcquireTelemetreVerite();
            break;
        case 0b00000:
            //Avance
            PWMSetSpeedConsigne(25, MOTEUR_DROIT);
            PWMSetSpeedConsigne(25, MOTEUR_GAUCHE);
            state = AcquireTelemetreVerite();
            break;

    }

}

/*
void OperatingSystemLoop(void) {
    switch (stateRobot) {
        case STATE_ATTENTE:
            timestamp = 0;
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_ATTENTE_EN_COURS;

        case STATE_ATTENTE_EN_COURS:
            if (timestamp > 1000)
                stateRobot = STATE_AVANCE;
            break;

        case STATE_AVANCE:
            PWMSetSpeedConsigne(30, MOTEUR_DROIT);
            PWMSetSpeedConsigne(30, MOTEUR_GAUCHE);
            stateRobot = STATE_AVANCE_EN_COURS;
            break;
        case STATE_AVANCE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_GAUCHE:
            PWMSetSpeedConsigne(20, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_DROITE:
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(20, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_SUR_PLACE_GAUCHE:
            PWMSetSpeedConsigne(15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_SUR_PLACE_DROITE:
            PWMSetSpeedConsigne(-15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_RECULE_PAR_GAUCHE:
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-30, MOTEUR_GAUCHE);
            stateRobot = STATE_RECULE_PAR_GAUCHE_EN_COURS;
            break;
        case STATE_RECULE_PAR_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_RECULE_PAR_DROITE:
            PWMSetSpeedConsigne(-30, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_RECULE_PAR_DROITE_EN_COURS;
            break;
        case STATE_RECULE_PAR_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        default:
            stateRobot = STATE_ATTENTE;
            break;
    }
}

unsigned char nextStateRobot = 0;

void SetNextRobotStateInAutomaticMode() {
    unsigned char positionObstacle = PAS_D_OBSTACLE;

    //D�termination de la position des obstacles en fonction des t�l�m�tres
    if (robotState.distanceTelemetreDroit < 30 &&
            robotState.distanceTelemetreCentre > 20 &&
            robotState.distanceTelemetreGauche > 30) //Obstacle � droite
        positionObstacle = OBSTACLE_A_DROITE;
    else if (robotState.distanceTelemetreDroit > 30 &&
            robotState.distanceTelemetreCentre > 20 &&
            robotState.distanceTelemetreGauche < 30) //Obstacle � gauche
        positionObstacle = OBSTACLE_A_GAUCHE;
    else if (robotState.distanceTelemetreCentre < 20) //Obstacle en face
        positionObstacle = OBSTACLE_EN_FACE;
    else if (robotState.distanceTelemetreDroit > 30 &&
            robotState.distanceTelemetreCentre > 20 &&
            robotState.distanceTelemetreGauche > 30) //pas d?obstacle
        positionObstacle = PAS_D_OBSTACLE;

    //D�termination de l?�tat � venir du robot
    if (positionObstacle == PAS_D_OBSTACLE)
        nextStateRobot = STATE_AVANCE;
    else if (positionObstacle == OBSTACLE_A_DROITE)
        nextStateRobot = STATE_TOURNE_GAUCHE;
    else if (positionObstacle == OBSTACLE_A_GAUCHE)
        nextStateRobot = STATE_TOURNE_DROITE;
    else if (positionObstacle == OBSTACLE_EN_FACE)
        nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;

    //Si l?on n?est pas dans la transition de l?�tape en cours
    if (nextStateRobot != stateRobot - 1) { 
        stateRobot = nextStateRobot;
    }

    
}
unsigned int Telemetre;
int Fct(Telemetre){
    
}
*/