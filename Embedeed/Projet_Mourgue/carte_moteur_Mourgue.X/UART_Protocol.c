
#include <xc.h>
#include "UART_Protocol.h"

unsigned char UartCalculateChecksum(int msgFunction,
        int msgPayloadLength, unsigned char* msgPayload) {
    //Fonction prenant entree la trame et sa longueur pour calculer le checksum
    //...
}

void UartEncodeAndSendMessage(int msgFunction,
        int msgPayloadLength, unsigned char* msgPayload) {
    //Fonction d?encodage et d?envoi d?un message

    int pos = 0;
    unsigned char encodeMsg[msgPayloadLength + 6];

    encodeMsg[pos++] += 0xFE;
    encodeMsg[pos++] += (unsigned char) (msgFunction >> 8);
    encodeMsg[pos++] += (unsigned char) (msgFunction >> 0);
    encodeMsg[pos++] += (unsigned char) (msgPayloadLength >> 8);
    encodeMsg[pos++] += (unsigned char) (msgPayloadLength >> 0);
    for (int i = 0; i < msgPayloadLength; i++) {
        encodeMsg[pos++] ^= msgPayload[i];
    }



}
int msgDecodedFunction = 0;
int msgDecodedPayloadLength = 0;
unsigned char msgDecodedPayload[128];
int msgDecodedPayloadIndex = 0;

void UartDecodeMessage(unsigned char c) {
    //Fonction prenant en entree un octet et servant a reconstituer les trames
}

void UartProcessDecodedMessage(int function,
        int payloadLength, unsigned char* payload) {
    //Fonction appelee apres le decodage pour executer l?action
    //correspondant au message recu
    //...
}
//*************************************************************************/
//Fonctions correspondant aux messages
//*************************************************************************/
