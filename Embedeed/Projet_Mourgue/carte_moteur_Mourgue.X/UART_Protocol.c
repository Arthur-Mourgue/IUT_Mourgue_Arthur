
#include <xc.h>
#include "UART_Protocol.h"
#include "UART.h"
#include "CB_TX1.h"
#include "OS.h"
#include "asservissement.h"
#include "Utilities.h"
#include "Robot.h"

float LKp,LKi,LKd,LerrP,LerrI,LerrD,AKp,AKi,AKd,AerrP,AerrI,AerrD;
unsigned char PidCor; 

enum MessageFunctions {
    TextMessage = 0x0080,
    LEDValues = 0x0020,
    DistancesTelemetre = 0x0030,
    MotorSpeed = 0x0040,
    PIDAsservicement = 0x0063 
};

unsigned char autoControlActivated = 1;
int rcvState = Waiting;
int msgDecodedFunction = 0;
int msgDecodedPayloadLength = 0;
unsigned char msgDecodedPayload[128];
int msgDecodedPayloadIndex = 0;
unsigned char calculatedChecksum = 0;

volatile PidCorrector* PidCorr ;
float P ;
float I ;
float D ;

unsigned char UartCalculateChecksum(int msgFunction, int msgPayloadLength, unsigned char* msgPayload) {
    //Fonction prenant entree la trame et sa longueur pour calculer le checksum
    unsigned char checksum = 0;
    checksum ^= 0xFE;
    checksum ^= (unsigned char) (msgFunction >> 8);
    checksum ^= (unsigned char) (msgFunction >> 0);
    checksum ^= (unsigned char) (msgPayloadLength >> 8);
    checksum ^= (unsigned char) (msgPayloadLength >> 0);
    int i;
    for (i = 0; i < msgPayloadLength; i++) {
        checksum ^= msgPayload[i];
    }
    return checksum;

}

void UartEncodeAndSendMessage(int msgFunction, int msgPayloadLength, unsigned char* msgPayload) {
    //Fonction d?encodage et d?envoi d?un message

    int pos = 0;
    unsigned char encodeMsg[msgPayloadLength + 6];

    encodeMsg[pos++] = 0xFE;
    encodeMsg[pos++] = (unsigned char) (msgFunction >> 8);
    encodeMsg[pos++] = (unsigned char) (msgFunction >> 0);
    encodeMsg[pos++] = (unsigned char) (msgPayloadLength >> 8);
    encodeMsg[pos++] = (unsigned char) (msgPayloadLength >> 0);
    int i = 0;
    for (i = 0; i < msgPayloadLength; i++) {
        encodeMsg[pos++] = msgPayload[i];
    }
    encodeMsg[pos++] = (unsigned char) (UartCalculateChecksum(msgFunction, msgPayloadLength, msgPayload));
    SendMessage(encodeMsg, pos);
}

void UartDecodeMessage(unsigned char c) {
    //Fonction prenant en entree un octet et servant a reconstituer les trames
    switch (rcvState) {
        case Waiting:
            if (c == 0xFE)
                rcvState = FunctionMSB;
            break;
        case FunctionMSB:
            msgDecodedFunction = c << 8;
            rcvState = FunctionLSB;
            break;
        case FunctionLSB:
            msgDecodedFunction += c << 0;
            rcvState = PayloadLengthMSB;
            break;
        case PayloadLengthMSB:
            msgDecodedPayloadLength = c << 8;
            rcvState = PayloadLengthLSB;
            break;
        case PayloadLengthLSB:
            msgDecodedPayloadLength += c << 0;
            if (msgDecodedPayloadLength == 0)
                rcvState = CheckSum;
            else if (msgDecodedPayloadLength >= 128)
                rcvState = Waiting;
            else {
                msgDecodedPayloadIndex = 0;
                rcvState = Payload;
            }
            break;
        case Payload:
            msgDecodedPayload[msgDecodedPayloadIndex] = c;
            msgDecodedPayloadIndex++;
            if (msgDecodedPayloadIndex >= msgDecodedPayloadLength) {
                rcvState = CheckSum;
            }
            break;
        case CheckSum:
            calculatedChecksum = UartCalculateChecksum(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
            if (calculatedChecksum == c) {
                //Success, on a un message valide
                UartProcessDecodedMessage(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
            } else {
            }
            rcvState = Waiting;
            break;
        default:
            rcvState = Waiting;
            break;
    }
}

void UartProcessDecodedMessage(unsigned char function, unsigned char payloadLength, unsigned char* payload) {
    //Fonction éappele èaprs le édcodage pour éexcuter l?action
    //correspondant au message çreu
    switch (function) {
        case SET_ROBOT_STATE:
            SetRobotState(payload[0]);
            break;
        case SET_ROBOT_MANUAL_CONTROL:
            SetRobotAutoControlState(payload[0]);
            break;
        case SET_ROBOT_PID_ASSERVICEMENT:
            
            LKp = getFloat(payload, 0);
            LKi = getFloat(payload, 4);
            LKd = getFloat(payload, 8);
            LerrP = getFloat(payload, 12);
            LerrI = getFloat(payload, 16);
            LerrD = getFloat(payload, 20);
            
            AKp = getFloat(payload, 24);
            AKi = getFloat(payload, 28);
            AKd = getFloat(payload, 32);
            AerrP = getFloat(payload, 36);
            AerrI = getFloat(payload, 44);
            AerrD = getFloat(payload, 48);
            
            SetupPidAsservissement(&robotState.PidLineaire, LKp, LKi, LKd, LerrP, LerrI, LerrD);
            SetupPidAsservissement(&robotState.PidAngulaire, AKp, AKi, AKd, AerrP, AerrI, AerrD);
            
            SendAsservissementConstant(&robotState.PidLineaire,PID_LINEAIRE);
            SendAsservissementConstant(&robotState.PidAngulaire,PID_ANGULAIRE);

            break;
        default:
            break;
    }
}

void SetRobotState(unsigned char payload) {

    stateRobot = payload;
}

void SetRobotAutoControlState(unsigned char payload) {

    if (payload == 0) {
        //Manuel
        autoControlActivated = 0;

    } else {
        //Auto
        autoControlActivated = 1;

    }
}

//*************************************************************************/
//Fonctions correspondant aux messages
//*************************************************************************/
