/* 
 * File:   UART_Protocol.h
 * Author: TP_EO_6
 *
 * Created on 4 janvier 2023, 16:04
 */

#ifndef UART_PROTOCOL_H
#define	UART_PROTOCOL_H


#define Waiting 1
#define FunctionMSB 2
#define FunctionLSB 3
#define PayloadLengthMSB 4
#define PayloadLengthLSB 5
#define Payload 6
#define CheckSum 7

#define SET_ROBOT_STATE 0x0051
#define SET_ROBOT_PID_ASSERVICEMENT 0x0063
#define SET_ROBOT_MANUAL_CONTROL 0x0052

unsigned char UartCalculateChecksum(int msgFunction,int msgPayloadLength, unsigned char* msgPayload);
void UartEncodeAndSendMessage(int msgFunction,int msgPayloadLength, unsigned char* msgPayload);
void UartDecodeMessage(unsigned char c);
void UartProcessDecodedMessage(unsigned char function,unsigned char payloadLength, unsigned char* payload);
void SetRobotState(unsigned char payload);
void SetRobotAutoControlState(unsigned char payload);

extern unsigned char autoControlActivated ;
extern unsigned char stateRobot; 


#endif	/* UART_PROTOCOL_H */

