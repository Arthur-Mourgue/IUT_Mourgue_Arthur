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
#include "UART_Protocol.h"

unsigned long timestamp;
unsigned long startingActionTimestamp;
unsigned char stateRobot;
unsigned char state;

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
            PWMSetSpeedConsigne(20, MOTEUR_DROIT);
            PWMSetSpeedConsigne(20, MOTEUR_GAUCHE);
            stateRobot = STATE_AVANCE_EN_COURS;
            break;
        case STATE_AVANCE_EN_COURS:
            if (autoControlActivated == 1)SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_GAUCHE:
            PWMSetSpeedConsigne(25, MOTEUR_DROIT);
            PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_GAUCHE_EN_COURS:
            if (autoControlActivated == 1)SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_DROITE:
            PWMSetSpeedConsigne(15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(25, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_DROITE_EN_COURS:
            if (autoControlActivated == 1)SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_SUR_PLACE_GAUCHE:
            PWMSetSpeedConsigne(15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS:
            if (autoControlActivated == 1)SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_SUR_PLACE_DROITE:
            PWMSetSpeedConsigne(-15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS:
            if (autoControlActivated == 1)SetNextRobotStateInAutomaticMode();
            break;
        case STATE_RECULE_PAR_GAUCHE:
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-15, MOTEUR_GAUCHE);
            stateRobot = STATE_RECULE_PAR_GAUCHE_EN_COURS;
            break;
        case STATE_RECULE_PAR_GAUCHE_EN_COURS:
            if (autoControlActivated == 1)SetNextRobotStateInAutomaticMode();
            break;
        case STATE_RECULE_PAR_DROITE:
            PWMSetSpeedConsigne(-15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_RECULE_PAR_DROITE_EN_COURS;
            break;
        case STATE_RECULE_PAR_DROITE_EN_COURS:
            if (autoControlActivated == 1)SetNextRobotStateInAutomaticMode();
            break;
        case STATE_RECULE_UN_PEU_DROITE:
            PWMSetSpeedConsigne(-15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_RECULE_PAR_DROITE_EN_COURS;
            break;
        case STATE_RECULE_UN_PEU_DROITE_EN_COURS:
            if (autoControlActivated == 1)SetNextRobotStateInAutomaticMode();
            break;
        case STATE_RECULE_UN_PEU_GAUCHE:
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-15, MOTEUR_GAUCHE);
            stateRobot = STATE_RECULE_UN_PEU_GAUCHE_EN_COURS;
            break;
        case STATE_RECULE_UN_PEU_GAUCHE_EN_COURS:
            if (autoControlActivated == 1)SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_UN_PEU_GAUCHE:
            PWMSetSpeedConsigne(25, MOTEUR_DROIT);
            PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_UN_PEU_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_UN_PEU_GAUCHE_EN_COURS:
            if (autoControlActivated == 1)SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_UN_PEU_DROITE:
            PWMSetSpeedConsigne(15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(25, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_UN_PEU_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_UN_PEU_DROITE_EN_COURS:
            if (autoControlActivated == 1)SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_BCP_DROITE:
            PWMSetSpeedConsigne(5, MOTEUR_DROIT);
            PWMSetSpeedConsigne(25, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_BCP_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_BCP_DROITE_EN_COURS:
            if (autoControlActivated == 1)SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_BCP_GAUCHE:
            PWMSetSpeedConsigne(25, MOTEUR_DROIT);
            PWMSetSpeedConsigne(5, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_BCP_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_BCP_GAUCHE_EN_COURS:
            if (autoControlActivated == 1)SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_MINI_GAUCHE:
            PWMSetSpeedConsigne(25, MOTEUR_DROIT);
            PWMSetSpeedConsigne(20, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_MINI_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_MINI_GAUCHE_EN_COURS:
            if (autoControlActivated == 1)SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_MINI_DROITE:
            PWMSetSpeedConsigne(20, MOTEUR_DROIT);
            PWMSetSpeedConsigne(25, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_MINI_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_MINI_DROITE_EN_COURS:
            if (autoControlActivated == 1)SetNextRobotStateInAutomaticMode();
            break;
        case STATE_180:
            PWMSetSpeedConsigne(15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-15, MOTEUR_GAUCHE);
            startingActionTimestamp = timestamp;
            stateRobot = STATE_180_EN_COURS;
            break;
        case STATE_180_EN_COURS:
            if ((timestamp - startingActionTimestamp) > 1000)
                stateRobot = STATE_AVANCE;
            //SetNextRobotStateInAutomaticMode();
            break;
        case STATE_180_GAUCHE:
            PWMSetSpeedConsigne(15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-15, MOTEUR_GAUCHE);
            stateRobot = STATE_180_GAUCHE_EN_COURS;
            break;
        case STATE_180_GAUCHE_EN_COURS:
            if (autoControlActivated == 1)SetNextRobotStateInAutomaticMode();
            break;
        case STATE_180_DROITE:
            PWMSetSpeedConsigne(-15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
            stateRobot = STATE_180_DROITE_EN_COURS;
            break;
        case STATE_180_DROITE_EN_COURS:
            if (autoControlActivated == 1)SetNextRobotStateInAutomaticMode();
            break;
        case STATE_CELEBRATION:
            PWMSetSpeedConsigne(10, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-10, MOTEUR_GAUCHE);
            startingActionTimestamp = timestamp;
            stateRobot = STATE_CELEBRATION_EN_COURS;
            break;
        case STATE_CELEBRATION_EN_COURS:
            if ((timestamp - startingActionTimestamp) > 4250)
                stateRobot = STATE_AVANCE;
            //SetNextRobotStateInAutomaticMode();
            break;

        case STATE_90_DROITE:
            PWMSetSpeedConsigne(-15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
            startingActionTimestamp = timestamp;
            stateRobot = STATE_90_DROITE_EN_COURS;
            break;
        case STATE_90_DROITE_EN_COURS:
            if ((timestamp - startingActionTimestamp) > 500)
                stateRobot = STATE_AVANCE;
            //SetNextRobotStateInAutomaticMode();
            break;
        case STATE_90_GAUCHE:
            PWMSetSpeedConsigne(15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-15, MOTEUR_GAUCHE);
            startingActionTimestamp = timestamp;
            stateRobot = STATE_90_GAUCHE_EN_COURS;
            break;
        case STATE_90_GAUCHE_EN_COURS:
            if ((timestamp - startingActionTimestamp) > 500)
                stateRobot = STATE_AVANCE;
            //SetNextRobotStateInAutomaticMode();
            break;
        default:
            stateRobot = STATE_ATTENTE;
            break;
    }
}
//HuidaIZ
//iheuhd

unsigned char nextStateRobot = 0;

void SetNextRobotStateInAutomaticMode() {

    //Détermination de la position des obstacles en fonction des télémètres
    if (robotState.distanceTelemetreCentre < 10) {
        if (robotState.distanceTelemetreDroit > robotState.distanceTelemetreGauche) {
            nextStateRobot = STATE_90_DROITE;
        } else {
            nextStateRobot = STATE_90_GAUCHE;
        }
    } else if (robotState.distanceTelemetreGauche < 8) {
        //Recule par la droite
        //nextStateRobot = STATE_RECULE_PAR_DROITE;
        nextStateRobot = STATE_180_DROITE;
    } else if (robotState.distanceTelemetreDroit < 8) {
        //Recule par la gauche
        //nextStateRobot = STATE_RECULE_PAR_GAUCHE;
        nextStateRobot = STATE_180_GAUCHE;
    } else if (robotState.distanceTelemetreGauche2 < 8) {
        //Recule un peu par la droite
        //nextStateRobot = STATE_RECULE_UN_PEU_DROITE;
        nextStateRobot = STATE_180_DROITE;
    } else if (robotState.distanceTelemetreDroit2 < 8) {
        //nextStateRobot = STATE_RECULE_UN_PEU_GAUCHE;
        nextStateRobot = STATE_180_GAUCHE;
    } else {
        state = AcquireTelemetreVerite();
        switch (state) {
            case 0b00001:
                //Tourne un tout petit peu a Gauche
                nextStateRobot = STATE_TOURNE_MINI_GAUCHE;
                break;
            case 0b10000:
                //Tourne un tout petit peu a Droite
                nextStateRobot = STATE_TOURNE_MINI_DROITE;
                break;
            case 0b00010:
                //Tourne Gauche+++
                nextStateRobot = STATE_TOURNE_UN_PEU_GAUCHE;
                //state = AcquireTelemetreVerite();
                break;
            case 0b01000:
                //Tourne Droite+++
                nextStateRobot = STATE_TOURNE_UN_PEU_DROITE;
                //state = AcquireTelemetreVerite();
                break;
            case 0b00011:
                //Tourne Gauche++  ???????????
                nextStateRobot = STATE_TOURNE_UN_PEU_GAUCHE;
                //state = AcquireTelemetreVerite();
                break;
            case 0b11000:
                //Tourne Droite++  ???????????
                nextStateRobot = STATE_TOURNE_UN_PEU_DROITE;
                //state = AcquireTelemetreVerite();
                break;
            case 0b00101:
                //Tourne sur place Gauche
                nextStateRobot = STATE_TOURNE_GAUCHE;
                //state = AcquireTelemetreVerite();
                break;
            case 0b10100:
                //Tourne sur place Droite
                nextStateRobot = STATE_TOURNE_DROITE;
                //state = AcquireTelemetreVerite();
            case 0b00110:
                //Tourne Gauche++++
                nextStateRobot = STATE_TOURNE_GAUCHE;
                //state = AcquireTelemetreVerite();
                break;
            case 0b01100:
                //Tourne Droite++++
                nextStateRobot = STATE_TOURNE_DROITE;
                //state = AcquireTelemetreVerite();
                break;
            case 0b00111:
                //Tourne sur place Gauche
                nextStateRobot = STATE_TOURNE_GAUCHE;
                //state = AcquireTelemetreVerite();
                break;
            case 0b11100:
                //Tourne sur place Droite
                nextStateRobot = STATE_TOURNE_DROITE;
                //state = AcquireTelemetreVerite();
                break;
            case 0b01001:
                //Tourne un peu vers la Droite
                nextStateRobot = STATE_TOURNE_BCP_GAUCHE;
                break;
            case 0b10010:
                //Tourne un peu vers la Gauche
                nextStateRobot = STATE_TOURNE_BCP_DROITE;
                break;
            case 0b01111:
                //Tourne sur place Gauche++
                nextStateRobot = STATE_TOURNE_BCP_GAUCHE;
                break;
            case 0b11110:
                //Tourne sur place Droite++
                nextStateRobot = STATE_TOURNE_BCP_DROITE;
                break;
            case 0b01101:
                //Tourne sur place Gauche
                nextStateRobot = STATE_TOURNE_BCP_GAUCHE;
                break;
            case 0b10110:
                //Tourne sur place Droite
                nextStateRobot = STATE_TOURNE_BCP_DROITE;
                break;
            case 0b10111:
                //Tourne sur place Gauche+
                nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
                break;
            case 0b11101:
                //Tourne sur place Droite+
                nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
                break;
            case 0b11001:
                //Tourne un peu a Droite
                nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
                break;
            case 0b10011:
                //Tourne un peu a Gauche
                nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
                break;
            case 0b00100:
                //Compare Gauche2 et Droite2
                if (robotState.distanceTelemetreDroit > robotState.distanceTelemetreGauche) {
                    nextStateRobot = STATE_90_DROITE;
                } else {
                    nextStateRobot = STATE_90_GAUCHE;
                }
                break;
            case 0b10101:
                //Compare Gauche2 et Droite2
                nextStateRobot = STATE_180;
                break;
            case 0b01110:
                if (robotState.distanceTelemetreDroit2 > robotState.distanceTelemetreGauche2) {
                    // +2 moteur gauche
                    nextStateRobot = STATE_90_DROITE;
                } else {
                    // +2 moteur droite
                    nextStateRobot = STATE_90_GAUCHE;
                }
                break;
            case 0b11111:
                //180°
                nextStateRobot = STATE_180;

                break;
            case 0b10001:
                //Compare droite et gauche +2 pour le moteur le plus loins
                if (robotState.distanceTelemetreDroit2 > robotState.distanceTelemetreGauche2) {
                    // +2 moteur gauche
                    nextStateRobot = STATE_TOURNE_UN_PEU_DROITE;
                } else {
                    // +2 moteur droite
                    nextStateRobot = STATE_TOURNE_UN_PEU_GAUCHE;
                }
                break;
            case 0b11011:
                //Compare tout capteur et +2 pour le moteur le plus loins
                if (robotState.distanceTelemetreDroit2 > robotState.distanceTelemetreGauche2) {
                    //+2 moteur  gauche
                    nextStateRobot = STATE_TOURNE_UN_PEU_DROITE;
                } else {
                    //+2 moteur droite
                    nextStateRobot = STATE_TOURNE_UN_PEU_GAUCHE;
                }
                break;
            case 0b00000:
                //Avance
                nextStateRobot = STATE_AVANCE;
                break;
        }
    }

    //Si l?on n?est pas dans la transition de l?étape en cours
    if (nextStateRobot != stateRobot - 1) {
        stateRobot = nextStateRobot;
        unsigned long time = timestamp;
        unsigned char payload[5];
        payload[0] = stateRobot;
        payload[1] = time >> 24;
        payload[2] = time >> 16;
        payload[3] = time >> 8;
        payload[4] = time >> 0;
        UartEncodeAndSendMessage(0x0050, 5, payload);
    }
}

unsigned char AcquireTelemetreVerite(void) {
    state = 0b00000;
    if (robotState.distanceTelemetreGauche2 < 10) {
        state |= 0b10000;
    }
    if (robotState.distanceTelemetreGauche < 15) {
        state |= 0b01000;
    }
    if (robotState.distanceTelemetreCentre < 25) {
        state |= 0b00100;
    }
    if (robotState.distanceTelemetreDroit < 15) {
        state |= 0b00010;
    }
    if (robotState.distanceTelemetreDroit2 < 10) {
        state |= 0b00001;
    }
    return state;
}


